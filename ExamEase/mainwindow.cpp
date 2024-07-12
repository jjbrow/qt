#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPushButton>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //连接数据库
    db.connectDataBase();
    //初始化表格
    initTable();
    //操作按钮绑定
    operateButtons();


}
//表格初始化
void MainWindow::initTable(){
    ui->tableWidget->setRowCount(0); // 清除现有内容
    //设置表格列
    ui->tableWidget->setColumnCount(8);
    // 设置表头
    QStringList headers;
    headers << "名称" << "分数" << "创建日期"<<"模拟考试"<<"新增试题"<<"修改试题"<<"导出"<<"删除";
    QHeaderView *header = ui->tableWidget->horizontalHeader();
    //自动宽度
    header->setSectionResizeMode(2, QHeaderView::Stretch);
   // header->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    // 设置每列的具体宽度（如果需要的话）
    ui->tableWidget->setColumnWidth(1, 50); // 设置“名称”列宽度为100
    ui->tableWidget->setColumnWidth(3, 80); // 设置“名称”列宽度为100
    ui->tableWidget->setColumnWidth(4, 80); // 设置“名称”列宽度为100
    ui->tableWidget->setColumnWidth(5, 80); // 设置“名称”列宽度为100
    ui->tableWidget->setColumnWidth(6, 50);  // 设置“修改”列宽度为50
    ui->tableWidget->setColumnWidth(7, 50);  // 设置“删除”列宽度为50
    //查询所有
    QList<Paper> list=db.getAllPapers();
    int row = 0;
    for (const Paper &p:list) {
        // 创建行和按钮
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(p.name()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(p.total())));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(p.createDate().toString("yyyy-MM-dd hh:mm:ss")));
        //设置列表按钮
        setupButtonsForPaper(ui->tableWidget,row,p,list);
        ++row;
    }
    // 设置第 0 列不可编辑
   makeColumnNonEditable(ui->tableWidget, 1);
   makeColumnNonEditable(ui->tableWidget, 2);

    // 连接 cellChanged 信号 修改第一列
    connect(ui->tableWidget, &QTableWidget::cellChanged, [=](int row, int column) {
        QList<Paper> list = db.getAllPapers();
        if (column == 0) { // 如果是第一列
            QTableWidgetItem *item = ui->tableWidget->item(row, column);
            if (item) {
                Paper paper = list[row];
                paper.setName(item->text());
                if (db.updatePaper(paper)) {
                    qDebug() << "更新成功";
                } else {
                    QMessageBox::critical(ui->tableWidget, "错误", "更新数据库失败");
                }
            }
        }
    });
}
//设置第几列不可编辑
void MainWindow::makeColumnNonEditable(QTableWidget *tableWidget, int column) {
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = tableWidget->item(row, column);
        if (item != nullptr) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }
}
//设置列表按钮
void MainWindow::setupButtonsForPaper(QTableWidget *tableWidget, int row, const Paper &p, const QList<Paper> &list) {
    const QString buttonStyle=R"(
                              QPushButton {
                                      background-color: #c0c0c0;
                                      border: none;
                                      color: white;
                                      padding: 20px 5px;
                                      text-align: center;
                                      text-decoration: none;
                                      font-size: 14px;
                                      font: 10pt "微软雅黑";
                                      margin: 4px 2px;

                                  }
                                  QPushButton:hover {
                                      background-color: white;
                                      color: black;

                                  }
                          )";
    QPushButton *exam = new QPushButton("模拟考试");
    QPushButton *addQuestion = new QPushButton("新增试题");
    QPushButton *editQuestion = new QPushButton("修改试题");
    QPushButton *exportPaper = new QPushButton("导出");
    QPushButton *deleteButton = new QPushButton("删除");
    //添加样式
    exam->setStyleSheet(buttonStyle);
    addQuestion->setStyleSheet(buttonStyle);
    editQuestion->setStyleSheet(buttonStyle);
    exportPaper->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    //添加到表格
    tableWidget->setCellWidget(row, 3, exam);
    tableWidget->setCellWidget(row, 4, addQuestion);
    tableWidget->setCellWidget(row, 5, editQuestion);
    tableWidget->setCellWidget(row, 6, exportPaper);
    tableWidget->setCellWidget(row, 7, deleteButton);
    //模拟考试
    connect(exam, &QPushButton::clicked, [=]() {


    });
    //新增试题
    connect(addQuestion, &QPushButton::clicked, [=]() {
        AddQuestion *aq = new AddQuestion(p.id());
        aq->exec();
    });
    //修改试题
    connect(editQuestion, &QPushButton::clicked, [=]() {


    });
    //导出
    connect(exportPaper, &QPushButton::clicked, [=]() {


    });
    //删除
    connect(deleteButton, &QPushButton::clicked, [=]() {
                // 弹出确认框
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(ui->tableWidget, "确认删除", "确认删除这张试卷？",
                                                  QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    // 获取选定行的id
                    if(db.deletePaper(p.id())){
                        tableWidget->removeRow(row);
                        qDebug()<<"删除成功";
                    }else{
                        QMessageBox::critical(ui->tableWidget, "错误", "删除试卷失败");
                    }
                }
            });


}
//操作按钮绑定
void MainWindow::operateButtons(){
    //按钮样式绑定
    ui->addPaper->setStyleSheet(R"(
                                        QPushButton{
                                font: 10pt "微软雅黑";
                                background-color: rgb(166, 172, 175);
                                border-radius: 5px
                                }
                                        QPushButton:hover {
                                            background-color: #c0c0c0;
                                            border-color: #a0a0a0;
                                        }
                                    )");
    ui->refreshTable->setStyleSheet(R"(
                                        QPushButton{
                                font: 10pt "微软雅黑";
                                background-color: rgb(166, 172, 175);
                                border-radius: 5px
                                }
                                        QPushButton:hover {
                                            background-color: #c0c0c0;
                                            border-color: #a0a0a0;
                                        }
                                    )");


    //绑定添加按钮
    connect(ui->addPaper,&QPushButton::clicked,this,[=]{
        // 新增一行数据
        Paper p;
        p.setTotal(0);
        p.setCreateDate(QDateTime::currentDateTime());
        int id = db.insertPaper(p);
        if (id!=NULL) {
            // 如果成功添加到数据库，则更新表格界面
            int newRow = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(newRow);
            ui->tableWidget->setItem(newRow, 1, new QTableWidgetItem(QString::number(p.total())));
            ui->tableWidget->setItem(newRow, 2, new QTableWidgetItem(p.createDate().toString("yyyy-MM-dd hh:mm:ss")));
            p.setId(id);
            setupButtonsForPaper(ui->tableWidget,newRow,p,db.getAllPapers());
            makeColumnNonEditable(ui->tableWidget, 1);
            makeColumnNonEditable(ui->tableWidget, 2);

        } else {
            qDebug() << "Failed to add paper to database.";
        }
    });
    //绑定刷新按钮
    connect(ui->refreshTable,&QPushButton::clicked,this,[=]{
        //解除表格名称改变信号
        disconnect(ui->tableWidget, &QTableWidget::cellChanged, nullptr, nullptr);
        //初始化表格
        initTable();
    });
}


//析构函数
MainWindow::~MainWindow()
{
    delete ui;
    db.closeDatabase();
}

