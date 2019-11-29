#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QTcpSocket>
#include<QHostAddress>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_lineEdit1_editingFinished();

    void on_lineEdit2_editingFinished();

    void on_pushButton_clicked();
    void dataReceived();

private:
    Ui::Login *ui;
    QString ip;
    QHostAddress *serverIp;
    QTcpSocket *tcpSocket;
    QString username;
    int port;
};

#endif // LOGIN_H
