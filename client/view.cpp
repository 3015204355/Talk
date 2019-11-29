#include "view.h"
#include "ui_view.h"
#include<buttonlistwidget.h>
#include<QGridLayout>
#include<QSignalMapper>
#include<QPushButton>
#include<QMessageBox>
View::View(QWidget *parent,QString Username) :
    QWidget(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
    username=Username;
    qDebug()<<"View ";
    qDebug()<<username;

    //TCP连接
    tcpSocket=new QTcpSocket(this);
    port=12345;
    serverIp=new QHostAddress();
    ip="10.201.171.110";
    serverIp->setAddress(ip);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    tcpSocket->connectToHost(*serverIp,port);

    //获取数据库成员信息
   QString loginstring ="LIST: "+username;
   qDebug()<<"从数据库获取好友列表";
   int length=tcpSocket->write(loginstring.toLatin1(),loginstring.length());
   if(length<=0)
   {
       qDebug()<<"List Friends Failed\n";
   }


 //  QString buttontext = "btn1,btn2,btn3,btn4,btn5,btn6,btn7,btn8,btn9,btn10";//10个button

 //  QStringList texts = buttontext.split(",");


}
void View::dataReceived()
{
      QByteArray datagram;
      QString msg;
    while(tcpSocket->bytesAvailable()>0)
    {
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        msg=datagram.data();
     }
     qDebug()<<msg;
     list=msg;
     QStringList texts = list.split(";");
     buttonListWidget *buttonList = new buttonListWidget(this);
     buttonList->initWidget(texts);
     ui->scrollArea->setWidget(buttonList);
}

View::~View()
{
    delete ui;
}
