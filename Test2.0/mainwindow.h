#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QObject>
#include <QAction>
#include <QPushButton>
#include <QListWidgetItem>
#include <QIcon>
#include <QMessageBox>
#include <QBarSeries>
#include <QSqlError>
#include <QComboBox>
#include "factury.h"
#include "user.h"
#include "container.h"

namespace Ui { class MainWindow;};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

public:
    Ui::MainWindow *ui;
    Factury* factury;
    QSharedPointer<SQLHelper> sql;  // 整个项目维护同一个sql！
    QSharedPointer<User> user;
    QSharedPointer<Container> container;
    bool isLogin;
    bool isConnected;
    bool init;

    // 用于标识当前是否是编辑状态
    bool editMode = true;

    // 切换显示表，更新控件动态内容
    void resetWidgetInfo(QSharedPointer<QSqlTableModel>);

    QSharedPointer<QSqlQueryModel> setQueryModel(QSharedPointer<QSqlQueryModel>);

signals:
    void loginSuccess(int, QString, QString);

    void connectSuccess(QString, QString);

private slots:
    void onLoginAction();

    void onUserAction();

    void onConnectAction();

    void onLoginBtn();

    // 行为界面注册按钮
    void onRgBtn();

    // 注册界面注册按钮
    void onGuestRgBtn();

    void onOrderTicketBtn();

    void onRefundBtn();

    void onLoginSuccess(int, QString, QString);

    void onConnectBtn();

    void onConnectSuccess(QString, QString);

    void onLogoutBtn();

    void onDisconnectBtn();

    // tableList选择tableItem显示相应的表
    void onItemACtivated(QListWidgetItem *item);

    // 处理编辑的槽函数
    void onAddAction();

    void onDeleteAction();

    void onSubmitAction();

    void onCancelACtion();

    void onEditAction();

    // 数据槽函数

    void onAscAction();

    void onDescAction();

    void onFilterBtn();

    void onFilterAdvBtn();

    // 字段筛选项改变，改变数据项
    void onFieldCurrentIndexChanged(int);

    void onSellACtion();

    void onComboBoxStaYearChanged(QString);

    // 查询某天
    void onStaYearMBtn();

    // 查询某月
    void onStaYearMBtn2();

    // 销售员业绩
    void onSellOpYearBtn();

    void onSellOpMonthBtn();

    void onComboBoxStaYearMChanged(QString);

    void onComboBoxStaMonthMChanged(QString);

    void onComboBoxSaleYearChanged(QString);

    void onsellOpComboBoxYearChanged(QString);

    void onFundAction();

    void autoFillLoginInfo(QString);

    void onOrderAction();

    void onQueryAction();

    void onRefundAction();

    void FUCK();

    void THANKS();

};

#endif // MAINWINDOW_H
