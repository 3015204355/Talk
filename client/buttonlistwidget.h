#include <QWidget>
#include<QSignalMapper>
class buttonListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit buttonListWidget(QWidget *parent = 0);
    void initWidget(QStringList& nameList);
private:
     QSignalMapper *signalMapper;
signals:

public slots:
       void doClicked(const QString & btnname);//处理最终信号的槽
};
