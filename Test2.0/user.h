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
#include "ui_mainwindow.h"
#include "factury.h"
#include "container.h"

class SQLHelper;

class User : public QObject
{
    Q_OBJECT
public:
    User();

    ~User();

    int userType;

    QString userName;

    bool isConnected;

    Factury* factury;

    QSharedPointer<QVector<QString> > tables;

    QSharedPointer<SQLHelper> sql;

    QSharedPointer<Container> container;

    // 根据不同类型用户选择其可见的Table
    virtual void InitData() = 0;

    virtual void setVisiabe(Ui::MainWindow*) = 0;

    virtual void connect(Ui::MainWindow*, QString, QString) = 0;

    virtual void disConnect(Ui::MainWindow*) = 0;

    void setFactury(Factury* factury);

    void setContainer(QSharedPointer<Container> container);

    void getTables();

    void showHomePage(Ui::MainWindow* ui, QString userName);

};

class Administrator : public User
{
public:

    Administrator();

    ~Administrator();

    void InitData() override;

    void setVisiabe(Ui::MainWindow*) override;

    void connect(Ui::MainWindow*, QString, QString) override;

    void disConnect(Ui::MainWindow*) override;

};

class Guest : public User
{
public:

    Guest();

    ~Guest();

    void InitData() override;

    void setVisiabe(Ui::MainWindow*) override;

    void connect(Ui::MainWindow*, QString, QString) override;

    void disConnect(Ui::MainWindow*) override;
};

#endif // USER_H
