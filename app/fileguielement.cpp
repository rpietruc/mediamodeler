#include "fileguielement.h"
#include "ui_fileguielement.h"
#include <QDebug>

namespace media {

FileGuiElement::FileGuiElement(QWidget *aParent) :
    GuiElementBase(aParent),
    mUi(new Ui::FileGuiElement)
    {
    mUi->setupUi(this);
    mUi->pushButton->setText("Open File ...");
    mFileDialog.setFileMode(QFileDialog::AnyFile);

    QObject::connect(&mFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(fileOpen(QString)));
    QObject::connect(mUi->pushButton, SIGNAL(clicked()), &mFileDialog, SLOT(show()));
    }

FileGuiElement::~FileGuiElement()
    {
    delete mUi;
    }

void FileGuiElement::addProperty(const QString &aName, const QVariant &aValue)
    {
    Q_ASSERT(aName == "fileName");
    Q_ASSERT(aValue.canConvert(QVariant::String));
    mUi->label->setText(aValue.toString());
    }

void FileGuiElement::fileOpen(const QString &aPath)
    {
    mUi->label->setText(aPath);
    emit paramChanged("fileName", aPath);
    }

FilesGuiElement::FilesGuiElement(QWidget *aParent) :
    GuiElementBase(aParent),
    mUi(new Ui::FileGuiElement)
    {
    mUi->setupUi(this);
    mUi->pushButton->setText("Open Files ...");
    mFileDialog.setFileMode(QFileDialog::ExistingFiles);

    QObject::connect(&mFileDialog, SIGNAL(filesSelected(QStringList)), this, SLOT(filesOpen(QStringList)));
    QObject::connect(mUi->pushButton, SIGNAL(pressed()), &mFileDialog, SLOT(show()));
    }

FilesGuiElement::~FilesGuiElement()
    {
    delete mUi;
    }

void FilesGuiElement::addProperty(const QString &aName, const QVariant &aValue)
    {
    Q_ASSERT(aName == "fileList");
    Q_ASSERT(aValue.canConvert(QVariant::StringList));
    mUi->label->setText(aValue.toStringList().join("; "));
    }

void FilesGuiElement::filesOpen(const QStringList &aPathList)
    {
    mUi->label->setText(aPathList.join("; "));
    emit paramChanged("fileList", QVariant(aPathList));
    }

} // namespace media

