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
    QString createTableSql = "CREATE TABLE IF NOT EXISTS user ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "username varchar NOT NULL UNIQUE, "
                                "password varchar NOT NULL)";

     if (!query.exec(createTableSql)) {
         qDebug() << "Failed to create table:" << query.lastError();
         return;
     }

       // 检查是否有用户名为 admin 的用户，如果没有则插入
     QString checkUserSql = "SELECT COUNT(*) FROM user WHERE username = 'admin'";
     if (!query.exec(checkUserSql)) {
          qDebug() << "Failed to check user:" << query.lastError();
          return;
     }

     if (query.next()) {
         int count = query.value(0).toInt();
         if (count == 0) {
             QString insertAdminSql = "INSERT INTO user (username, password) VALUES ('admin', 'admin')";
             if (!query.exec(insertAdminSql)) {
                   qDebug() << "Failed to insert admin user:" << query.lastError();
               }
           }
      }else{
         qDebug()<<"admin user already exists";
      }
}

//查询用户
User DataBaseUtils::queryById(const QString &id){
   User user;
   QString sqlStr = QString("select * from user where id='%1';").arg(id);
   QSqlQuery query(db);
   if(query.exec(sqlStr)){
       if(query.next()){
          user.setId(query.value(0).toString());
          user.setName(query.value(1).toString());
          user.setPassword(query.value(2).toString());
       }
   }
   return user;
}
//更新密码
bool DataBaseUtils::updateUser(const QString &password){
    QString sqlStr = QString("update user set password='%1' where id='1';").arg(password);
    QSqlQuery query(db);
     if (!query.exec(sqlStr)) {
         qWarning() << "Error: failed to update user" << query.lastError().text();
         return false;
     }
     return true;
}


