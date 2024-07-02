#ifndef USER_H
#define USER_H
#include <QString>


class User
{

public:

    User()=default;
    void setId(QString idx);
    void setName(QString namex);
    void setPassword(QString passwordx);
    QString getId();
    QString getName();
    QString getPassword();
private:
    QString id;
    QString name;
    QString password;


};

#endif // USER_H
