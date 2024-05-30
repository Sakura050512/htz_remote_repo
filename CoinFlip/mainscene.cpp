#include "mainscene.h"
#include "ui_mainscene.h"
#include "Qpainter"
#include "QPaintEvent"
#include "mypushbutton.h"
#include "QDebug"
#include "QTimer"
#include "QSoundEffect"

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(320,588);

    //设置图标
    setWindowIcon(QIcon(":/Image/Coin0001.png"));

    //设置标题
    setWindowTitle("翻金币主场景");

    //退出按钮实现
    connect(ui->actionQuit,&QAction::triggered,[=](){
                this->close();
            });

    //准备开始按钮的音效
    QSoundEffect * startSound = new QSoundEffect;
    startSound->setSource(QUrl::fromLocalFile(":/Music/TapButtonSound.wav"));

    //开始按钮
    MyPushButton * startBtn = new MyPushButton(":/Image/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);

    //实例化选择关卡
    chooseScene = new ChooseLeavelScene;

            //监听选择关卡的返回按钮的信号
            connect(chooseScene,&ChooseLeavelScene::chooseSceneBack,this,[=](){
                this->setGeometry(chooseScene->geometry());
                chooseScene->hide();
                this->show();
            });

    connect(startBtn,&QPushButton::clicked,[=](){
        //点击开始，播放音效
        startSound->play();
        //做弹跳特效
        startBtn->zoom1();
        startBtn->zoom2();

        //延时进入下一页面
        QTimer::singleShot(500,this,[=](){
            //设置chooseScene 场景的位置
            chooseScene->setGeometry(this->geometry());

            //自身隐藏，进入选择关卡页面
            this->hide();
            chooseScene->show();
            });
        });
}


void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/Image/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //画背景图标
    pix.load(":/Image/Title.png");

    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix);
}
MainScene::~MainScene()
{
    delete ui;
}
