#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <modelcreator.h>
#include <elementbase.h>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QThread>
#include <modelcreator.h>
#include "guielementlist.h"
#include "fileguielement.h"
#include <QMessageBox>

using namespace media;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCreator(new ModelCreator(this))
    {
    mUi->setupUi(this);
    mCreator->loadElements(QDir(INSTALL_PLUGINS));
#ifndef QT_NO_DEBUG
    mCreator->loadElements(qApp->applicationDirPath());
#endif
    QString modelPath = QSettings(QApplication::organizationName(), QApplication::applicationName()).value(QString("model/last")).toString();
    if (!modelPath.isEmpty())
        {
        mUi->groupBox->setTitle(modelPath);
        loadModel(modelPath);
        }
    QFileDialog *fileDialog = new QFileDialog(this, "Choose Model", QDir().absoluteFilePath(modelPath), "*.ini");
    QObject::connect(mUi->actionOpen, SIGNAL(triggered()), fileDialog, SLOT(show()));
    QObject::connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(loadModel(QString)));
//    QObject::connect(mUi->actionRun, SIGNAL(toggled(bool)), mUi->actionRun, SLOT(setDisabled(bool)), Qt::QueuedConnection);
//    mUi->actionRun->setDisabled(true);
    }

MainWindow::~MainWindow()
    {
    delete mUi;
    }

void MainWindow::closeEvent(QCloseEvent * /*event*/)
    {
    mCreator->clearModel();
    }

void MainWindow::loadModel(const QString& aFilePath)
    {
    QSettings modelFile(aFilePath, QSettings::IniFormat);
    QFileInfo fileInfo(aFilePath);
    mModelFile = fileInfo.fileName();
    QSettings settings(QApplication::organizationName(), mModelFile);

    QLayoutItem *item;
    while ((item = mUi->gridLayout->takeAt(0)) != 0)
        delete item;
    mCreator->clearModel();
    foreach (QWidget* e, mGuiElements)
        delete e;
    mGuiElements.clear();

    int index;
    for (index = 0;; ++index)
        {
        QString pluginName = modelFile.value(QString("nodes/%1").arg(index)).toString();
        if (pluginName.isEmpty())
            break;

        if (index != mCreator->addElement(pluginName))
            {
            QMessageBox::warning(this, "loadModel", QString("Cannot load %1").arg(pluginName));
            return;
            }
        }

    QSet<int> sources;
    for (int i = 0; i < index; ++i)
        {
        ElementBase* elem = mCreator->getElement(i);
        settings.beginGroup(elem->objectName());
        QStringList keys = settings.childKeys();
        foreach (QString key, keys)
            elem->setParamValue(key, settings.value(key));
        settings.endGroup();

        QGroupBox* group = new QGroupBox(elem->objectName(), mUi->groupBox);
        ElementBase::ParamList list = elem->getParams();
        if (!list.isEmpty())
            {
            GuiElementBase* gui = NULL;
            if (list.size() == 1)
                {
                if (list.find("File") != list.end())
                    gui = new FileGuiElement(mUi->groupBox);
                else if (list.find("Files") != list.end())
                    gui = new FilesGuiElement(mUi->groupBox);
                }
            if (!gui)
                gui = new GuiElementList(group);

            gui->init(list);
            QObject::connect(gui, SIGNAL(paramChanged(QString, QVariant)), this, SLOT(handleSetting(QString, QVariant)));
            QObject::connect(gui, SIGNAL(paramChanged(QString, QVariant)), elem, SLOT(setParamValue(QString, QVariant)));

            QVBoxLayout *verticalLayout = new QVBoxLayout(group);
            verticalLayout->addWidget(gui);
            }
        mUi->gridLayout->addWidget(group, i/3, i%3);
        mGuiElements.push_back(group);
        sources.insert(i);

        QThread *elemThread = new QThread(this);
        elem->moveToThread(elemThread);
        elemThread->start();
        }

    for (int i = 0;; ++i)
        {
        QString connectionPair = modelFile.value(QString("edges/%1").arg(i)).toString();
        if (connectionPair.isEmpty())
            break;

        QStringList connectionList = connectionPair.split(" ");
        mCreator->connectElements(connectionList.front().toInt(), connectionList.back().toInt());

        sources.remove(connectionList.back().toInt());
        }

    foreach (int index, sources)
        {
        QObject::connect(mUi->actionRun, SIGNAL(toggled(bool)), mCreator->getElement(index), SLOT(start(bool)), Qt::QueuedConnection);
        QObject::connect(mCreator->getElement(index), SIGNAL(processingCompleted(bool)), mUi->actionRun, SLOT(setDisabled(bool)), Qt::QueuedConnection);
        QObject::connect(mCreator->getElement(index), SIGNAL(processingCompleted(bool)), mUi->actionRun, SLOT(setChecked(bool)), Qt::QueuedConnection);
        }
    QSettings(QApplication::organizationName(), QApplication::applicationName()).setValue(QString("model/last"), aFilePath);
    mUi->groupBox->setTitle(aFilePath);
//    mUi->actionRun->setEnabled(true);
    }

void MainWindow::handleSetting(const QString& aName, const QVariant& aValue)
    {
    for (int i = 0; i < mGuiElements.size(); ++i)
        if ((mGuiElements[i]->children().size() && (sender() == mGuiElements[i]->children().at(0))) ||
            (mGuiElements[i]->children().size() && (sender() == mGuiElements[i]->children().at(1))))
            {
            QSettings(QApplication::organizationName(), mModelFile).setValue(mCreator->getElement(i)->objectName() + "/" + aName, aValue);
            break;
            }
    }
