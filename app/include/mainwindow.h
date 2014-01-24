#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

namespace Ui {
    class MainWindow;
}

namespace media {
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
    void saveSetting(const QString& aSection, const QString& aName, const QVariant& aValue);

private:
    Ui::MainWindow *mUi;
    media::ModelCreator *mCreator;
    QVector<QWidget*> mElemBoxes;
    QVector<QThread*> mElemThreads;
    QString mModelFile;
    };

#endif // MAINWINDOW_H
