#include "register.h"
#include "ui_register.h"
#include "client.h"
#include "ui_client.h"

#include<QMessageBox>
#include<QDebug>
std::string username;
std::string password;
std::string phone;
//是不是需要大写开头？
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_lineEdit1_editingFinished()
{
  username=ui->lineEdit1->text().toStdString();
}

void Register::on_lineEdit2_editingFinished()
{
    password=ui->lineEdit2->text().toStdString();
}

void Register::on_lineEdit3_editingFinished()
{
    phone=ui->lineEdit3->text().toStdString();
}

void Register::on_pushButton_clicked()
{
    qDebug()<<QString::fromStdString(username);
    qDebug()<<QString::fromStdString(password);
    qDebug()<<QString::fromStdString(phone);
    int ret2 = QMessageBox::information(this, tr("提示"), tr("注册成功！"), QMessageBox::Ok);
}

void Register::on_pushButton_2_clicked()
{
    close();
    client *cl=new client();
    cl->show();
    //delete cl;
}
