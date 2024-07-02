#ifndef LOGIN_H
#define LOGIN_H
#include "mainwindow.h"
#include <QWidget>
#include<updateuser.h>
#include"databaseutils.h"
namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
    void checkedData();
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::login *ui;
    MainWindow w;
    updateUser *uu;
};

#endif // LOGIN_H
