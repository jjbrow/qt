#include "question.h"

Question::Question()
    : m_id(-1), m_paperId(-1), m_type(0)
{
}

int Question::id() const
{
    return m_id;
}

void Question::setId(int id)
{
    m_id = id;
}

int Question::paperId() const
{
    return m_paperId;
}

void Question::setPaperId(int paperId)
{
    m_paperId = paperId;
}

QString Question::name() const
{
    return m_name;
}

void Question::setName(const QString &name)
{
    m_name = name;
}

int Question::type() const
{
    return m_type;
}

void Question::setType(int type)
{
    m_type = type;
}

QString Question::option1() const
{
    return m_option1;
}

void Question::setOption1(const QString &option1)
{
    m_option1 = option1;
}

QString Question::option2() const
{
    return m_option2;
}

void Question::setOption2(const QString &option2)
{
    m_option2 = option2;
}

QString Question::option3() const
{
    return m_option3;
}

void Question::setOption3(const QString &option3)
{
    m_option3 = option3;
}

QString Question::option4() const
{
    return m_option4;
}

void Question::setOption4(const QString &option4)
{
    m_option4 = option4;
}

QString Question::option5() const
{
    return m_option5;
}

void Question::setOption5(const QString &option5)
{
    m_option5 = option5;
}

QString Question::analysis() const{
    return m_analysis;
}
void Question::setAnalysis(const QString &analysis){
    m_analysis = analysis;
}



QString Question::answer() const
{
    return m_answer;
}

void Question::setAnswer(QString answer)
{
    m_answer = answer;
}



QDateTime Question::createDate() const
{
    return m_createDate;
}

void Question::setCreateDate(const QDateTime &createDate)
{
    m_createDate = createDate;
}
