#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/Image/system.svg"));
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
    ui->tableWidget->setColumnCount(9);
    // 设置表头
    QStringList headers;
    headers << "名称" << "分数" << "题目数"<< "创建日期"<<"模拟考试"<<"新增试题"<<"修改试题"<<"导出"<<"删除";
    QHeaderView *header = ui->tableWidget->horizontalHeader();
    //自动宽度
    header->setSectionResizeMode(3, QHeaderView::Stretch);
    header->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    // 设置每列的具体宽度（如果需要的话）
    ui->tableWidget->setColumnWidth(1, 50); // 设置“名称”列宽度为100
    ui->tableWidget->setColumnWidth(2, 50);
    ui->tableWidget->setColumnWidth(3, 80); // 设置“名称”列宽度为100
    ui->tableWidget->setColumnWidth(4, 80); // 设置“名称”列宽度为100
    ui->tableWidget->setColumnWidth(5, 80); // 设置“名称”列宽度为100
    ui->tableWidget->setColumnWidth(6, 80);  // 设置“修改”列宽度为50
    ui->tableWidget->setColumnWidth(7, 50);  // 设置“删除”列宽度为50
    ui->tableWidget->setColumnWidth(8, 50);
    //查询所有
    QList<Paper> list=db.getAllPapers();
    int row = 0;
    for (const Paper &p:list) {
        // 创建行和按钮
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(p.name()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(p.total())));
        QList<Question> tempSize = db.getAllQuestionsByPaper(p.id());
        if(tempSize.isEmpty()){
           ui->tableWidget->setItem(row, 2, new QTableWidgetItem("0"));
        }else{
           ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(tempSize.size())));
        }
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(p.createDate().toString("yyyy-MM-dd hh:mm:ss")));
        //设置列表按钮
        setupButtonsForPaper(ui->tableWidget,row,p,list);
        ++row;
    }
    // 设置第 0 列不可编辑
   makeColumnNonEditable(ui->tableWidget, 1);
   makeColumnNonEditable(ui->tableWidget, 2);
   makeColumnNonEditable(ui->tableWidget, 3);

    // 连接 cellChanged 信号 修改第一列
    connect(ui->tableWidget, &QTableWidget::cellChanged, [=](int row, int column) {
        QList<Paper> list = db.getAllPapers();
        if (column == 0) { // 如果是第一列
            QTableWidgetItem *item = ui->tableWidget->item(row, column);
            if (item) {
                Paper paper = list[row];
                paper.setName(item->text());
                if (db.updatePaper(paper)) {
                    refreshTable();
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
    tableWidget->setCellWidget(row, 4, exam);
    tableWidget->setCellWidget(row, 5, addQuestion);
    tableWidget->setCellWidget(row, 6, editQuestion);
    tableWidget->setCellWidget(row, 7, exportPaper);
    tableWidget->setCellWidget(row, 8, deleteButton);
    QString pName = p.name();
    //模拟考试
    connect(exam, &QPushButton::clicked, [=]() {
        if(pName.isNull()){
            QMessageBox::critical(ui->tableWidget, "错误", "试卷名称不能为空");
            return;
        }
        QList<Question> list =db.getAllQuestionsByPaper(p.id());
        if(list.isEmpty()){
            QMessageBox::critical(ui->tableWidget, "错误", "试卷中题目为空");
            return;
        }
        SimulatedExam *s = new SimulatedExam(list);
        connect(s, &SimulatedExam::dialogClosed, this, &MainWindow::refreshTable);
        s->setPaperId(p.id());
        s->exec();


    });
    //新增试题
    connect(addQuestion, &QPushButton::clicked, [=]() {
        if(pName.isNull()){
            QMessageBox::critical(ui->tableWidget, "错误", "试卷名称不能为空");
            return;
        }
        AddQuestion *aq = new AddQuestion(p.id());
        // 连接 添加题目 的 dialogClosed 信号到槽函数
        connect(aq, &AddQuestion::dialogClosed, this, &MainWindow::refreshTable);
        aq->exec();
    });
    //修改试题
    connect(editQuestion, &QPushButton::clicked, [=]() {
        if(pName.isNull()){
            QMessageBox::critical(ui->tableWidget, "错误", "试卷名称不能为空");
            return;
        }
        EditQuestion *ed = new EditQuestion(p.id());
        // 连接 修改题目 的 dialogClosed 信号到槽函数
        connect(ed, &EditQuestion::dialogClosed, this, &MainWindow::refreshTable);
        ed->exec();

    });
    //导出
    connect(exportPaper, &QPushButton::clicked, [=]() {
        if(pName.isNull()){
            QMessageBox::critical(ui->tableWidget, "错误", "试卷名称不能为空");
            return;
        }
        exportForWord(p.id());

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
                        refreshTable();
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
        //判断上一行的name是否为空
        int newRow = ui->tableWidget->rowCount();
        // 获取最后一行的行号
        int lastRow = newRow - 1;
        // 检查表格是否为空
        if (lastRow > 0) {
            // 获取最后一行第二列的项
            QTableWidgetItem *item = ui->tableWidget->item(lastRow, 0);
            // 检查项是否为空
            if (!item || item->text().isEmpty()) {
                QMessageBox::critical(ui->tableWidget, "错误", "试卷名称不能为空");
                return;
            }
        }

        // 新增一行数据
        Paper p;
        p.setTotal(0);
        p.setCreateDate(QDateTime::currentDateTime());
        int id = db.insertPaper(p);
        if (id!=NULL) {
            // 如果成功添加到数据库，则更新表格界面
            ui->tableWidget->insertRow(newRow);
            ui->tableWidget->setItem(newRow, 1, new QTableWidgetItem(QString::number(p.total())));
            ui->tableWidget->setItem(newRow, 2, new QTableWidgetItem("0"));
            ui->tableWidget->setItem(newRow, 3, new QTableWidgetItem(p.createDate().toString("yyyy-MM-dd hh:mm:ss")));
            p.setId(id);
            setupButtonsForPaper(ui->tableWidget,newRow,p,db.getAllPapers());
            makeColumnNonEditable(ui->tableWidget, 1);
            makeColumnNonEditable(ui->tableWidget, 2);
            makeColumnNonEditable(ui->tableWidget, 3);

        } else {
            qDebug() << "Failed to add paper to database.";
        }
    });
    //绑定刷新按钮
    connect(ui->refreshTable,&QPushButton::clicked,this,[=]{
        refreshTable();
    });
}
//将试卷导出到word
void MainWindow::exportForWord(int id){
        //鼠标开启等待
        QApplication::setOverrideCursor(Qt::WaitCursor);
        // 创建 Word 应用程序对象
        wordApp = new QAxObject("Word.Application");
        if (!wordApp) {
            qDebug() << "无法实例化 Word.Application";
            QMessageBox::critical(ui->tableWidget, "错误", "无法实例化 Word.Application");
            return; // 退出或处理错误
        }
        wordApp->dynamicCall("SetVisible(bool)", false); // 不显示 Word 应用程序
        // 创建一个新的文档
        QAxObject* documents = wordApp->querySubObject("Documents");
        if (!documents) {
            qDebug() << "Documents 对象未找到";
            QMessageBox::critical(ui->tableWidget, "错误", "Documents 对象未找到");
            return;
        }
        QAxObject* document = documents->querySubObject("Add()");
        if (!document) {
            qDebug() << "文档未成功创建";
            QMessageBox::critical(ui->tableWidget, "错误", "文档未成功创建");
            return;
        }
        //查询试卷标题
        Paper p = db.getPaper(id);

        // 获取文档的内容
        QAxObject* content = document->querySubObject("Content");

        // 插入标题
        QAxObject* paragraph = content->querySubObject("Paragraphs")->querySubObject("Add()");
        QAxObject* range = paragraph->querySubObject("Range");
        range->dynamicCall("Text", p.name());
        QAxObject* font = range->querySubObject("Font");
        font->setProperty("Bold", true);
        font->setProperty("Size", 24);

        // 将标题段落居中
        paragraph->setProperty("Alignment", 1); // 1 代表 wdAlignParagraphCenter
        //姓名
        paragraph = content->querySubObject("Paragraphs")->querySubObject("Add()");
        range = paragraph->querySubObject("Range");

        QString s = "(本试卷卷面总分100分, 及格分70分, 考试时间100分钟)\n";
        s.append(" 姓名:              分数:     ");
        range->dynamicCall("Text", s);
        paragraph->setProperty("Alignment", 1); // 0 代表 wdAlignParagraphLeft
        font = range->querySubObject("Font");
        font->setProperty("Bold", false);
        font->setProperty("Size", fontSize);
        //查询各类型试题
         QList<Question> allList = db.getAllQuestionsByPaper(id);
         //单选
         QList<Question> list;
         //判断
         QList<Question> judge;
         //多选
         QList<Question> choice;
         for (Question &q :allList) {
             if(q.type()==0){
                 //单选
                 list.append(q);
             }else if(q.type()==1){
                 //判断
                 judge.append(q);
             }else if(q.type()==2){
                 //多选
                 choice.append(q);
             }
         }
         int no = 1;
         if(!list.isEmpty()){
             // 插入单选择题
             paragraph = content->querySubObject("Paragraphs")->querySubObject("Add()");
             range = paragraph->querySubObject("Range");
             range->dynamicCall("Text", "\n第一部分: 选择题 每题1分 请选择一个最合适的答案");
             paragraph->setProperty("Alignment", 0); // 0 代表 wdAlignParagraphLeft
             font = range->querySubObject("Font");
             font->setProperty("Bold", false);
             font->setProperty("Size", fontSize);
             // 插入选择题
             for (int i = 0; i < list.size(); ++i) {
                 paragraph = content->querySubObject("Paragraphs")->querySubObject("Add()");
                 range = paragraph->querySubObject("Range");
                 QString question = QString::number(no++);
                 question.append(".");
                 question.append(list[i].name());
                 question.append("( )\n");
                 question.append("A.").append(list[i].option1()).append("\n");
                 question.append("B.").append(list[i].option2()).append("\n");
                 if(!list[i].option3().isEmpty()){
                     question.append("C.").append(list[i].option3()).append("\n");
                 }
                 if(!list[i].option4().isEmpty()){
                     question.append("D.").append(list[i].option4());
                 }
                 range->dynamicCall("Text", question);
                 paragraph->setProperty("Alignment", 0); // 0 代表 wdAlignParagraphLeft
               }
         }
         if(!judge.isEmpty()){
             // 插入判断
             paragraph = content->querySubObject("Paragraphs")->querySubObject("Add()");
             range = paragraph->querySubObject("Range");
             range->dynamicCall("Text", "\n第二部分: 判断题 每题1分 请选择一个最合适的答案");
             paragraph->setProperty("Alignment", 0); // 0 代表 wdAlignParagraphLeft
             font = range->querySubObject("Font");
             font->setProperty("Bold", false);
             font->setProperty("Size", fontSize);
             // 插入判断题
             for (int i = 0; i < judge.size(); ++i) {
                 paragraph = content->querySubObject("Paragraphs")->querySubObject("Add()");
                 range = paragraph->querySubObject("Range");
                 QString question = QString::number(no++);
                 question.append(".");
                 question.append(judge[i].name());
                 question.append("( )");
                 range->dynamicCall("Text", question);
                 paragraph->setProperty("Alignment", 0); // 0 代表 wdAlignParagraphLeft
               }
         }
         if(!choice.isEmpty()){
             // 插入多选择题
             paragraph = content->querySubObject("Paragraphs")->querySubObject("Add()");
             range = paragraph->querySubObject("Range");
             range->dynamicCall("Text", "\n第三部分: 多选题 每题2分 请选择一个最合适的答案");
             paragraph->setProperty("Alignment", 0); // 0 代表 wdAlignParagraphLeft
             font = range->querySubObject("Font");
             font->setProperty("Bold", false);
             font->setProperty("Size", fontSize);
             // 插入多选择题
             for (int i = 0; i < choice.size(); ++i) {
                 paragraph = content->querySubObject("Paragraphs")->querySubObject("Add()");
                 range = paragraph->querySubObject("Range");
                 QString question = QString::number(no++);
                 question.append(".");
                 question.append(choice[i].name());
                 question.append("( )\n");
                 question.append("A.").append(choice[i].option1()).append("\n");
                 question.append("B.").append(choice[i].option2()).append("\n");
                 if(!list[i].option3().isEmpty()){
                     question.append("C.").append(choice[i].option3()).append("\n");
                 }
                 if(!list[i].option4().isEmpty()){
                     question.append("D.").append(choice[i].option4());
                 }
                 range->dynamicCall("Text", question);
                 paragraph->setProperty("Alignment", 0); // 0 代表 wdAlignParagraphLeft
               }
         }
    // 保存文档
    QString filePath  = "D:/Code/";
    filePath.append(p.name()).append(DateUtils::getCurrentDateTimeString()).append(".docx");
    document->dynamicCall("SaveAs(const QString&)", filePath);
    // 关闭文档和 Word 应用程序
    document->dynamicCall("Close()");
    wordApp->dynamicCall("Quit()");
    delete wordApp;
    qDebug() << "文档成功创建";
    //鼠标结束等待
    QApplication::restoreOverrideCursor();
    QMessageBox msgBox;
    msgBox.setText("文件成功导出到路径:  "+filePath);
    msgBox.exec();
}
//刷新表格
void MainWindow::refreshTable(){
    //解除表格名称改变信号
    disconnect(ui->tableWidget, &QTableWidget::cellChanged, nullptr, nullptr);
    //初始化表格
    initTable();
}

//析构函数
MainWindow::~MainWindow()
{
    delete ui;
    db.closeDatabase();
}

