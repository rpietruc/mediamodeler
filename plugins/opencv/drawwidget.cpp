#include "drawwidget.h"
#include "ui_drawwidget.h"

DrawWidget::DrawWidget(QWidget *aParent) :
    QWidget(aParent),
    mUi(new Ui::DrawWidget)
    {
    mUi->setupUi(this);
    }

DrawWidget::~DrawWidget()
    {
    delete mUi;
    }

void DrawWidget::showPicture(const QImage& aImage)
    {
    resize(aImage.size());
    mUi->label->setPixmap(QPixmap::fromImage(aImage));
    show();
    }
