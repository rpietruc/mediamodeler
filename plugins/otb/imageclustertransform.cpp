#include "imageclustertransform.h"

#include <itkImageRegionIterator.h>
#include <itkScalarImageToListAdaptor.h>
#include <itkKdTree.h>
#include <itkKdTreeGenerator.h>
#include <itkMeanShiftModeCacheMethod.h>
#include <itkHypersphereKernelMeanShiftModeSeeker.h>
#include <itkSampleMeanShiftBlurringFilter.h>
#include <itkSampleMeanShiftClusteringFilter.h>

using namespace itk;

namespace media {

ImageClusterTransform::ImageClusterTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

void ImageClusterTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i <source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
                mImageFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                GrayImageFrame::ImageType::Pointer srcImg = mSrcFrame;

                typedef Statistics::ScalarImageToListAdaptor<GrayImageFrame::ImageType> ListSampleType;
                ListSampleType::Pointer listSample = ListSampleType::New();
                listSample->SetImage(srcImg);

                typedef Statistics::KdTreeGenerator<ListSampleType> TreeGeneratorType;
                TreeGeneratorType::Pointer treeGenerator = TreeGeneratorType::New();
                treeGenerator->SetSample(listSample);
                treeGenerator->SetBucketSize(200);
                treeGenerator->Update();

                typedef TreeGeneratorType::KdTreeType TreeType;
                TreeType::Pointer tree = treeGenerator->GetOutput();

                typedef Statistics::HypersphereKernelMeanShiftModeSeeker<TreeType> ModeSeekerType;
                ModeSeekerType::Pointer modeSeeker = ModeSeekerType::New();
                modeSeeker->SetInputSample(tree);
                //modeSeeker->SetInputSample(listSample);
                modeSeeker->SetSearchRadius(4.0);

                typedef Statistics::MeanShiftModeCacheMethod<TreeType::MeasurementVectorType> CacheMethodType;
                CacheMethodType::Pointer cacheMethod = CacheMethodType::New();
                cacheMethod->SetMaximumEntries(255);
                cacheMethod->SetMaximumConsecutiveFailures(100);
                cacheMethod->SetHitRatioThreshold(0.5);
                modeSeeker->SetCacheMethod(cacheMethod.GetPointer());

                typedef Statistics::SampleMeanShiftBlurringFilter<TreeType> FilterType;
                FilterType::Pointer filter = FilterType::New();
                filter->SetInputSample(tree);
                filter->SetMeanShiftModeSeeker(modeSeeker);
                filter->Update();

                std::cout <<"Cache statistics: " <<std::endl;
                cacheMethod->Print(std::cout);

                typedef GrayImageFrame::ImageType OutputImageType;
                OutputImageType::Pointer outputImage = OutputImageType::New();
                outputImage->SetRegions(srcImg->GetLargestPossibleRegion());
                outputImage->Allocate();

                typedef ImageRegionIterator<OutputImageType> ImageIteratorType;
                ImageIteratorType io_iter(outputImage, outputImage->GetLargestPossibleRegion());
                io_iter.GoToBegin();

                FilterType::OutputType::Pointer output = filter->GetOutput();
                FilterType::OutputType::Iterator fo_iter = output->Begin();
                FilterType::OutputType::Iterator fo_end = output->End();

                while (fo_iter != fo_end)
                    {
                    io_iter.Set((GrayImageFrame::PixelType) fo_iter.GetMeasurementVector()[0]);
                    ++fo_iter;
                    ++io_iter;
                    }

                ListSampleType::Pointer listSample2 = ListSampleType::New();
                listSample2->SetImage(outputImage);

                TreeGeneratorType::Pointer treeGenerator2 = TreeGeneratorType::New();
                treeGenerator2->SetSample(listSample2);
                treeGenerator2->SetBucketSize(200);
                treeGenerator2->Update();

                typedef Statistics::SampleMeanShiftClusteringFilter<TreeType> ClusteringMethodType;

                ClusteringMethodType::Pointer clusteringMethod = ClusteringMethodType::New();
                clusteringMethod->SetInputSample(treeGenerator2->GetOutput());
                clusteringMethod->SetThreshold(0.5);
                clusteringMethod->SetMinimumClusterSize(16);
                clusteringMethod->DebugOn();
                clusteringMethod->Update();

                // save clustered srcImg
                OutputImageType::Pointer clusterMap = OutputImageType::New();
                clusterMap->SetRegions(srcImg->GetLargestPossibleRegion());
                clusterMap->Allocate();

                ImageIteratorType m_iter(clusterMap, clusterMap->GetLargestPossibleRegion());
                m_iter.GoToBegin();

                ClusteringMethodType::ClusterLabelsType clusterLabels = clusteringMethod->GetOutput();

                ClusteringMethodType::ClusterLabelsType::iterator co_iter = clusterLabels.begin();

                while (co_iter != clusterLabels.end())
                    {
                    m_iter.Set((GrayImageFrame::PixelType) *co_iter);
                    ++co_iter;
                    ++m_iter;
                    }

                mImageFrame.resizeAndCopyImage(clusterMap);
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
