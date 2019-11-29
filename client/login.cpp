#include "login.h"
#include "ui_login.h"
/*这个头文件可能需要删除*/
#include "talk.h"
#include "ui_talk.h"


#include "view.h"
#include "ui_view.h"
#include<QMessageBox>
#include<QDebug>
std::string loginusername;
std::string loginpassword;

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    tcpSocket=new QTcpSocket(this);
    port=12345;
    serverIp=new QHostAddress();
    ip="10.201.171.110";
    serverIp->setAddress(ip);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    tcpSocket->connectToHost(*serverIp,port);
}

Login::~Login()
{
    delete ui;
    //tcpSocket->close();
}
void Login::dataReceived()
{
      QByteArray datagram;
      QString msg;
    while(tcpSocket->bytesAvailable()>0)
    {

        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        msg=datagram.data();

     }
     //qDebug()<<msg;
     if(msg=="Success!")
     {
         qDebug()<<msg;
         int ret2 = QMessageBox::information(this, tr("提示"), tr("登录成功！"), QMessageBox::Ok);
         if(ret2>0)
         {
             close();
             /*Talk *tk=new Talk(NULL,QString::fromStdString(loginusername));
             tk->show();*/

             View *vw=new View(NULL,QString::fromStdString(loginusername));
             vw->show();
         }

      }
     if(msg=="Failed!")
     {
         qDebug()<<msg;
         int ret2 = QMessageBox::information(this, tr("提示"), tr("Password Don't Match！"), QMessageBox::Ok);
         if(ret2>0)
         {
           close();
         }
     }

}
void Login::on_lineEdit1_editingFinished()
{
     loginusername=ui->lineEdit1->text().toStdString();
}

void Login::on_lineEdit2_editingFinished()
{
     loginpassword=ui->lineEdit2->text().toStdString();
}

void Login::on_pushButton_clicked()
{
    qDebug()<<QString::fromStdString(loginusername);
    qDebug()<<QString::fromStdString(loginpassword);
  //  qDebug()<<QString::fromStdString(phone);
    QString loginstring ="LOGIN: "+QString::fromStdString(loginusername)+" "+QString::fromStdString(loginpassword);
    int length=tcpSocket->write(loginstring.toLatin1(),loginstring.length());
    if(length<=0)
    {
        qDebug()<<"Login Failed\n";
    }

}
