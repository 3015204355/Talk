#include "talk.h"
#include "ui_talk.h"
#include<QByteArray>
#include<sys/types.h>

Talk::Talk(QWidget *parent,QString Username) :
    QWidget(parent),
    ui(new Ui::Talk)
{
    username=Username;
    qDebug()<<username;
    ui->setupUi(this);
    tcpSocket=new QTcpSocket(this);
    port=12345;
    serverIp=new QHostAddress();
    ip="10.201.117.110";
    serverIp->setAddress(ip);
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    tcpSocket->connectToHost(*serverIp,port);
}

Talk::~Talk()
{
    delete ui;
}
void Talk::dataReceived()
{
    while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        QString msg=datagram.data();
        ui->textBrowser->setText(msg);
        //qDebug()<<msg;
     }

}

void Talk::on_pushButton_clicked()
{
    /*QTcpSocket* tcpSocket=new QTcpSocket(this);
    int port=12345;
    QHostAddress* serverIp=new QHostAddress();
    QString ip="10.201.186.187";
    serverIp->setAddress(ip);
    tcpSocket->connectToHost(*serverIp,port);*/
    QString textstring =  username+":"+ui->textEdit->toPlainText();
    int length=tcpSocket->write(textstring.toLatin1(),textstring.length());
     ui->textBrowser->setText(textstring);
   // QString textPlianString =  ui->textEdit->toPlainText();

  /*  while(tcpSocket->bytesAvailable()>0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        QString msg=datagram.data();
        ui->textBrowser->setText(msg);
        qDebug()<<msg;
     }



    ui->textEdit->toHtml()=NULL;*/
}
