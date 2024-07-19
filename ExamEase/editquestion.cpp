#include "editquestion.h"
#include "ui_editquestion.h"

EditQuestion::EditQuestion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditQuestion)
{
    ui->setupUi(this);
}
EditQuestion::EditQuestion(int id): QDialog(nullptr),
    ui(new Ui::EditQuestion){
    ui->setupUi(this);
    paper_id=id;
    this->setWindowIcon(QIcon(":/Image/search.svg"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);  // 移除帮助按钮
    //连接数据库
    db.connectDataBase();
    //初始化表格
    initTable();
}
//表格初始化
void EditQuestion::initTable(){
    ui->tableWidget->setRowCount(0); // 清除现有内容
    //设置表格列
    ui->tableWidget->setColumnCount(5);
    // 设置表头
    QStringList headers;
    headers << "题目" << "类型"<< "答案" << "修改"<<"删除";
    QHeaderView *header = ui->tableWidget->horizontalHeader();
    //自动宽度
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    // 设置每列的具体宽度（如果需要的话）
    ui->tableWidget->setColumnWidth(0, 50); // 设置“名称”列宽度为100
    //查询所有
    QList<Question> list=db.getAllQuestionsByPaper(paper_id);
    int row = 0;
    for (const Question &p:list) {
        // 创建行和按钮
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(p.name()));
        QString typeStr;
        if(p.type()==0){
            typeStr.append("单选");
        }else if(p.type()==1){
            typeStr.append("判断");
        }else if(p.type()==2){
            typeStr.append("多选");
        }
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(typeStr));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(p.answer()));
        //设置列表按钮
        setupButtonsForPaper(ui->tableWidget,row,p,list);
        ++row;
    }
    // 设置第 0 列不可编辑
   makeColumnNonEditable(ui->tableWidget, 0);
   makeColumnNonEditable(ui->tableWidget, 1);
   makeColumnNonEditable(ui->tableWidget, 2);
}
//设置第几列不可编辑
void EditQuestion::makeColumnNonEditable(QTableWidget *tableWidget, int column) {
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QTableWidgetItem *item = tableWidget->item(row, column);
        if (item != nullptr) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }
}
//设置列表按钮
void EditQuestion::setupButtonsForPaper(QTableWidget *tableWidget, int row, const Question &p, const QList<Question> &list) {
    const QString buttonStyle=R"(
                              QPushButton {
                                      background-color: #c0c0c0;
                                      border: none;
                                      color: white;
                                      padding: 30px 5px;
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

    QPushButton *editQuestion = new QPushButton("修改试题");
    QPushButton *deleteButton = new QPushButton("删除");
    //添加样式
    editQuestion->setStyleSheet(buttonStyle);
    deleteButton->setStyleSheet(buttonStyle);
    //添加到表格
    tableWidget->setCellWidget(row, 3, editQuestion);
    tableWidget->setCellWidget(row, 4, deleteButton);
    QString pName = p.name();
    //修改试题
    connect(editQuestion, &QPushButton::clicked, [=]() {
        //查询试卷
        Question q =db.getQuestion(p.id());
        AddQuestion *aq = new AddQuestion(q);
        // 连接 添加题目 的 dialogClosed 信号到槽函数
        connect(aq, &AddQuestion::dialogClosed, this, &EditQuestion::refreshTable);
        aq->exec();
    });
    //删除
    connect(deleteButton, &QPushButton::clicked, [=]() {

                // 弹出确认框
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(ui->tableWidget, "确认删除", "确认删除这道题目？",
                                                  QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    // 获取选定行的id
                    if(db.deleteQuestion(p.id())){
                        tableWidget->removeRow(row);
                        qDebug()<<"删除成功";
                        refreshTable();
                    }else{
                        QMessageBox::critical(ui->tableWidget, "错误", "删除题目失败");
                    }
                }
            });


}
//刷新表格
void EditQuestion::refreshTable(){
    //解除表格名称改变信号
    disconnect(ui->tableWidget, &QTableWidget::cellChanged, nullptr, nullptr);
    //初始化表格
    initTable();
}
EditQuestion::~EditQuestion()
{
    delete ui;
}
void EditQuestion::closeEvent(QCloseEvent *event) {
    emit dialogClosed(); // 发射信号
    QDialog::closeEvent(event); // 调用基类的关闭事件处理
}

