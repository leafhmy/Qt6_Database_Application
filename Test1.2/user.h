#ifndef USER_H
#define USER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "sqlhelper.h"
#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QIcon>

class User : public QObject
{
    Q_OBJECT
public:
    User();

    ~User();

    int userType;

    virtual void InitData() = 0;

    void getTables();

    QVector<QString>* tables;

    SQLHelper* sql;

};

class Administrator : public User
{
public:

    // 根据不同类型用户选择其可见的Table
    void InitData() override;

};

class Teacher : public User
{
public:

    void InitData() override;
};

class Student : public User
{
public:

    void InitData() override;
};

#endif // USER_H
