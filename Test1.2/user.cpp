#include "user.h"

User::User()
{

}

User::~User()
{
    this->sql->close();
    delete this->sql;
    this->sql = nullptr;
}

void User::getTables()
{
    this->tables = this->sql->getTables();

}

void Administrator::InitData()
{
    getTables();
}

void Teacher::InitData()
{
    getTables();
}

void Student::InitData()
{
    getTables();
}
