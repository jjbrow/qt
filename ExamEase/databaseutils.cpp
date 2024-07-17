#include "databaseutils.h"

DataBaseUtils::DataBaseUtils(QObject *parent) : QObject(parent)
{

}
DataBaseUtils::~DataBaseUtils(){
     closeDatabase();
}
//关闭数据库连接
void DataBaseUtils::closeDatabase(){
    if(db.open()){
        db.close();
    }
}
//连接数据库
void DataBaseUtils::connectDataBase(){
    // 避免重复连接数据库
    if(QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("ExamEase.db");     // 设置数据库名称
    }
    if(!db.open()) {
        qDebug() << "database open error：" << db.lastError();;
    } else {
        qDebug() << "database ok!";
    }

    // 检查 user 表是否存在，如果不存在则创建
    QSqlQuery query(db);

     QString createPaperTable = "CREATE TABLE IF NOT EXISTS paper ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "name TEXT, "
                                  "total INTEGER, "
                                  "create_date DATETIME)";

       if (!query.exec(createPaperTable)) {
           qDebug() << "Failed to create paper table:" << query.lastError();
           return ;
       }

       QString createQuestionTable = "CREATE TABLE IF NOT EXISTS question ("
                                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                     "paper_id INTEGER, "
                                     "name TEXT, "
                                     "type INTEGER, "
                                     "option1 TEXT, "
                                     "option2 TEXT, "
                                     "option3 TEXT, "
                                     "option4 TEXT, "
                                     "option5 TEXT, "
                                     "analysis TEXT,"
                                     "answer TEXT, "
                                     "create_date DATETIME)";

       if (!query.exec(createQuestionTable)) {
           qDebug() << "Failed to create question table:" << query.lastError();
           return ;
       }
}

bool DataBaseUtils::insertQuestion(const Question &question)
{
    QSqlQuery query;
    query.prepare("INSERT INTO question (paper_id, name, type, option1, option2, option3, option4, option5,analysis, answer, create_date) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?,?)");

    query.addBindValue(question.paperId());
    query.addBindValue(question.name());
    query.addBindValue(question.type());
    query.addBindValue(question.option1().isEmpty()? QVariant():question.option1());
    query.addBindValue(question.option2().isEmpty()? QVariant():question.option2());
    query.addBindValue(question.option3().isEmpty()? QVariant():question.option3());
    query.addBindValue(question.option4().isEmpty()? QVariant():question.option4());
    query.addBindValue(question.option5().isEmpty()? QVariant():question.option5());
    query.addBindValue(question.analysis().isEmpty()? QVariant():question.analysis());
    query.addBindValue(question.answer().isEmpty()? QVariant():question.answer());
    query.addBindValue(question.createDate());

    if (!query.exec()) {
        qDebug() << "Failed to insert question:" << query.lastError();
        return false;
    }
    return true;
}
bool DataBaseUtils::updateQuestion(const Question &question)
{
    QSqlQuery query;
    query.prepare("UPDATE question SET paper_id = ?, name = ?, type = ?, option1 = ?, option2 = ?, option3 = ?, option4 = ?, option5 = ?,analysis=? answer = ?, create_date = ? "
                  "WHERE id = ?");

    query.addBindValue(question.paperId());
    query.addBindValue(question.name());
    query.addBindValue(question.type());
    query.addBindValue(question.option1());
    query.addBindValue(question.option2());
    query.addBindValue(question.option3());
    query.addBindValue(question.option4());
    query.addBindValue(question.option5());
    query.addBindValue(question.analysis());
    query.addBindValue(question.answer());
    query.addBindValue(question.createDate());
    query.addBindValue(question.id());

    return query.exec();
}
bool DataBaseUtils::deleteQuestion(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM question WHERE id = ?");
    query.addBindValue(id);

    return query.exec();
}
Question DataBaseUtils::getQuestion(int id)
{
    QSqlQuery query;
    query.prepare("SELECT id, paper_id, name, type, option1, option2, option3, option4, option5,analysis, answer, create_date FROM question WHERE id = ?");
    query.addBindValue(id);
    query.exec();

    Question question;
    if (query.next()) {
        question.setId(query.value(0).toInt());
        question.setPaperId(query.value(1).toInt());
        question.setName(query.value(2).toString());
        question.setType(query.value(3).toInt());
        question.setOption1(query.value(4).toString());
        question.setOption2(query.value(5).toString());
        question.setOption3(query.value(6).toString());
        question.setOption4(query.value(7).toString());
        question.setOption5(query.value(8).toString());
        question.setAnalysis(query.value(9).toString());
        question.setAnswer(query.value(10).toString());
        question.setCreateDate(query.value(11).toDateTime());
    }

    return question;
}
QList<Question> DataBaseUtils::getAllQuestions()
{
    QList<Question> questions;
    QSqlQuery query("SELECT id, paper_id, name, type, option1, option2, option3, option4, option5, answer, create_date FROM question");

    while (query.next()) {
        Question question;
        question.setId(query.value(0).toInt());
        question.setPaperId(query.value(1).toInt());
        question.setName(query.value(2).toString());
        question.setType(query.value(3).toInt());
        question.setOption1(query.value(4).toString());
        question.setOption2(query.value(5).toString());
        question.setOption3(query.value(6).toString());
        question.setOption4(query.value(7).toString());
        question.setOption5(query.value(8).toString());
        question.setAnalysis(query.value(9).toString());
        question.setAnswer(query.value(10).toString());
        question.setCreateDate(query.value(11).toDateTime());

        questions.append(question);
    }

    return questions;
}

QList<Question> DataBaseUtils::getAllQuestionsByPaper(int id)
{
    QList<Question> questions;
    QSqlQuery query("SELECT id, paper_id, name, type, option1, option2, option3, option4, option5, answer, create_date FROM question where paper_id = ? ");
    query.addBindValue(id);

    query.exec();
    while (query.next()) {
        Question question;
        question.setId(query.value(0).toInt());
        question.setPaperId(query.value(1).toInt());
        question.setName(query.value(2).toString());
        question.setType(query.value(3).toInt());
        question.setOption1(query.value(4).toString());
        question.setOption2(query.value(5).toString());
        question.setOption3(query.value(6).toString());
        question.setOption4(query.value(7).toString());
        question.setOption5(query.value(8).toString());
        question.setAnalysis(query.value(9).toString());
        question.setAnswer(query.value(10).toString());
        question.setCreateDate(query.value(11).toDateTime());

        questions.append(question);
    }

    return questions;
}

int DataBaseUtils::insertPaper(const Paper &paper)
{
    QSqlQuery query;
    query.prepare("INSERT INTO paper (name, total, create_date) VALUES (?, ?, ?)");

    query.addBindValue(paper.name());
    query.addBindValue(paper.total());
    query.addBindValue(paper.createDate());
    query.exec();
    return query.lastInsertId().toInt();
}
bool DataBaseUtils::updatePaper(const Paper &paper)
{
    QSqlQuery query;
    query.prepare("UPDATE paper SET name = ? WHERE id = ?");
    if(paper.name()!=NULL){
        query.addBindValue(paper.name());
    }
    query.addBindValue(paper.id());

    if (!query.exec()) {
        qDebug() << "Failed to update paper:" << query.lastError().text();
    }

    return query.exec();
}
bool DataBaseUtils::deletePaper(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM paper WHERE id = ?");
    query.addBindValue(id);

    return query.exec();
}
Paper DataBaseUtils::getPaper(int id)
{
    QSqlQuery query;
    query.prepare("SELECT id, name, total, create_date FROM paper WHERE id = ?");
    query.addBindValue(id);
    query.exec();

    Paper paper;
    if (query.next()) {
        paper.setId(query.value(0).toInt());
        paper.setName(query.value(1).toString());
        paper.setTotal(query.value(2).toInt());
        paper.setCreateDate(query.value(3).toDateTime());
    }

    return paper;
}
QList<Paper> DataBaseUtils::getAllPapers()
{
    QList<Paper> papers;
    QSqlQuery query("SELECT id, name, total, create_date FROM paper");

    while (query.next()) {
        Paper paper;
        paper.setId(query.value(0).toInt());
        paper.setName(query.value(1).toString());
        paper.setTotal(query.value(2).toInt());
        paper.setCreateDate(query.value(3).toDateTime());

        papers.append(paper);
    }

    return papers;
}




