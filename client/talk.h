#ifndef TALK_H
#define TALK_H

#include <QWidget>
#include<QTcpSocket>
#include<QHostAddress>

namespace Ui {
class Talk;
}

class Talk : public QWidget
{
    Q_OBJECT

public:
    explicit Talk(QWidget *parent = 0,QString Username=NULL);
    ~Talk();

private slots:
    void on_pushButton_clicked();
    void dataReceived();

private:
    Ui::Talk *ui;
    QString ip;
    QHostAddress *serverIp;
    QTcpSocket *tcpSocket;
    QString username;
    int port;
};

#endif // TALK_H
