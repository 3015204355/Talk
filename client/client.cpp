#include "client.h"
#include "ui_client.h"
#include "register.h"
#include "ui_register.h"
#include "login.h"
#include "ui_login.h"
#include<QInputDialog>
#include<QDebug>

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
}

client::~client()
{
    delete ui;
}

void client::on_pushButton_clicked()
{
    close();
    Register *rg=new Register();
    rg->show();
    //delete rg;
}

void client::on_pushButton_2_clicked()
{
    close();
    Login *lg=new Login();
    lg->show();
}
