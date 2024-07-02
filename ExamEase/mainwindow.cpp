#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    db.connectDataBase();
    ui->setupUi(this);



    ui->addPaper->setStyleSheet(R"(
                                        QPushButton{
                                font: 10pt "微软雅黑";
                                background-color: rgb(166, 172, 175);
                                border-radius: 10px
                                }
                                        QPushButton:hover {
                                            background-color: #c0c0c0;
                                            border-color: #a0a0a0;
                                        }
                                    )");
    connect(ui->addPaper,&QPushButton::clicked,this,[=]{


        QList<Paper> list=db.getAllPapers();
        for (const Paper &p:list) {
            qDebug()<<p.name()<<"=="<<p.createDate();
        }


    });


}

MainWindow::~MainWindow()
{
    delete ui;
    db.closeDatabase();
}

