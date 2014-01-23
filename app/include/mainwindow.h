#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "guielementlist.h"

namespace Ui {
    class MainWindow;
}

namespace media {
    class ElementBase;
    class ElementFactory;
    class ModelCreator;
}

class QThread;

class MainWindow : public QMainWindow
    {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void releaseModel();
    void loadModel(const QString &aFilePath);
    void closeEvent(QCloseEvent * event);
    void handleSetting(const QString& aName, const QVariant& aValue);

private:
    Ui::MainWindow *mUi;
    media::ModelCreator *mCreator;
    QVector<QWidget*> mGuiElements;
    QVector<QThread*> mElemThreads;
    QString mModelFile;
    };

#endif // MAINWINDOW_H
