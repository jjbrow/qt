#ifndef READINGLOGS_H
#define READINGLOGS_H
#include<QString>
#include <QObject>
#include<QQueue>
#define messageNum 2000
class ReadingLogs : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.Interface.ReadingLogs")

public:
    explicit ReadingLogs(QObject *parent=nullptr);
    //是否停止标记;
    bool messageFlag;
    //缓存数据队列
    QQueue<QString> messageCache;
signals:
    void messageReceived(const QString &message);

public slots:

    void reading(const QString &message);

};



#endif // READINGLOGS_H
