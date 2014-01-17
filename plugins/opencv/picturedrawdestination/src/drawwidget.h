#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>
#include <QImage>

namespace Ui {
    class DrawWidget;
}

class DrawWidget : public QWidget
    {
    Q_OBJECT

public:
    explicit DrawWidget(QWidget *aParent = 0);
    ~DrawWidget();

    void showPicture(const QImage& aImage);

private:
    Ui::DrawWidget *mUi;
    };

#endif // DRAWWIDGET_H
