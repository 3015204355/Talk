#include "buttonlistwidget.h"
#include<QHBoxLayout>
#include<QPushButton>
#include<QGridLayout>
#include<QMessageBox>
buttonListWidget::buttonListWidget(QWidget *parent) : QWidget(parent)
{

}

void buttonListWidget::initWidget(QStringList &nameList)
{
    signalMapper = new QSignalMapper(this);
    QGridLayout *layout = new QGridLayout;
    foreach (QString str, nameList)
    {
        QPushButton *button = new QPushButton(str,this);
        button->setMinimumSize(button->size());

        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map ()));//原始信号传递给signalmapper

        signalMapper->setMapping (button,str);

        layout->addWidget(button);
    }
    this->setLayout(layout);
    connect(signalMapper, SIGNAL(mapped (const QString &)), this, SLOT(doClicked(const QString &)));//将转发的信号连接到最终的槽函数
}
void buttonListWidget::doClicked(const QString& btnname)

{

    QMessageBox::information(this, "Clicked", btnname + " is clicked!");//显示被按下的btn名称。

}
