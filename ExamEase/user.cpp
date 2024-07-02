#include "user.h"



void User::setId(QString idx){
    this->id = idx;
}
void User::setName(QString namex){
     this->name=namex;
}
void User::setPassword(QString passwordx){
      this->password=passwordx;
}
QString User::getId(){
    return id;
}
QString User::getName(){
    return name;
}
QString User::getPassword(){
    return password;
}
