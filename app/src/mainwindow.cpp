#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <modelcreator.h>
#include <QFileDialog>
#include <QThread>
#include <QDebug>
#include "propertiesbox.h"
#include <QMessageBox>

using namespace media;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mUi(new Ui::MainWindow),
    mCreator(new ModelCreator(this))
    {
    mUi->setupUi(this);
    mCreator->loadFactories(QDir(INSTALL_PLUGINS));
    QString modelDir = INSTALL_INI;
    QString modelPath = QSettings(QApplication::organizationName(), QApplication::applicationName()).value(QString("model/last")).toString();
    if (!modelPath.isEmpty())
        {
        mUi->groupBox->setTitle(modelPath);
        loadModel(modelPath);
        modelDir = QDir().absoluteFilePath(modelPath);
        }
    QFileDialog *fileDialog = new QFileDialog(this, "Choose Model", modelDir, "*.ini");
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
    try { releaseModel(); }
    catch(...) { qDebug() << "releaseModel exception catched"; }
    }

void MainWindow::releaseModel()
    {
    QLayoutItem *item;
    while ((item = mUi->gridLayout->takeAt(0)) != 0)
        delete item;
    mCreator->deleteAllElements();
    foreach (QWidget* e, mElemBoxes)
        delete e;
    mElemBoxes.clear();
    foreach (QThread* thread, mElemThreads)
        thread->quit();
    foreach (QThread* thread, mElemThreads)
        thread->wait(1000);

    mElemThreads.clear();
    }

void MainWindow::loadModel(const QString& aFilePath)
    {
    QSettings modelFile(aFilePath, QSettings::IniFormat);
    QFileInfo fileInfo(aFilePath);
    mModelFile = fileInfo.fileName();
    QSettings settings(QApplication::organizationName(), mModelFile);

    releaseModel();

    int index;
    for (index = 0;; ++index)
        {
        QString pluginName = modelFile.value(QString("nodes/%1").arg(index)).toString();
        if (pluginName.isEmpty())
            break;

        if (index != mCreator->createElement(pluginName))
            {
            QMessageBox::warning(this, "loadModel", QString("Cannot load %1").arg(pluginName));
            return;
            }
        }

    for (int i = 0; i < index; ++i)
        {
        ElementBase* elem = mCreator->getElement(i);
        settings.beginGroup(elem->objectName());
        QStringList keys = settings.childKeys();
        foreach (QString key, keys)
            elem->setProperty(qPrintable(key), settings.value(key));
        settings.endGroup();

        PropertiesBox* box = new PropertiesBox(elem, mUi->groupBox);
        QObject::connect(box, SIGNAL(settingChanged(QString, QString, QVariant)), this, SLOT(saveSetting(QString, QString, QVariant)));
        mUi->gridLayout->addWidget(box, i/3, i%3);
        mElemBoxes.push_back(box);

        QObject::connect(mUi->actionRun, SIGNAL(toggled(bool)), elem, SLOT(setRunning(bool)), Qt::QueuedConnection);
//        QObject::connect(mUi->actionRun, SIGNAL(toggled(bool)), mUi->groupBox, SLOT(setDisabled(bool)), Qt::QueuedConnection);
        //TODO: processing should be completed when all sources finish
        QObject::connect(elem, SIGNAL(processingCompleted(bool)), mUi->actionRun, SLOT(setDisabled(bool)), Qt::QueuedConnection);
        QObject::connect(elem, SIGNAL(processingCompleted(bool)), mUi->actionRun, SLOT(setChecked(bool)), Qt::QueuedConnection);

        QThread *elemThread = new QThread(this);
        elem->setParent(NULL); //cannot moveToThread object with a parent
        elem->moveToThread(elemThread);
        QObject::connect(elemThread, SIGNAL(finished()), elem, SLOT(deleteLater()));
        QObject::connect(elemThread, SIGNAL(finished()), elemThread, SLOT(deleteLater()));
        mElemThreads.push_back(elemThread);
        }

    foreach (QThread* thread, mElemThreads)
        thread->start();

    for (int i = 0;; ++i)
        {
        QString connectionPair = modelFile.value(QString("edges/%1").arg(i)).toString();
        if (connectionPair.isEmpty())
            break;

        QStringList connectionList = connectionPair.split(" ");
        mCreator->connectElements(connectionList.front().toInt(), connectionList.back().toInt());
        }

    QSettings(QApplication::organizationName(), QApplication::applicationName()).setValue(QString("model/last"), aFilePath);
    mUi->groupBox->setTitle(aFilePath);
//    mUi->actionRun->setEnabled(true);
    }

void MainWindow::saveSetting(const QString& aSection, const QString& aName, const QVariant& aValue)
    {
    QSettings(QApplication::organizationName(), mModelFile).setValue(aSection + "/" + aName, aValue);
    }
