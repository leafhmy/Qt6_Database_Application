#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QObject>
#include <QAction>
#include <QPushButton>
#include "sqlhelper.h"
#include "user.h"
#include <QListWidgetItem>
#include <QIcon>
#include <QMessageBox>
#include <QBarSeries>
#include <QSqlError>
#include <QComboBox>
#include <QThread>

namespace Ui { class MainWindow;};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SQLHelper *sql;  // 整个项目维护同一个sql！
    User* user;
    bool isLogin;
    bool isConnected;
    bool init;

    // 用于标识当前是否是编辑状态
    bool editMode = true;

    // 记录打开的tableodel
    QMap<QString, QSqlTableModel*> openedTableModel;

    QMap<QString, QChart*> openedChart;

    QSqlQueryModel* queryModel;

    // 当前TableView显示的tableModel
    QSqlTableModel* currentTableModel;

    // 切换显示表，更新控件动态内容
    void resetWidgetInfo(QSqlTableModel*);

    QSqlQueryModel* setQueryModel(QSqlQueryModel*);


signals:
    void loginSuccess(int, QString);

    void connectSuccess(QString, QString);

private slots:
    void onLoginAction();

    void onUserAction();

    void onConnectAction();

    void onLoginBtn();

    void onLoginSuccess(int, QString);

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

    void onComboBoxStaYearMChanged(QString);

    void onComboBoxStaMonthMChanged(QString);

    void onComboBoxSaleYearChanged(QString);

    void onFundAction();

    void autoFillLoginInfo(QString);

    void FUCK();

    void THANKS();

};

#endif // MAINWINDOW_H
