#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPushButton>
#include <QDebug>
#include <QDBusConnection>
#include<QDBusError>
#include<readinglogs.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readingLogs = new ReadingLogs(this);
    label = new QLabel(this);
    //label
    ui->stateLabel->setFixedSize(20,20);
    ui->stateLabel->setStyleSheet("background-color:green;border-radius:10px");
    QPoint pos = ui->stateLabel->pos();
    ui->stateLabel->move(pos.x()+20,pos.y());
    //启动服务
    startDBusService();
     // 连接按钮的点击事件到停止服务的槽函数
    connect(ui->cease,&QPushButton::clicked,this,&MainWindow::updateMessageFlag);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::startDBusService(){
    readingLogs->messageFlag = false;
    QDBusConnection connection = QDBusConnection::sessionBus();
    //注册服务
        QString msg = "状态: ";
       if (connection.registerService("com.example.MyService")) {
           if (connection.registerObject("/com/example/MyObject", readingLogs, QDBusConnection::ExportAllSlots)) {
               //连接readingLogs的信号
                connect(readingLogs,&ReadingLogs::messageReceived,this,&MainWindow::updateTextBrower);
                readingLogs->messageFlag = true;
                msg.append("服务启动成功");
           }else{
               msg.append("服务启动失败 ").append(connection.lastError().message());
               ui->cease->setText("读取");
           }
       }else{
           msg.append("服务启动失败 ").append(connection.lastError().message());
           ui->cease->setText("读取");
       }
       //提示信息
      label->setText(msg);
      ui->statusbar->addWidget(label);
}
void MainWindow::updateTextBrower(const QString &message){
    //正常启动 直接显示到页面
    if(readingLogs->messageFlag){
        ui->textBrowser->append(message); 
    }
}
 void MainWindow::updateMessageFlag(){
     //修改按钮文字
     if(readingLogs->messageFlag){
         ui->cease->setText("读取");
         ui->stateLabel->setStyleSheet("background-color:red;border-radius:10px");
     }else{
         //先从缓存中读取
         QQueue<QString> logs=readingLogs->messageCache;
         if(!logs.isEmpty()){
             //迭代器
             QQueue<QString>::const_iterator iter;
             for(iter=logs.constBegin();iter!=logs.constEnd();++iter){
                  ui->textBrowser->append(*iter);
             }

             readingLogs->messageCache.clear();
         }
         ui->cease->setText("停止");
         ui->stateLabel->setStyleSheet("background-color:green;border-radius:10px");

     }
     //修改读取标记
     readingLogs->messageFlag = !readingLogs->messageFlag;

 }

