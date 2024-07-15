#include "addquestion.h"
#include "ui_addquestion.h"

AddQuestion::AddQuestion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddQuestion)
{
    ui->setupUi(this);
}

AddQuestion::~AddQuestion()
{
    delete ui;
    db.closeDatabase();
}
//初始化添加试题页面
AddQuestion::AddQuestion(int id) :
    QDialog(nullptr),
    ui(new Ui::AddQuestion)
{
    paper_id = id;
    ui->setupUi(this);
    //连接数据库
    db.connectDataBase();
    //设置样式
    customizeStyle();
    ui->stackedWidget->setCurrentIndex(0);
    //按钮操作绑定
    buttonOperate();

}
//按钮操作绑定
void AddQuestion::buttonOperate(){

    //绑定确定操作
    connect(ui->complete,&QPushButton::clicked,[=]{
        //题目
        QString name = ui->textEdit->toPlainText();
        //解析
        QString analysis = ui->analysis->toPlainText();
        if (name.isEmpty()) {
               QMessageBox::warning(this, "警告", "题目不能为空");
               return;
           }
        // 获取当前选中的值
        QVariant currentValue = ui->type->currentData();
        // 获取答案
        QString answer = ui->answer->text();
        Question q;
        q.setPaperId(paper_id);
        q.setType(currentValue.toInt());
        q.setName(name);
        q.setAnswer(answer);
        q.setAnalysis(analysis);
        q.setCreateDate(QDateTime::currentDateTime());
        if(currentValue==0){
           //单选
           q.setOption1(ui->lineEdit_s1->text());
           q.setOption2(ui->lineEdit_s2->text());
           q.setOption3(ui->lineEdit_s3->text());
           q.setOption4(ui->lineEdit_s4->text());
        }else if(currentValue==1){
            //判断
            q.setOption1("正确");
            q.setOption2("错误");

        }else if(currentValue==2){
            //多选
            q.setOption1(ui->lineEdit_m1->text());
            q.setOption2(ui->lineEdit_m2->text());
            q.setOption3(ui->lineEdit_m3->text());
            q.setOption4(ui->lineEdit_m4->text());
        }
        if(db.insertQuestion(q)){
            QMessageBox msgBox;
            msgBox.setText("添加成功！");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            // 显示消息框并等待用户点击
            if (msgBox.exec() == QMessageBox::Ok) {
                // 清空 QTextEdit 的内容
                clearSelections();
                ui->textEdit->clear();
                ui->analysis->clear();
            }

            qDebug() << "添加题目成功" ;
        }else{
            qDebug() << "添加题目失败" ;
        }

    });


}
//设置样式
void AddQuestion::customizeStyle(){
     setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);  // 移除帮助按钮
    // 设置 QTextEdit 为圆角
    ui->textEdit->setStyleSheet("QTextEdit { border-radius: 2px; border: 1px solid gray; }");
    // 设置所有 QCheckBox 的大小和样式
    QList<QCheckBox *> checkboxes = this->findChildren<QCheckBox *>();
    QList<QLineEdit *> lineEdits =this->findChildren<QLineEdit*>();
    QList<QRadioButton *> radioButtons = this->findChildren<QRadioButton *>();
    // 创建QButtonGroup实例
    buttonGroup = new QButtonGroup(this);

    //设置按钮组  绑定值
    for(QRadioButton *radioButton : radioButtons) {
      //  checkbox->setStyleSheet("QCheckBox::indicator { width: 20px; height: 20px; }");
        buttonGroup->addButton(radioButton);
        // 当用户点击按钮后，可以通过信号槽机制获取值
        connect(radioButton, &QRadioButton::clicked, [=]() {
            QString value = radioButton->property("value").toString();
            ui->answer->setText(value);
            qDebug() << "当前选择的值为：" << value;
        });
    }
    for(QCheckBox *checkbox : checkboxes) {
      //获取所有选中的checkbox的值
        connect(checkbox, &QCheckBox::clicked, [=]() {
            QString value;
            for(QCheckBox *temp : checkboxes) {
                if (temp->isChecked()) {
                         value.append(temp->property("value").toString());
                    }
            }
            ui->answer->setText(value);
            qDebug() << "当前选择的值为：" << value;
        });
    }
    //设置lineEdit 圆角
    for(QLineEdit *lineEdit : lineEdits) {
        lineEdit->setStyleSheet("QLineEdit { border-radius: 3px; border: 1px solid gray; }");
    }
    //设置QToolButton  删除图标
    QIcon icon(":/Image/delete.png");
    for(QToolButton *qToolButton : this->findChildren<QToolButton *>()){
       qToolButton->setIconSize(QSize(20, 20));
       qToolButton->setIcon(icon);
       qToolButton->setStyleSheet("QToolButton { background: transparent; }");
    }
    //设置comboBox值
    ui->type->setItemData(0,0);
    ui->type->setItemData(1,1);
    ui->type->setItemData(2,2);

    // 连接信号到槽函数
    connect(ui->type, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,[=]{
         // 获取当前选中的值
         QVariant currentValue = ui->type->currentData();

         if(currentValue==0){
            //单选
            //清除选中状态
            clearSelections();
            ui->stackedWidget->setCurrentIndex(0);

         }else if(currentValue==1){
             //判断
             //清除选中状态
             clearSelections();
             ui->stackedWidget->setCurrentIndex(2);

         }else if(currentValue==2){
             //多选
            //清除选中状态
            clearSelections();
            ui->stackedWidget->setCurrentIndex(1);
         }

    });
    ui->radioButton_1->setProperty("value", "A");
    ui->radioButton_2->setProperty("value", "B");
    ui->radioButton_3->setProperty("value", "C");
    ui->radioButton_4->setProperty("value", "D");
    ui->radioButton_5->setProperty("value", "A");
    ui->radioButton_6->setProperty("value", "B");
    ui->checkBox_1->setProperty("value", "A");
    ui->checkBox_2->setProperty("value", "B");
    ui->checkBox_3->setProperty("value", "C");
    ui->checkBox_4->setProperty("value", "D");
    QStyledItemDelegate *delegate = new QStyledItemDelegate(this);
    ui->type->setItemDelegate(delegate);
    ui->type->setStyleSheet("QComboBox {"
                                      "    border: 1px solid gray;"
                                      "    border-radius: 3px;"
                                      "    padding: 5px;"  // 增加选项之间的间距
                                      "    min-width: 6em;"
                                      "    min-height: 20px;"
                                      "    background: white;"
                                      "}"
                                      "QComboBox::drop-down {"
                                      "    subcontrol-origin: padding;"
                                      "    subcontrol-position: top right;"
                                      "    width: 20px;"
                                      "    height: 30px;"
                                      "    border-left-width: 1px;"
                                      "    border-left-color: darkgray;"
                                      "    border-left-style: solid;"
                                      "    border-top-right-radius: 3px;"
                                      "    border-bottom-right-radius: 3px;"

                                      "}"

                                      "QComboBox QAbstractItemView {"
                                      "    border: 2px solid darkgray;"
                                      "    padding: 2px;"
                                      "    background-color: white;"
                                      "}"
                                      "QComboBox QAbstractItemView::item {"
                                      "    height: 30px;"
                                      "    padding-left: 10px;"
                                      "    padding-right: 10px;"
                                      "}"
                                      "QComboBox QAbstractItemView::item:selected {"
                                      "    background-color: lightblue;"
                                      "}");
}
//清除选中状态
void AddQuestion::clearSelections(){
    // 获取 buttonGroup 中的所有按钮
    QList<QCheckBox *> checkboxes = this->findChildren<QCheckBox *>();
    buttonGroup->setExclusive(false);
    QList<QAbstractButton*> buttons = buttonGroup->buttons();
    foreach(QAbstractButton* button, buttons) {
        button->setChecked(false);
    }
    for(QCheckBox *checkbox : checkboxes) {
      checkbox->setChecked(false);
    }
    buttonGroup->setExclusive(true);
    ui->answer->setText("");
}

