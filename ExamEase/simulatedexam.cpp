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
    buttonGroup = new QButtonGroup(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);  // 移除帮助按钮
    this->setWindowIcon(QIcon(":/Image/simulated.svg"));
    //连接数据库
    db.connectDataBase();
    //显示第一页
    viewQuestion(index);
    //绑定按钮
    buttonOperate();
    timeOut();
}
//绑定按钮
void SimulatedExam::buttonOperate(){
    //下一题
    connect(ui->next,&QPushButton::clicked,[=]{
        Question q = list[index];

        if(clearFlag){
            //清空 换到下一页
            clearSelections();
            clearFlag = false;
            index++;
            if(index>=list.size()){
                index= list.size()-1;
            }
            viewQuestion(index);
            return;
        }else{
            clearFlag = true;
        }
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
            //排序
            if(!answer.isEmpty()){
                 std::sort(answer.begin(), answer.end());
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
            //分数
            if(q.type()==2){
                score += 2;
            }else{
                score += 1;
            }
            qDebug()<<"回答正确";
            ui->analysis->setText("解析:"+q.analysis());
            ui->result->setText("回答正确");
            QFont font("微软雅黑", 12);
            ui->result->setFont(font);
            QPalette palette = ui->result->palette();
            palette.setColor(QPalette::WindowText, QColor("#2BDE73"));
            ui->result->setPalette(palette);
            ui->score->setText(QString::number(score));
        }else{
            qDebug()<<"回答错误";
            ui->analysis->setText("解析:"+q.analysis());
            ui->result->setText("回答错误");
            QFont font("微软雅黑", 12);
            ui->result->setFont(font);
            QPalette palette = ui->result->palette();
            palette.setColor(QPalette::WindowText, Qt::red);
            ui->result->setPalette(palette);
        }
        if(index>=list.size()-1){
            ui->next->hide();
            summit();
        }


    });
    //提交
    connect(ui->summit,&QPushButton::clicked,[=]{
        summit();
    });
    //退出
    connect(ui->quit,&QPushButton::clicked,this, &QDialog::close);
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
        buttonGroup->addButton(ui->radioButton_1);
        buttonGroup->addButton(ui->radioButton_2);
        buttonGroup->addButton(ui->radioButton_3);
        buttonGroup->addButton(ui->radioButton_4);
        buttonGroup->addButton(ui->radioButton_5);

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
        ui->stackedWidget->setCurrentIndex(0);
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
void SimulatedExam::clearSelections(){
    ui->analysis->setText("解析:");
    ui->result->setText("");
    buttonGroup->setExclusive(false);
    QList<QAbstractButton*> buttons = buttonGroup->buttons();
    foreach(QAbstractButton* button, buttons) {
        button->setChecked(false);
    }

    QList<QCheckBox *> checkboxes = this->findChildren<QCheckBox *>();
    for(QCheckBox *checkbox : checkboxes) {
      checkbox->setChecked(false);
    }
    QList<QRadioButton *> radiobuttons = this->findChildren<QRadioButton *>();
    for(QRadioButton *radiobutton : radiobuttons) {
      radiobutton->setChecked(false);
    }
    buttonGroup->setExclusive(true);
}
void SimulatedExam::timeOut(){
        // 更新标签显示
        auto updateLabel = [&]() {
            int minutes = timeRemaining / 60;
            int seconds = timeRemaining % 60;
            ui->timeLabel->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
        };

        // 定时器超时槽函数
        connect(&timer, &QTimer::timeout, [=]() {
            if (timeRemaining > 0) {
                timeRemaining--;
                updateLabel();
            } else {
                timer.stop();
                ui->timeLabel->setText("时间截止");
                // 弹出对话框
                QMessageBox::information(this, "考试结束", "考试结束");
                summit();
            }
        });
        timer.start(1000); // 每秒钟触发一次
}
//提交
void SimulatedExam::summit(){
    //更新试卷分数
    Paper p;
    p.setId(paper_id);
    p.setTotal(score);
    if(db.updatePaperTotal(p)){
        QMessageBox msgBox;
        msgBox.setText("提交成功！");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        // 显示消息框并等待用户点击
        if (msgBox.exec() == QMessageBox::Ok) {
            // 关闭窗口
            close();
        }
    }else{
        QMessageBox msgBox;
        msgBox.setText("提交失败！");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        // 显示消息框并等待用户点击
        if (msgBox.exec() == QMessageBox::Ok) {
            // 关闭窗口
            close();
        }
    }

}
SimulatedExam::~SimulatedExam()
{
    delete ui;
    delete buttonGroup;
    db.closeDatabase();
}
void SimulatedExam::closeEvent(QCloseEvent *event) {
    emit dialogClosed(); // 发射信号
    timer.stop(); // 停止计时器
    QDialog::closeEvent(event); // 调用基类的关闭事件处理函数
}
int SimulatedExam::getPaperId(){
    return paper_id;
}
void SimulatedExam::setPaperId(int id){
    paper_id = id;
}
