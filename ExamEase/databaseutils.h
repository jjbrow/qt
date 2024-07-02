#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include <QObject>
#include <QDebug>
#include "paper.h"
#include "question.h"
class DataBaseUtils : public QObject
{
    Q_OBJECT
public:
    explicit DataBaseUtils(QObject *parent = nullptr);
    ~DataBaseUtils();
    void connectDataBase();
    void closeDatabase();

    // 插入新的Paper
    bool insertPaper(const Paper &paper);

    // 更新已有的Paper
    bool updatePaper(const Paper &paper);

    // 删除Paper
    bool deletePaper(int id);

    // 查询Paper
    Paper getPaper(int id);

    // 查询所有Paper
    QList<Paper> getAllPapers();

    // 插入新的Question
    bool insertQuestion(const Question &question);

    // 更新已有的Question
    bool updateQuestion(const Question &question);

    // 删除Question
    bool deleteQuestion(int id);

    // 查询Question
    Question getQuestion(int id);

    // 查询所有Question
    QList<Question> getAllQuestions();



private:
    QSqlDatabase db;
};

#endif // DATABASEUTILS_H
