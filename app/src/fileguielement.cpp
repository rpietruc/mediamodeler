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

    QObject::connect(&mFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(fileOpen(QString)));
    QObject::connect(mUi->pushButton, SIGNAL(clicked()), &mFileDialog, SLOT(show()));
    }

FileGuiElement::~FileGuiElement()
    {
    delete mUi;
    }

void FileGuiElement::init(const ElementBase::ParamList& aParams)
    {
    mUi->label->setText(aParams["File"].toString());
    }

void FileGuiElement::fileOpen(const QString &aPath)
    {
    mUi->label->setText(aPath);
    emit paramChanged("File", aPath);
    }

FilesGuiElement::FilesGuiElement(QWidget *aParent) :
    GuiElementBase(aParent),
    mUi(new Ui::FileGuiElement)
    {
    mUi->setupUi(this);
    mUi->pushButton->setText("Open Files ...");

    QObject::connect(&mFileDialog, SIGNAL(filesSelected(QStringList)), this, SLOT(filesOpen(QStringList)));
    QObject::connect(mUi->pushButton, SIGNAL(pressed()), &mFileDialog, SLOT(show()));
    }

FilesGuiElement::~FilesGuiElement()
    {
    delete mUi;
    }

void FilesGuiElement::init(const ElementBase::ParamList& aParams)
    {
    mUi->label->setText(aParams["Files"].toStringList().join("; "));
    }

void FilesGuiElement::filesOpen(const QStringList &aPathList)
    {
    mUi->label->setText(aPathList.join("; "));
    emit paramChanged("Files", QVariant(aPathList));
    }

} // namespace media

