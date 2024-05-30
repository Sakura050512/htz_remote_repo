#ifndef CHOOSELEAVELSCENE_H
#define CHOOSELEAVELSCENE_H

#include <QMainWindow>
#include "playscene.h"
class ChooseLeavelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLeavelScene(QWidget *parent = nullptr);

    //重写绘图事件
    void paintEvent(QPaintEvent *);

    //游戏场景指针
    PlayScene * play = NULL;

signals:
    //自定义信号 告诉主场景 点击了返回
    void chooseSceneBack();
};

#endif // CHOOSELEAVELSCENE_H
