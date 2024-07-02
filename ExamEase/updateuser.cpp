#include "updateuser.h"
#include "ui_updateuser.h"
#include<QMessageBox>
updateUser::updateUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::updateUser)
{
    ui->setupUi(this);


    connect(ui->confirm,&QPushButton::clicked,this,[=]{
        DataBaseUtils dbUtils;
        dbUtils.connectDataBase();
        User u = dbUtils.queryById("1");

        QString oldPasswor = ui->oldPassword->text();
        QString newPassword = ui->newPassword->text();
        QString confirmPassword = ui->confirmPassword->text();
        if(oldPasswor==NULL||newPassword==NULL||confirmPassword==NULL){
            QMessageBox::critical(nullptr, "Error", "密码不能为空");
            return;
        }
        if(u.getPassword().compare(oldPasswor,Qt::CaseSensitive)!=0){
            QMessageBox::critical(nullptr, "Error", "旧密码输入错误");
            return;
        }

        if(newPassword.compare(confirmPassword,Qt::CaseSensitive)!=0){
            QMessageBox::critical(nullptr, "Error", "新密码和确认密码不一致");
            return;
        }
        //更新数据库
        if(!dbUtils.updateUser(newPassword)){
            QMessageBox::critical(nullptr, "Error", "密码更新出错");
            return;
        }
        QMessageBox msgBox;
        msgBox.setWindowTitle("success");
        msgBox.setText("修改成功");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.resize(50,50);
        // 显示信息提示框，并等待用户确认
        int ret = msgBox.exec();
        // 当确认按钮被点击时
        if (ret == QMessageBox::Ok) {
             this->close();
        }
        dbUtils.closeDatabase();
    });
    connect(ui->cancel,&QPushButton::clicked,this,[=]{
        this->close();
    });

}

updateUser::~updateUser()
{
    delete ui;
}
