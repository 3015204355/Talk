#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include<QHostAddress>
#include<QTcpSocket>
namespace Ui {
class View;
}

class View : public QWidget
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0,QString Username=NULL);
    ~View();

private slots:
    void dataReceived();
private:
    Ui::View *ui;
    QString ip;
    QHostAddress *serverIp;
    QTcpSocket *tcpSocket;
    QString username;
    QString list;
    int port;
};

#endif // VIEW_H
