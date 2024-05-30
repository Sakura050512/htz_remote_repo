#include "playscene.h"
#include "QDebug"
#include "QMenuBar"
#include "QPaintEvent"
#include "QPainter"
#include "QPixmap"
#include "QPropertyAnimation"
#include "QSoundEffect"
#include "QTimer"
#include "QLabel"
#include "QFont"
#include "mypushbutton.h"
#include "mycoin.h"
#include "dataconfig.h"

// PlayScene::PlayScene(QWidget *parent)
//     : QMainWindow{parent}
// {}
PlayScene::PlayScene(int levelNum)
{
    this->levelIndex=levelNum;

    //配置选择关卡场景
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap(":/Image/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡场景");

    //创建菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出 菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //添加音效资源
    //选择按钮播放音效
    QSoundEffect * backSound = new QSoundEffect;
    backSound->setSource(QUrl::fromLocalFile(":/Music/BackButtonSound.wav"));
    //翻金币音效
    QSoundEffect * flipSound = new QSoundEffect;
    flipSound->setSource(QUrl::fromLocalFile(":/Music/ConFlipSound.wav"));
    //胜利音效
    QSoundEffect * winSound = new QSoundEffect;
    winSound->setSource(QUrl::fromLocalFile(":/Music/LevelWinSound.wav"));

    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/Image/BackButton.png",":/Image/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //点击返回
    connect(backBtn,&QPushButton::clicked,[=](){
        //点击返回 播放音效
        backSound->play();
        //告诉主场景 主场景监听选择场景的返回按钮
        // //延时返回
        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    //显示当前关卡数
    QLabel * label= new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1=QString("Level: %1").arg(this->levelIndex);
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(30,this->height()-50,120,50);


    //初始化每个关卡的二维数组
    dataConfig config;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
            this->gameArray[i][j]=config.mData[this->levelIndex][i][j];
    }

    //胜利图片显示
    QLabel * winLabel = new QLabel;
    QPixmap tmpPix;
    tmpPix.load(":/Image/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winLabel->setPixmap(tmpPix);
    winLabel->setParent(this);
    winLabel->move((this->width()-tmpPix.width())*0.5,-tmpPix.height());


    //显示金币背景图案
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            //绘制背景图片
            QPixmap pix=QPixmap(":/Image/BoardNode.png");
            QLabel  * label = new QLabel;
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->setParent(this);
            label->move(57+i*50,200+j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j]==1)
            {
                str=":/Image/Coin0001.png";
            }
            else
            {
                str=":/Image/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move(59+i*50,204+j*50);

            //给金币属性赋值
            coin->posX=i;
            coin->posY=j;
            coin->flag=this->gameArray[i][j];//1正面 0反面

            //将金币放入金币的二维数组中 以便日后维护
            coinBtn[i][j]=coin;

            //点击金币 进行翻转
            connect(coin,&MyCoin::clicked,[=](){
                //金币翻转 播放音效
                flipSound->play();
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<4;j++)
                    {
                        this->coinBtn[i][j]->isWin=true;
                    }
                }
                coin->changeFlag();
                this->gameArray[i][j]=this->gameArray[i][j]==0 ? 1 : 0;

                //延时翻转周围硬币
                QTimer::singleShot(300,this,[=](){
                    if(coin->posX+1<=3)
                    {
                        coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX+1][coin->posY]==this->gameArray[coin->posX+1][coin->posY]==0 ? 1 : 0;
                    }
                    if(coin->posX-1>=0)
                    {
                        coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        this->gameArray[coin->posX-1][coin->posY]==this->gameArray[coin->posX-1][coin->posY]==0 ? 1 : 0;
                    }
                    if(coin->posY+1<=3)
                    {
                        coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY+1]==this->gameArray[coin->posX][coin->posY+1]==0 ? 1 : 0;
                    }
                    if(coin->posY-1>=0)
                    {
                        coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        this->gameArray[coin->posX][coin->posY-1]==this->gameArray[coin->posX][coin->posY-1]==0 ? 1 : 0;
                    }

                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            this->coinBtn[i][j]->isWin=false;
                        }
                    }

                    //判断是否胜利
                    this->isWin = true;
                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            if(coinBtn[i][j]->flag==false)
                            {
                                this->isWin=false;
                                break;
                            }
                        }
                    }
                    if(this->isWin==true)
                    {
                        //胜利了 播放音效
                        winSound->play();
                        qDebug()<<"游戏胜利";
                        //胜利之后无法再次点击
                        for(int i=0;i<4;i++)
                        {
                            for(int j=0;j<4;j++)
                            {
                                coinBtn[i][j]->isWin=true;
                            }
                        }

                        //胜利图片弹出
                        QPropertyAnimation * animation = new QPropertyAnimation(winLabel,"geometry");
                        //设置时间间隔
                        animation->setDuration(1000);
                        //设置开始位置
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        //设置结束位置
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                        //设置缓和曲线
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        //执行动画
                        animation->start();
                    }
                });
            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *)
{
    //创建背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/Image/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/Image/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
