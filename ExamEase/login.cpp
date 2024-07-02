#include "login.h"
#include "ui_login.h"
#include<QDebug>
#include<QKeyEvent>
#include<QMessageBox>
login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    connect(ui->loginButton,&QPushButton::clicked,this,&login::checkedData);
    connect(ui->cancelButton,&QPushButton::clicked,this,[=]{
        this->close();
    });

    //修改密码
    uu = new updateUser();
    connect(ui->changePassword,&QPushButton::clicked,this,[=]{
        uu->setWindowModality(Qt::ApplicationModal);
        uu->show();
    });


}
void login::checkedData(){
    QString userName = ui->userName->text();
    QString password =  ui->password->text();
    if(userName==NULL||password==NULL){
        QMessageBox::critical(nullptr, "Error", "用户名或密码不能为空");
        return;
    }
    if(userName.compare("admin",Qt::CaseSensitive)!=0){
        QMessageBox::critical(nullptr, "Error", "用户名错误");
        return;
    }
    //database
    DataBaseUtils dbUtils;
    dbUtils.connectDataBase();
    User u = dbUtils.queryById("1");
    if(password.compare(u.getPassword(),Qt::CaseSensitive)!=0){
        QMessageBox::critical(nullptr, "Error", "密码错误");
        return;
    }
    dbUtils.closeDatabase();
    //关闭登录页面
    this->close();
    //打开日志页面
    w.show();
}
//回车键事件
void login::keyPressEvent(QKeyEvent *event)  {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            checkedData();
        } else {
            QWidget::keyPressEvent(event);
        }
    }


login::~login()
{
    delete ui;
}
