#include "regiongrowingpcnn.h"

#include <popt.h>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

void usage(poptContext optCon, int exitcode, char *error, char *addl)
    {
    poptPrintUsage(optCon, stderr, 0);
    if (error)
        fprintf(stderr, "%s: %s", error, addl);
    exit(exitcode);
    }

int main(int argc, const char **argv)
    {
    char *fileIn = 0;

    double beta_min = .2;    char beta_min_default[16];  sprintf(beta_min_default,  "%.2f", beta_min);
    double beta_max = 1.;    char beta_max_default[16];  sprintf(beta_max_default,  "%.2f", beta_max);
    double beta_step = 0.01; char beta_step_default[16]; sprintf(beta_step_default, "%.2f", beta_step);
    double d = .5;           char d_default[16];         sprintf(d_default,         "%.2f", d);
    double sbmax = .1;       char sbmax_default[16];     sprintf(sbmax_default,     "%.2f", sbmax);
 
    poptContext optCon;   /* context for parsing command-line options */
    struct poptOption optionsTable[] =
        {
        {"input-file", 'i', POPT_ARG_STRING, &fileIn, 0, "Input file", 0},
        {0,            'd', POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &d,          0, "Positive constant",                   d_default},
        {"beta-min",    0,  POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &beta_min,   0, "Linking coefficient minimum value",   beta_min_default},
        {"beta-max",    0,  POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &beta_max,   0, "Linking coefficient maximum value",   beta_max_default},
        {"beta-step",   0,  POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &beta_step,  0, "Linking coefficient step value",      beta_step_default},
        {"threshold",  'S', POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &sbmax,      0, "Excessive mean difference threshold", sbmax_default},
        POPT_AUTOHELP
        POPT_TABLEEND
        };

    optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
    poptSetOtherOptionHelp(optCon, "[OPTIONS]* <port>");

    int c = poptGetNextOpt(optCon);
    if (c < -1)
        {
        /* an error occurred during option processing */
        fprintf(stderr, "%s: %s\n", poptBadOption(optCon, POPT_BADOPTION_NOALIAS), poptStrerror(c));
        exit(1);
        }

    if (argc < 4)
        {
        poptPrintUsage(optCon, stderr, 0);
        exit(1);
        }
 
    const char *fileOut = poptGetArg(optCon);
    if (!fileOut || poptPeekArg(optCon))
        usage(optCon, 1, "Specify an output file", ".e.g., output.png");

    if (!fileIn)
        usage(optCon, 1, "Specify an input file", ".e.g., input.png");

    if (beta_min >= beta_max)
        usage(optCon, 1, "beta_max must be greater than beta_min", "");

    IplImage *imgIn = cvLoadImage(fileIn, CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *imgOut = cvCreateImage(cvGetSize(imgIn), IPL_DEPTH_8U, 1);

    regionGrowingPcnn(imgIn, imgOut, beta_min, beta_max, beta_step, d, sbmax);

    normalize(imgOut);
    int res = cvSaveImage(fileOut, imgOut, CV_IMWRITE_PNG_STRATEGY_DEFAULT);
    if (!res)
        fprintf(stderr, "save image returned %d\n", res);

    cvReleaseImage(&imgOut);
    cvReleaseImage(&imgIn);

    poptFreeContext(optCon);
    exit(0);
    }
