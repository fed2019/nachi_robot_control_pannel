#ifndef OPERATEBUTTON_H
#define OPERATEBUTTON_H
#include <QPushButton>
#include <QMouseEvent>


class operateButton : public QPushButton
{
public:
    operateButton();
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

};

#endif // OPERATEBUTTON_H
