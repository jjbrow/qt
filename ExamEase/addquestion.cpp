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
}
//初始化添加试题页面
AddQuestion::AddQuestion(int id) :
    QDialog(nullptr),
    ui(new Ui::AddQuestion)
{
    paper_id = id;
    ui->setupUi(this);

    customizeStyle();


}
//设置样式
void AddQuestion::customizeStyle(){
     setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);  // 移除帮助按钮
    // 设置 QTextEdit 为圆角
    ui->textEdit->setStyleSheet("QTextEdit { border-radius: 2px; border: 1px solid gray; }");
    // 设置所有 QCheckBox 的大小和样式
    QList<QCheckBox *> checkboxes = this->findChildren<QCheckBox *>();
    QList<QLineEdit *> lineEdits =this->findChildren<QLineEdit*>();
    for(QCheckBox *checkbox : checkboxes) {
        checkbox->setStyleSheet("QCheckBox::indicator { width: 20px; height: 20px; }");
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

}

