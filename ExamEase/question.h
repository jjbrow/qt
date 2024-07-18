#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QDateTime>

class Question
{
public:
    Question();

    int id() const;
    void setId(int id);

    int paperId() const;
    void setPaperId(int paperId);

    QString name() const;
    void setName(const QString &name);

    int type() const;
    void setType(int type);

    QString option1() const;
    void setOption1(const QString &option1);

    QString option2() const;
    void setOption2(const QString &option2);

    QString option3() const;
    void setOption3(const QString &option3);

    QString option4() const;
    void setOption4(const QString &option4);

    QString option5() const;
    void setOption5(const QString &option5);

    QString answer() const;
    void setAnswer(QString answer);

    QString analysis() const;
    void setAnalysis(const QString &analysis);

    QDateTime createDate() const;
    void setCreateDate(const QDateTime &createDate);


private:
    int m_id;
    int m_paperId;
    QString m_name;
    int m_type;
    QString m_option1;
    QString m_option2;
    QString m_option3;
    QString m_option4;
    QString m_option5;
    //解析
    QString m_analysis;
    QString m_answer;
    QDateTime m_createDate;
};

#endif // QUESTION_H
