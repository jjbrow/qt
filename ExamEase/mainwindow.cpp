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
    //按钮样式绑定
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
    //表格样式
    ui->tableWidget->setColumnCount(5);
    // 设置表头
    QStringList headers;
    headers << "名称" << "分数" << "创建日期"<<"修改"<<"删除";
    QHeaderView *header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(2, QHeaderView::Stretch);
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //查询所有
    QList<Paper> list=db.getAllPapers();
    int row = 0;
    for (const Paper &p:list) {
        // 创建行和按钮
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(p.name()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(p.total())));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(p.createDate().toString("yyyy-MM-dd hh:mm:ss")));
        QPushButton *editButton = new QPushButton("修改");
        QPushButton *deleteButton = new QPushButton("删除");
        connect(editButton, &QPushButton::clicked, [=]() {
                    // 获取选定行的id
                    int selectedId = p.id();

                    // 执行修改操作（示例中未实现具体的修改逻辑）
                    // 可以在此处弹出修改对话框或者直接更新数据库
                    // 示例：执行更新操作
//                    QSqlQuery updateQuery;
//                    updateQuery.prepare("UPDATE Papers SET name = :name, total = :total WHERE id = :id");
//                    updateQuery.bindValue(":name", "New Name");
//                    updateQuery.bindValue(":total", 100);
//                    updateQuery.bindValue(":id", selectedId);

//                    if (!updateQuery.exec()) {
//                        QMessageBox::critical(this, "Error", "Failed to update record");
//                        return;
//                    }

                    // 更新界面显示（可选）
                   // ui->tableWidget->item(row, 0)->setText("New Name");
                  //  ui->tableWidget->item(row, 1)->setText("100");
                });

        ui->tableWidget->setCellWidget(row, 3, editButton);
        ui->tableWidget->setCellWidget(row, 4, deleteButton);
        ++row;
    }



    //绑定添加按钮
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

