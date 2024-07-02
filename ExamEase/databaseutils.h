#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include <QObject>
#include <QDebug>
#include "user.h"
class DataBaseUtils : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseUtils(QObject *parent = nullptr);
    ~DataBaseUtils();
    void connectDataBase();
    void closeDatabase();
    //查询用户
    User queryById(const QString &id);
    //更新密码
    bool updateUser(const QString &password);
signals:

private:
    QSqlDatabase db;
};

#endif // DATABASEUTILS_H
