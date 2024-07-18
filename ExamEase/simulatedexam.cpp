#include "simulatedexam.h"
#include "ui_simulatedexam.h"

SimulatedExam::SimulatedExam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulatedExam)
{
    ui->setupUi(this);
}

SimulatedExam::SimulatedExam(QList<Question> l):
    QDialog(nullptr),
    ui(new Ui::SimulatedExam)
{
    ui->setupUi(this);
    list = l;
    index = 0;
    //显示第一页
    viewQuestion(index);
    //绑定按钮
    buttonOperate();

}
//绑定按钮
void SimulatedExam::buttonOperate(){
    //下一题
    connect(ui->next,&QPushButton::clicked,[=]{
        Question q = list[index];
        index++;
        if(index>=list.size()){
            index= list.size()-1;
        }
        viewQuestion(index);

        //选中答案
        QString answer;
        //判断题目类型
        if(q.type()==2){
            QList<QCheckBox *> checkboxes = this->findChildren<QCheckBox *>();
            for(QCheckBox *ch:checkboxes){
                if(ch->isChecked()){
                    answer.append(ch->property("value").toString());
                }
            }
        }else{
            QList<QRadioButton *> radioButtons = this->findChildren<QRadioButton *>();
            for(QRadioButton *radioButton : radioButtons) {
                if(radioButton->isChecked()){
                    answer.append(radioButton->property("value").toString());
                }
            }
        }
        //判断答案是否正确
        if(!answer.isEmpty()&&answer==q.answer()){
            qDebug()<<"回答正确";
        }else{
            qDebug()<<"回答错误";
        }

    });
    //上一题
    connect(ui->previous,&QPushButton::clicked,[=]{
        index--;
        if(index<=0){
            index= 0;
        }
        viewQuestion(index);
    });
    //提交
    connect(ui->summit,&QPushButton::clicked,[=]{});
    //退出
    connect(ui->quit,&QPushButton::clicked,[=]{});
}
void SimulatedExam::viewQuestion(int index){
    QString title;
    int i = index;
    i++;
    Question q = list[index];
    ui->topic->setText(q.name());
    ui->radioButton_4->hide();
    ui->radioButton_5->hide();
    ui->checkBox_4->hide();
    ui->checkBox_5->hide();
    if(q.type()==0){
        ui->stackedWidget->setCurrentIndex(0);
        //单选
        ui->radioButton_1->setText("A."+q.option1());
        ui->radioButton_2->setText("B."+q.option2());
        ui->radioButton_3->show();
        ui->radioButton_3->setText("C."+q.option3());
        if(!q.option4().isEmpty()){
            ui->radioButton_4->show();
            ui->radioButton_4->setText("D."+q.option4());
        }
        if(!q.option5().isEmpty()){
            ui->radioButton_5->show();
            ui->radioButton_5->setText("E."+q.option5());
        }
        ui->radioButton_1->setProperty("value", "A");
        ui->radioButton_2->setProperty("value", "B");
        ui->radioButton_3->setProperty("value", "C");
        ui->radioButton_4->setProperty("value", "D");
        ui->radioButton_5->setProperty("value", "E");
        title.append(QString::number(i)).append("/").append(QString::number(list.size())).append("  [").append("单选").append("]");
    }else if(q.type()==1){
        //判断
        ui->radioButton_1->setText("A.正确");
        ui->radioButton_2->setText("B.错误");
        ui->radioButton_1->setProperty("value", "A");
        ui->radioButton_2->setProperty("value", "B");
        ui->radioButton_3->hide();
        title.append(QString::number(i)).append("/").append(QString::number(list.size())).append("  [").append("判断").append("]");
    }else if(q.type()==2){
        ui->stackedWidget->setCurrentIndex(1);
       //多选
        ui->checkBox_1->setText("A."+q.option1());
        ui->checkBox_2->setText("B."+q.option2());
        ui->checkBox_3->setText("C."+q.option3());
        if(!q.option4().isEmpty()){
            ui->checkBox_4->show();
            ui->checkBox_4->setText("D."+q.option4());
        }
        if(!q.option5().isEmpty()){
            ui->checkBox_5->show();
            ui->checkBox_5->setText("E."+q.option5());
        }
        ui->checkBox_1->setProperty("value", "A");
        ui->checkBox_2->setProperty("value", "B");
        ui->checkBox_3->setProperty("value", "C");
        ui->checkBox_4->setProperty("value", "D");
        ui->checkBox_5->setProperty("value", "E");
        title.append(QString::number(i)).append("/").append(QString::number(list.size())).append("  [").append("多选").append("]");
    }
    ui->title->setText(title);
}
SimulatedExam::~SimulatedExam()
{
    delete ui;
}
