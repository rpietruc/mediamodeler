#ifndef FILEGUIELEMENT_H
#define FILEGUIELEMENT_H

#include "guielementbase.h"
#include <QFileDialog>

namespace Ui {
    class FileGuiElement;
}

namespace media {

class FileGuiElement : public GuiElementBase
    {
    Q_OBJECT

public:
    explicit FileGuiElement(QWidget *aParent = 0);
    ~FileGuiElement();

    void addProperty(const QString &aName, const QVariant &aValue);

private slots:
    void fileOpen(const QString &aPath);

protected:
    QFileDialog mFileDialog;

private:
    Ui::FileGuiElement *mUi;
    };

class FilesGuiElement : public GuiElementBase
    {
    Q_OBJECT

public:
    explicit FilesGuiElement(QWidget *aParent = 0);
    ~FilesGuiElement();

    void addProperty(const QString &aName, const QVariant &aValue);

private slots:
    void filesOpen(const QStringList &aPathList);

private:
    Ui::FileGuiElement *mUi;
    QFileDialog mFileDialog;
    };

} // namespace media

#endif // FILEGUIELEMENT_H
