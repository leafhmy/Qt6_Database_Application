#include "mainwindow.h"
#include "../QRibbon/QRibbon.h"
#include "ui_mainwindow.h"

//TODO: 显示当前是否在编辑状态，以及是否有未提交的编辑。

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    ui = new Ui::MainWindow();
    ui->setupUi(this);
    QRibbon::install(this);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    this->isLogin = false;
    this->isConnected = false;
    this->init = false;
    this->currentTableModel = nullptr;
    this->queryModel = nullptr;


    //连接信号和槽函数

    this->addAction(ui->actionFullScreen);
    connect(ui->actionFullScreen, &QAction::triggered, [&]() {

        auto full = ui->actionFullScreen->isChecked();
        menuBar()->setVisible(!full);
        ui->actionFullScreen->setShortcut(  full ? QKeySequence("Esc") : QKeySequence("Ctrl+F"));

        static bool maximized = false;// 记录当前状态
        if ( full )
        {
            maximized = isMaximized();
        }
        else if ( maximized && isMaximized() )
        {
            return;
        }

        if ( full && !isMaximized() || !full && isMaximized() )
        {
            if (isMaximized())
            {
                showNormal();
            }
            else
                showMaximized();
        }
    });

    // FUCK BEGIN
    connect(ui->orderAction, &QAction::triggered,
            this,  &MainWindow::FUCK);

    connect(ui->queryAction, &QAction::triggered,
            this,  &MainWindow::FUCK);

    connect(ui->refundAction, &QAction::triggered,
            this,  &MainWindow::FUCK);

    connect(ui->refundAction, &QAction::triggered,
            this,  &MainWindow::FUCK);

    connect(ui->action_3, &QAction::triggered,
            this,  &MainWindow::FUCK);

    connect(ui->action_4, &QAction::triggered,
            this,  &MainWindow::FUCK);

    connect(ui->action_7, &QAction::triggered,
            this,  &MainWindow::FUCK);

    connect(ui->thanksAction, &QAction::triggered,
            this,  &MainWindow::THANKS);

    // FUCK END

    connect(ui->comboBoxDbType, &QComboBox::currentTextChanged,
            this, &MainWindow::autoFillLoginInfo);

    connect(ui->actionLogin, &QAction::triggered,
            this,  &MainWindow::onLoginAction);

    connect(ui->actionUser, &QAction::triggered,
            this,  &MainWindow::onUserAction);

    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::onConnectAction);

    connect(ui->loginButton, &QPushButton::clicked,
            this, &MainWindow::onLoginBtn);

    connect(this, &MainWindow::loginSuccess,
            this, &MainWindow::onLoginSuccess);

    connect(ui->connectButton, &QPushButton::clicked,
            this, &MainWindow::onConnectBtn);

    connect(this, &MainWindow::connectSuccess,
            this, &MainWindow::onConnectSuccess);

    connect(ui->logoutButton, &QPushButton::clicked,
            this, &MainWindow::onLogoutBtn);

    connect(ui->disconnectButton, &QPushButton::clicked,
            this, &MainWindow::onDisconnectBtn);

    connect(ui->tableList, &QListWidget::itemActivated,
            this, &MainWindow::onItemACtivated);

    // 连接编辑处理信号和槽函数
    connect(ui->addAction, &QAction::triggered,
            this, &MainWindow::onAddAction);

    connect(ui->deleteAction, &QAction::triggered,
            this, &MainWindow::onDeleteAction);

    connect(ui->submitAction, &QAction::triggered,
            this, &MainWindow::onSubmitAction);

    connect(ui->cancelAction, &QAction::triggered,
            this, &MainWindow::onCancelACtion);

    connect(ui->editAction, &QAction::triggered,
            this, &MainWindow::onEditAction);

    // 连接数据处理信号和槽函数
    connect(ui->ascAction, &QAction::triggered,
            this, &MainWindow::onAscAction);

    connect(ui->descAction, &QAction::triggered,
            this, &MainWindow::onDescAction);

    connect(ui->filterButton, &QPushButton::clicked,
            this, &MainWindow::onFilterBtn);

    connect(ui->filterButtonAdv, &QPushButton::clicked,
            this, &MainWindow::onFilterAdvBtn);

    connect(ui->comboBoxCol, &QComboBox::currentIndexChanged,
            this, &MainWindow::onFieldCurrentIndexChanged);

    // 数据统计槽函数
    connect(ui->sellAction, &QAction::triggered,
            this, &MainWindow::onSellACtion);

    connect(ui->comboBoxStaYear, &QComboBox::currentTextChanged,
            this, &MainWindow::onComboBoxStaYearChanged);

    connect(ui->fundAction, &QAction::triggered,
            this, &MainWindow::onFundAction);

    connect(ui->comboBoxSaleYear, &QComboBox::currentTextChanged,
            this, &MainWindow::onComboBoxSaleYearChanged);

    connect(ui->comboBoxStaYearM, &QComboBox::currentTextChanged,
            this, &MainWindow::onComboBoxStaYearMChanged);

    connect(ui->comboBoxStaMonthM, &QComboBox::currentTextChanged,
            this, &MainWindow::onComboBoxStaMonthMChanged);

    connect(ui->staYearMButton, &QPushButton::clicked,
            this, &MainWindow::onStaYearMBtn);

    connect(ui->staYearMButton2, &QPushButton::clicked,
            this, &MainWindow::onStaYearMBtn2);

    // 默认显示提示页面
    ui->stackedWidget->setCurrentWidget(ui->NoticePage);

    ui->workStackedWidget->setCurrentWidget(ui->welcomePage);

    ui->welcomeLabel->setScaledContents(true);
    ui->welcomeLabel->resize(ui->welcomePage->size());

    // 初始空界面不启用编辑按钮
    ui->addAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->submitAction->setDisabled(true);
    ui->cancelAction->setDisabled(true);
    ui->editAction->setDisabled(true);

    ui->sellAction->setDisabled(true);
    ui->fundAction->setDisabled(true);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete sql;
    delete user;
}

QSqlQueryModel* MainWindow::setQueryModel(QSqlQueryModel* model)
{
    if (this->queryModel != nullptr)
    {
        delete this->queryModel;
        this->queryModel = nullptr;
    }

    this->queryModel = model;
    return this->queryModel;
}

//槽函数
// 点击菜单栏的登录选项
void MainWindow::onLoginAction()
{
    if (this->isLogin)
        ui->stackedWidget->setCurrentWidget(ui->UserInfoPage);
    else
        ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}

// 点击菜单栏的用户选项
void MainWindow::onUserAction()
{    if (this->isLogin)
        ui->stackedWidget->setCurrentWidget(ui->UserInfoPage);
    else
        ui->stackedWidget->setCurrentWidget(ui->NoticePage);
}

// 点击菜单栏的连接数据库选项
void MainWindow::onConnectAction()
{
    if (!this->isLogin)
        ui->stackedWidget->setCurrentWidget(ui->NoticePage);
    else if (!this->isConnected)
        ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
    else
        ui->stackedWidget->setCurrentWidget(ui->ConnectInfoPage);
}

// 点击登录按钮
void MainWindow::onLoginBtn()
{
    QString userName = ui->lineEditUser->text();
    QString password = ui->lineEditPassword->text();
    int userType = ui->comboBoxIdt->currentIndex();
    // 处理用户名和密码
    emit loginSuccess(userType, userName);
}

// 登录成功
void MainWindow::onLoginSuccess(int userType, QString userName)
{
    // 0管理员 1教师 2学生
    switch (userType) {
    case 0:
        this->user = new Administrator();
        break;
    case 1:
        this->user = new Teacher();
        break;
    case 2:
        this->user = new Student();
        break;
    }
    // 创建sql对象
    this->sql = new SQLHelper();
    this->user->sql = sql;

    this->user->userType = userType;
    this->isLogin = true;
    ui->userLabel->setText("欢迎您，管理员" + userName);
    ui->stackedWidget->setCurrentWidget(ui->UserInfoPage);
}

// 点击连接数据库按钮
void MainWindow::onConnectBtn()
{

    int dbType = ui->comboBoxDbType->currentIndex();
    QString dbUserName = ui->lineEditDbUser->text();
    QString dbPassword = ui->lineEditDbPassword->text();
    QString dbName = ui->lineEditDbName->text();

    QString dbHost = ui->lineEditDbHost->text();
    QString dbPort = ui->lineEditDbPort->text();


    // 处理数据库登录信息
    QString dbType_ = "";
    switch (dbType) {
    case 0:
        dbType_ = "QMYSQL";
        break;
    case 1:
        dbType_ = "QODBC";
        break;
    default:
        break;
    }
    bool connected = sql->connect(dbType_, dbUserName, dbPassword, dbName, dbHost, dbPort);
    if(connected)
    {
        this->isConnected = true;
        emit connectSuccess(dbUserName, dbName);
    }
    else
    {
        //处理连接失败
    }

}

// 数据库连接成功
void MainWindow::onConnectSuccess(QString dbUserName, QString dbName)
{
    ui->dbUserLabel->setText("用户：" + dbUserName);
    ui->dbNameLabel->setText("数据库：" + dbName);
    ui->stackedWidget->setCurrentWidget(ui->ConnectInfoPage);
    this->user->InitData();
    // 添加所有可见表名到tableList
    for (QString &table : *this->user->tables)
    {
        QListWidgetItem* item = new QListWidgetItem(table);
        ui->tableList->addItem(item);
    }

    ui->sellAction->setEnabled(true);
    ui->fundAction->setEnabled(true);

}

// 用户登出
void MainWindow::onLogoutBtn()
{
    if (this->isConnected)
    {
        onDisconnectBtn();
    }
    delete this->user;
    this->isLogin = false;
    ui->stackedWidget->setCurrentWidget(ui->NoticePage);
}

// 断开数据库连接
void MainWindow::onDisconnectBtn()
{
    this->isConnected = false;

    this->user->tables->clear();
    this->user->tables = nullptr;
    this->ui->tableList->clear();
    this->ui->comboBoxCol->clear();
    this->ui->comboBoxColAdv->clear();
    this->ui->comboBoxVal->clear();
    this->ui->comboBoxStaYearM->clear();
    this->ui->comboBoxStaMonthM->clear();
    this->ui->comboBoxStaDayM->clear();

    ui->comboBoxStaYear->clear();
    ui->comboBoxSaleYear->clear();

    //释放table model
    for (QSqlTableModel* tableModel : openedTableModel)
    {
        delete tableModel;
        tableModel = nullptr;
    }
    openedTableModel.clear();

    currentTableModel = nullptr;

    //释放chart
    for (QChart* chart : openedChart)
    {
//        delete chart;
        chart = nullptr;
    }

    openedChart.clear();

    // 释放querymodel
    if (this->queryModel != nullptr)
    {
        delete this->queryModel;
        this->queryModel = nullptr;
    }

    this->user->sql->close();
    ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
    ui->workStackedWidget->setCurrentWidget(ui->welcomePage);

    // 停用编辑按钮
    ui->addAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->submitAction->setDisabled(true);
    ui->cancelAction->setDisabled(true);
    ui->editAction->setDisabled(true);

    ui->sellAction->setDisabled(true);
    ui->fundAction->setDisabled(true);
}

// tableList选择tableItem显示相应的表
void MainWindow::onItemACtivated(QListWidgetItem *item)
{
    ui->workStackedWidget->setCurrentWidget(ui->baseTablePage);
    QSqlTableModel* tableModel;
    QString itemName = item->text();

    if (openedTableModel.find(itemName) != openedTableModel.end())
        tableModel = openedTableModel[itemName];
    else
    {
        tableModel = this->sql->getTableModel(itemName);
        openedTableModel[itemName] = tableModel;
    }

    this->currentTableModel = tableModel;

    // 显示表
    ui->tableView->setModel(this->currentTableModel);

    // 更新控件动态内容
    resetWidgetInfo(this->currentTableModel);


    // 启用编辑按钮
    ui->addAction->setEnabled(true);
    ui->deleteAction->setEnabled(true);
    ui->submitAction->setEnabled(true);
    ui->cancelAction->setEnabled(true);
    ui->editAction->setEnabled(true);

    // 设置为手动刷新，用来达到需要手动提交的效果
    currentTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    currentTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    //    // 设置双击编辑
//    ui->tableView->setEditTriggers(QTableView::DoubleClicked);

    // 将表格置为不可修改
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if (this->queryModel != nullptr)
    {
        delete this->queryModel;
        this->queryModel = nullptr;
    }

}

// 处理编辑的槽函数
void MainWindow::onAddAction()
{
    // 表格置为双击可修改
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

    // 设置编辑模式
    // m_editMode是自定义变量，用于标识当前是否是编辑状态
    this->editMode = true;

    // 将其余功能按键禁用，将提交和取消启用
    ui->addAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->editAction->setDisabled(true);

    ui->cancelAction->setEnabled(true);
    ui->submitAction->setEnabled(true);

    // m_sqlModel每次只能最大缓存256个，所以需要先fetchMore
    // 否则会导致移动到最后一行时，只会最多移到256行u
    while (currentTableModel->canFetchMore()) {
        currentTableModel->fetchMore();
    }

    int rowCount = currentTableModel->rowCount();

    // 在rowCount处添加一行
    currentTableModel->insertRow(rowCount);

    currentTableModel->setData(currentTableModel->index(rowCount, 0), QVariant());

    // 滚动到底部
    ui->tableView->scrollToBottom();

}

void MainWindow::onDeleteAction()
{
    // 将其余功能按键禁用，将提交和取消启用
    ui->addAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->editAction->setDisabled(true);

    ui->cancelAction->setEnabled(true);
    ui->submitAction->setEnabled(true);

    // 找到当前行，然后removeRow
    auto index = ui->tableView->currentIndex();

    if (!currentTableModel->removeRow(index.row()))
    {
        QSqlError error =  currentTableModel->lastError();
        QMessageBox::critical(this, "删除失败！", error.nativeErrorCode()+" "+error.text());
        onCancelACtion();
        return;
    }

    // 删除前，弹框确认
    int ok = QMessageBox::warning(this, "删除当前行！", "确定删除当前行？", QMessageBox::Yes, QMessageBox::No);

    if (ok == QMessageBox::No)
    {
        currentTableModel->revertAll();

        ui->addAction->setEnabled(true);
        ui->deleteAction->setEnabled(true);
        ui->cancelAction->setEnabled(true);
        ui->submitAction->setEnabled(true);
        ui->editAction->setEnabled(true);
        return;
    }

    // 提交
    currentTableModel->submitAll();
    // 查询
    currentTableModel->select();
    ui->addAction->setEnabled(true);
    ui->deleteAction->setEnabled(true);
    ui->cancelAction->setEnabled(true);
    ui->submitAction->setEnabled(true);
    ui->editAction->setEnabled(true);
    return;

}

void MainWindow::onEditAction()
{
    // 将其余功能按键禁用，将提交和取消启用
    ui->addAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->editAction->setDisabled(true);

    ui->cancelAction->setEnabled(true);
    ui->submitAction->setEnabled(true);

    // 表格置为双击可修改
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void MainWindow::onSubmitAction()
{
    editMode = false;

    // 提交弹框确认
    int ok = QMessageBox::warning(this, "提交修改", "确定提交修改？", QMessageBox::Yes, QMessageBox::No);

    if (ok == QMessageBox::No)
    {
        onCancelACtion();
        return;
    }

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 提交修改
    if (!currentTableModel->submitAll())
    {
        QSqlError error =  currentTableModel->lastError();
        QMessageBox::critical(this, "修改失败！", error.nativeErrorCode()+" "+error.text());
        onCancelACtion();
    }

    ui->addAction->setEnabled(true);
    ui->deleteAction->setEnabled(true);
    ui->cancelAction->setEnabled(true);
    ui->submitAction->setEnabled(true);
    ui->editAction->setEnabled(true);

    // 将表格置为不可修改
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void MainWindow::onCancelACtion()
{
    // 刷新表格的内容
    editMode = false;

    // 取消所有
    currentTableModel->revertAll();

    ui->addAction->setEnabled(true);
    ui->deleteAction->setEnabled(true);
    ui->cancelAction->setEnabled(true);
    ui->submitAction->setEnabled(true);
    ui->editAction->setEnabled(true);

    // 将表格置为不可修改
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::onAscAction()
{
    if (currentTableModel == nullptr)
        return;
    QModelIndex index = this->ui->tableView->currentIndex();
    int col_index = index.column();

    currentTableModel->sort(col_index, Qt::AscendingOrder);
}

void MainWindow::onDescAction()
{
    if (currentTableModel == nullptr)
        return;
    QModelIndex index = this->ui->tableView->currentIndex();
    int col_index = index.column();

    currentTableModel->sort(col_index, Qt::DescendingOrder);
}

void MainWindow::onFilterBtn()
{
    if (this->currentTableModel == nullptr)
        return;
    ui->workStackedWidget->setCurrentWidget(ui->queryTablePage);
    QString fieldName = ui->comboBoxCol->currentText();
    QString value;
    bool use_customed_value = ui->checkBoxCusVal->isChecked();
    if (use_customed_value)
        value = ui->lineEditVal->text();
    else
        value = ui->comboBoxVal->currentText();

    ui->queryTableView->setModel(this->setQueryModel(this->sql->selectByValue(this->currentTableModel->tableName(), fieldName, value)));

}

void MainWindow::onFilterAdvBtn()
{
    if (this->currentTableModel == nullptr)
        return;
    ui->workStackedWidget->setCurrentWidget(ui->queryTablePage);
    QString fieldName = ui->comboBoxColAdv->currentText();
    QString expression = ui->lineEditValAdv->text();
    QString from = ui->lineEditValFromAdv->text();
    QString to = ui->lineEditValToAdv->text();

    bool use_between = ui->checkBoxAdv->isChecked();
    if (use_between)
         ui->queryTableView->setModel(this->setQueryModel(this->sql->selectByValueBetween(this->currentTableModel->tableName(), fieldName, from, to)));
    else
         ui->queryTableView->setModel(this->setQueryModel(this->sql->selectByValueFuzzy(this->currentTableModel->tableName(), fieldName, expression)));
}

void MainWindow::onFieldCurrentIndexChanged(int index)
{
    if (index == -1)
        return;
    this->ui->comboBoxVal->clear();
    QString fieldName = ui->comboBoxCol->itemText(index);
    QVector<QString> values = this->sql->getValues(this->currentTableModel->tableName(), fieldName);

    for (QString &value : values)
        this->ui->comboBoxVal->addItem(value);
}


void MainWindow::resetWidgetInfo(QSqlTableModel* currentTableModel)
{
    // 更新筛选部分字段和值的选项
    ui->comboBoxCol->clear();
    ui->comboBoxColAdv->clear();
    ui->comboBoxVal->clear();

    // 获取字段
    QVector<QString> fields = this->sql->getFields(currentTableModel);

    for (QString &field : fields)
    {
        this->ui->comboBoxCol->addItem(field);
        this->ui->comboBoxColAdv->addItem(field);
    }

}

void MainWindow::onSellACtion()
{
    currentTableModel = nullptr;
    // 首次点击添加年份选项
    if (ui->comboBoxStaYear->currentIndex() == -1)
    {
        this->init = true;
        QMap<QString, QVector<QString>> years = this->sql->getDataTime();
        QMap<QString, QVector<QString>>::const_iterator it = years.constBegin();
        while (it != years.constEnd())
         {
             QString year =  it.key();
             ui->comboBoxStaYear->addItem(year);
             ui->comboBoxStaYearM->addItem(year);
             ++it;
         }

        this->ui->comboBoxStaMonthM->clear();
        this->ui->comboBoxStaDayM->clear();
        QString yearM = ui->comboBoxStaYearM->currentText();
        QVector<QString> monthData = this->sql->getMonth(yearM);
        for (QString & month : monthData)
            this->ui->comboBoxStaMonthM->addItem(month);
        QString monthM = ui->comboBoxStaMonthM->currentText();
        QVector<QString> dayData = this->sql->getDay(yearM, monthM);
        for (QString & day : dayData)
            this->ui->comboBoxStaDayM->addItem(day);
        this->init = false;
    }

    ui->workStackedWidget->setCurrentWidget(ui->statisticPage);
    ui->tabWidget_2->setCurrentWidget(ui->tabStaYear);

    QChart* chart = nullptr;
    QString year = ui->comboBoxStaYear->currentText();
    if (this->openedChart.find(year) != this->openedChart.end())
        chart = this->openedChart[year];
    else
    {
        chart = this->sql->getYearChartView(year);
        this->openedChart[year] = chart;
    }

    this->ui->tableViewStaMonth->setModel(this->setQueryModel(this->sql->getMonthTableModel(ui->comboBoxStaYearM->currentText(), ui->comboBoxStaMonthM->currentText())));

    ui->chartView->setChart(chart);



}

void MainWindow::onComboBoxStaYearChanged(QString year)
{
    if (ui->comboBoxStaYear->currentIndex() == -1)
        return;
    if (!this->isConnected)
        return;
    if (this->init)
        return;

    if (this->openedChart.find(year) != this->openedChart.end())
        ui->chartView->setChart(this->openedChart[year]);
    else
    {
        this->openedChart[year] = this->sql->getYearChartView(year);
        ui->chartView->setChart(this->openedChart[year]);
    }

}

void MainWindow::onStaYearMBtn()
{
    QString yearM = ui->comboBoxStaYearM->currentText();
    QString monthM = ui->comboBoxStaMonthM->currentText();
    QString dayM = ui->comboBoxStaDayM->currentText();

    this->ui->tableViewStaMonth->setModel(this->setQueryModel(this->sql->getDayTableModel(yearM, monthM, dayM)));
}

void MainWindow::onStaYearMBtn2()
{
    QString yearM = ui->comboBoxStaYearM->currentText();
    QString monthM = ui->comboBoxStaMonthM->currentText();

    this->ui->tableViewStaMonth->setModel(this->setQueryModel(this->sql->getMonthTableModel(yearM, monthM)));
}

void MainWindow::onComboBoxSaleYearChanged(QString year)
{
    if (this->ui->comboBoxSaleYear->count() == -1)
        return;
    QChart* chart;
    QString year_ = year;
    year = year + "sale";
    if (this->openedChart.find(year) != this->openedChart.end())
        chart = this->openedChart[year];
    else
    {
        chart = this->sql->getSaleChartView(year_);
        this->openedChart[year] = chart;
    }
    ui->chartView2->setChart(chart);
}

void MainWindow::onComboBoxStaYearMChanged(QString year)
{
    if (this->ui->comboBoxStaYearM->count() == -1)
        return;
    this->ui->comboBoxStaMonthM->clear();
    this->ui->comboBoxStaDayM->clear();

    QString yearM = ui->comboBoxStaYearM->currentText();
    QVector<QString> monthData = this->sql->getMonth(yearM);
    for (QString & month : monthData)
        this->ui->comboBoxStaMonthM->addItem(month);

    QString monthM = ui->comboBoxStaMonthM->currentText();
    QVector<QString> dayData = this->sql->getDay(yearM, monthM);
    for (QString & day : dayData)
        this->ui->comboBoxStaDayM->addItem(day);


}

void MainWindow::onComboBoxStaMonthMChanged(QString month)
{
    if (this->ui->comboBoxStaMonthM->count() == -1)
        return;
    this->ui->comboBoxStaDayM->clear();

    QString yearM = ui->comboBoxStaYearM->currentText();
    QString monthM = ui->comboBoxStaMonthM->currentText();
    QVector<QString> dayData = this->sql->getDay(yearM, monthM);
    for (QString & day : dayData)
        this->ui->comboBoxStaDayM->addItem(day);

}

void MainWindow::onFundAction()
{
    currentTableModel = nullptr;
    // 首次点击添加年份选项
    if (ui->comboBoxSaleYear->currentIndex() == -1)
    {
        QMap<QString, QVector<QString>> years = this->sql->getDataTime();
        QMap<QString, QVector<QString>>::const_iterator it = years.constBegin();
        while (it != years.constEnd())
         {
             QString year =  it.key();
             ui->comboBoxSaleYear->addItem(year);
             ++it;
         }
    }

    ui->workStackedWidget->setCurrentWidget(ui->statisticPage);
    ui->tabWidget_2->setCurrentWidget(ui->tabSaleYear);

    QChart* chart;
    QString year = ui->comboBoxSaleYear->currentText();
    QString year_ = year;
    year = year + "sale";
    if (this->openedChart.find(year) != this->openedChart.end())
        chart = this->openedChart[year];
    else
    {
        chart = this->sql->getSaleChartView(year_);
        this->openedChart[year] = chart;
    }

    ui->chartView2->setChart(chart);
}

void MainWindow::autoFillLoginInfo(QString dbType)
{
    if (dbType == "SQL Server")
    {
        ui->lineEditDbUser->setText("sa");
        ui->lineEditDbPassword->setText("123456");
        ui->lineEditDbName->setText("DB");
        ui->lineEditDbHost->setText("172.16.220.170");
        ui->lineEditDbPort->setText("1433");
    }

}

void MainWindow::FUCK()
{
    QMessageBox::warning(this, "FUCK!", "请联系管理员实现该功能（不一定会实现）^_^", QMessageBox::Yes);
}

void MainWindow::THANKS()
{
    QString text = QString("鸣谢：\n \n HMY\n 喝牛奶的波波鱼\n 你妈\n 罗老师\n \n Copyright: MJUAI19");
    QMessageBox::information(this, "^_^ 鸣谢 ^_^", text, QMessageBox::Yes);
}
