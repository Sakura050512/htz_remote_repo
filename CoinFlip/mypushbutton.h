#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
public:
    //explicit MyPushButton(QWidget *parent = nullptr);

    MyPushButton(QString normalImg,QString pressImg="");
    QString normalImgPath;
    QString pressImgPath;

    //弹跳特效
    void zoom1();//向下跳
    void zoom2();//向上跳

    //重写按钮 按下 和释放 事件
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvenet(QMouseEvent *e);

signals:
};

#endif // MYPUSHBUTTON_H
