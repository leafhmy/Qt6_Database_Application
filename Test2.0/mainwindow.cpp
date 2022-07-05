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

    connect(ui->orderAction, &QAction::triggered, this,  &MainWindow::onOrderAction);

    connect(ui->queryAction, &QAction::triggered, this,  &MainWindow::onQueryAction);

    connect(ui->refundAction, &QAction::triggered, this,  &MainWindow::onRefundAction);

    // FUCK BEGIN

    connect(ui->action_3, &QAction::triggered, this,  &MainWindow::FUCK);

    connect(ui->action_4, &QAction::triggered, this,  &MainWindow::FUCK);

    connect(ui->action_7, &QAction::triggered, this,  &MainWindow::FUCK);

    connect(ui->thanksAction, &QAction::triggered, this,  &MainWindow::THANKS);

    // FUCK END

    connect(ui->comboBoxDbType, &QComboBox::currentTextChanged, this, &MainWindow::autoFillLoginInfo);

    connect(ui->actionLogin, &QAction::triggered, this,  &MainWindow::onLoginAction);

    connect(ui->actionUser, &QAction::triggered, this,  &MainWindow::onUserAction);

    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::onConnectAction);

    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginBtn);

    connect(this, &MainWindow::loginSuccess, this, &MainWindow::onLoginSuccess);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectBtn);

    connect(this, &MainWindow::connectSuccess, this, &MainWindow::onConnectSuccess);

    connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutBtn);

    connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::onDisconnectBtn);

    connect(ui->tableList, &QListWidget::itemActivated, this, &MainWindow::onItemACtivated);

    // 连接编辑处理信号和槽函数
    connect(ui->addAction, &QAction::triggered, this, &MainWindow::onAddAction);

    connect(ui->deleteAction, &QAction::triggered, this, &MainWindow::onDeleteAction);

    connect(ui->submitAction, &QAction::triggered, this, &MainWindow::onSubmitAction);

    connect(ui->cancelAction, &QAction::triggered, this, &MainWindow::onCancelACtion);

    connect(ui->editAction, &QAction::triggered, this, &MainWindow::onEditAction);

    // 连接数据处理信号和槽函数
    connect(ui->ascAction, &QAction::triggered, this, &MainWindow::onAscAction);

    connect(ui->descAction, &QAction::triggered, this, &MainWindow::onDescAction);

    connect(ui->filterButton, &QPushButton::clicked, this, &MainWindow::onFilterBtn);

    connect(ui->filterButtonAdv, &QPushButton::clicked, this, &MainWindow::onFilterAdvBtn);

    connect(ui->comboBoxCol, &QComboBox::currentIndexChanged, this, &MainWindow::onFieldCurrentIndexChanged);

    // 数据统计槽函数
    connect(ui->sellAction, &QAction::triggered, this, &MainWindow::onSellACtion);

    connect(ui->comboBoxStaYear, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxStaYearChanged);

    connect(ui->fundAction, &QAction::triggered, this, &MainWindow::onFundAction);

    connect(ui->comboBoxSaleYear, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxSaleYearChanged);

    connect(ui->comboBoxStaYearM, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxStaYearMChanged);

    connect(ui->sellOpComboBoxYear, &QComboBox::currentTextChanged, this, &MainWindow::onsellOpComboBoxYearChanged);

    connect(ui->comboBoxStaMonthM, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxStaMonthMChanged);

    connect(ui->staYearMButton, &QPushButton::clicked, this, &MainWindow::onStaYearMBtn);

    connect(ui->staYearMButton2, &QPushButton::clicked, this, &MainWindow::onStaYearMBtn2);

    connect(ui->sellOpYearButton, &QPushButton::clicked, this, &MainWindow::onSellOpYearBtn);

    connect(ui->sellOpMonthButton, &QPushButton::clicked, this, &MainWindow::onSellOpMonthBtn);

    connect(ui->RgButton, &QPushButton::clicked, this, &MainWindow::onRgBtn);

    connect(ui->RgPushButton, &QPushButton::clicked, this, &MainWindow::onGuestRgBtn);

    connect(ui->orderTicketButton, &QPushButton::clicked, this, &MainWindow::onOrderTicketBtn);

    connect(ui->refundButton, &QPushButton::clicked, this, & MainWindow::onRefundBtn);

    // 默认显示提示页面
    ui->stackedWidget->setCurrentWidget(ui->NoticePage);
    ui->workStackedWidget->setCurrentWidget(ui->welcomePage);

    ui->objectDockWidget->hide();
    ui->activeDockWidget->hide();

    // 初始空界面不启用编辑、管理、数据按钮
    ui->addAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->submitAction->setDisabled(true);
    ui->cancelAction->setDisabled(true);
    ui->editAction->setDisabled(true);

    ui->sellAction->setDisabled(true);
    ui->fundAction->setDisabled(true);
    ui->orderAction->setDisabled(true);
    ui->queryAction->setDisabled(true);
    ui->refundAction->setDisabled(true);

    ui->ascAction->setDisabled(true);
    ui->descAction->setDisabled(true);
    ui->filterAction->setDisabled(true);

    // 创建sql对象
    sql = factury->getSQLHelper(factury);

    // 创建container对象
    container = factury->getContainer(factury, sql);
}

MainWindow::~MainWindow()
{
    this->sql->close();
    delete ui;
}

QSharedPointer<QSqlQueryModel> MainWindow::setQueryModel(QSharedPointer<QSqlQueryModel> model)
{
    if (!container->queryModel.isNull())
        container->queryModel->clear();

    container->queryModel = model;
    return container->queryModel;
}

//槽函数
// 点击菜单栏的登录选项
void MainWindow::onLoginAction()
{
    if (!this->isConnected)
        ui->stackedWidget->setCurrentWidget(ui->NoticePage);
    else if (this->isLogin)
        ui->stackedWidget->setCurrentWidget(ui->UserInfoPage);
    else
        ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}

// 点击菜单栏的用户选项
void MainWindow::onUserAction()
{
    if (!this->isConnected)
        ui->stackedWidget->setCurrentWidget(ui->NoticePage);
    else if (this->isLogin)
        ui->stackedWidget->setCurrentWidget(ui->UserInfoPage);
    else
        ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}

// 点击菜单栏的连接数据库选项
void MainWindow::onConnectAction()
{
    if (!this->isConnected)
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
    emit loginSuccess(userType, userName, password);
}

// 登录
void MainWindow::onLoginSuccess(int userType, QString userName, QString password)
{
    if (userType == 1)
    {
        int response = sql->loginIdentification(userName, password);
        if (response == 0)
        {
            QMessageBox::information(this, "-_-||", "用户不存在，请注册！", QMessageBox::Yes);
            ui->workStackedWidget->setCurrentWidget(ui->guestRegisterPage);
        }

        if (response == -1)
            QMessageBox::information(this, "-_-||", "密码错误！ \n 万能密码：123456", QMessageBox::Yes);

        if (response == 1)
            QMessageBox::information(this, "^_^", "登录成功", QMessageBox::Yes);

        if (response != 1)
            return;
    }

    // 0管理员 1顾客
    switch (userType) {
    case 0:
        this->user = factury->getUser(E_Administrator);
        break;
    case 1:
        this->user = factury->getUser(E_Guest);
        break;
    default:
        break;
    }

    QString dbUserName = ui->lineEditDbUser->text();
    QString dbName = ui->lineEditDbName->text();

    this->user->sql = sql;
    this->user->setFactury(factury);
    this->user->setContainer(container);

    this->user->connect(this->ui, dbUserName, dbName);

    this->user->userType = userType;
    this->user->userName = userName;
    this->user->setVisiabe(ui);
    this->user->showHomePage(this->ui, userName);

    this->isLogin = true;

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
    ui->stackedWidget->setCurrentWidget(ui->ConnectInfoPage);
}

// 用户登出
void MainWindow::onLogoutBtn()
{
    this->user->disConnect(this->ui);
    this->user.clear();
    this->isLogin = false;
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
    ui->workStackedWidget->setCurrentWidget(ui->welcomePage);
    ui->objectDockWidget->hide();
    ui->activeDockWidget->hide();
}

// 断开数据库连接
void MainWindow::onDisconnectBtn()
{
    this->isConnected = false;
    if (this->isLogin)
        onLogoutBtn();
    ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
}

// tableList选择tableItem显示相应的表
void MainWindow::onItemACtivated(QListWidgetItem *item)
{
    ui->workStackedWidget->setCurrentWidget(ui->baseTablePage);
    QSharedPointer<QSqlTableModel> tableModel;
    QString itemName = item->text();

    if (container->openedTableModel.find(itemName) != container->openedTableModel.end())
        tableModel = container->openedTableModel[itemName];
    else
    {
        tableModel = this->sql->getTableModel(itemName);
        container->openedTableModel[itemName] = tableModel;
    }

    container->currentTableModel = tableModel;

    // 显示表
    ui->tableView->setModel(container->currentTableModel.data());

    // 更新控件动态内容
    resetWidgetInfo(container->currentTableModel);

    // 启用编辑按钮
    ui->addAction->setEnabled(true);
    ui->deleteAction->setEnabled(true);
    ui->submitAction->setEnabled(true);
    ui->cancelAction->setEnabled(true);
    ui->editAction->setEnabled(true);

    // 设置为手动刷新，用来达到需要手动提交的效果
    container->currentTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 将表格置为不可修改
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
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
    while (container.data()->currentTableModel->canFetchMore()) {
        container.data()->currentTableModel->fetchMore();
    }

    int rowCount = container.data()->currentTableModel->rowCount();

    // 在rowCount处添加一行
    container.data()->currentTableModel->insertRow(rowCount);

    container.data()->currentTableModel->setData(container.data()->currentTableModel->index(rowCount, 0), QVariant());

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

    if (!container.data()->currentTableModel->removeRow(index.row()))
    {
        QSqlError error =  container.data()->currentTableModel->lastError();
        QMessageBox::critical(this, "删除失败！", error.nativeErrorCode()+" "+error.text());
        onCancelACtion();
        return;
    }

    // 删除前，弹框确认
    int ok = QMessageBox::warning(this, "删除当前行！", "确定删除当前行？", QMessageBox::Yes, QMessageBox::No);

    if (ok == QMessageBox::No)
    {
        container.data()->currentTableModel->revertAll();

        ui->addAction->setEnabled(true);
        ui->deleteAction->setEnabled(true);
        ui->cancelAction->setEnabled(true);
        ui->submitAction->setEnabled(true);
        ui->editAction->setEnabled(true);
        return;
    }

    // 提交
    container.data()->currentTableModel->submitAll();
    // 查询
    container.data()->currentTableModel->select();
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
    if (!container.data()->currentTableModel->submitAll())
    {
        QSqlError error =  container.data()->currentTableModel->lastError();
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
    container.data()->currentTableModel->revertAll();

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
    if (container->currentTableModel.isNull())
        return;
    QModelIndex index = this->ui->tableView->currentIndex();
    int col_index = index.column();
    container.data()->currentTableModel->sort(col_index, Qt::AscendingOrder);
}

void MainWindow::onDescAction()
{
    if (container->currentTableModel.isNull())
        return;
    QModelIndex index = this->ui->tableView->currentIndex();
    int col_index = index.column();
    container.data()->currentTableModel->sort(col_index, Qt::DescendingOrder);
}

void MainWindow::onFilterBtn()
{
    if (container->currentTableModel.isNull())
        return;
    ui->workStackedWidget->setCurrentWidget(ui->queryTablePage);
    QString fieldName = ui->comboBoxCol->currentText();
    QString value;
    bool use_customed_value = ui->checkBoxCusVal->isChecked();
    if (use_customed_value)
        value = ui->lineEditVal->text();
    else
        value = ui->comboBoxVal->currentText();

    ui->queryTableView->setModel(this->setQueryModel(this->sql->selectByValue(container->currentTableModel.data()->tableName(), fieldName, value)).data());
}

void MainWindow::onFilterAdvBtn()
{
    if (container->currentTableModel.isNull())
        return;
    ui->workStackedWidget->setCurrentWidget(ui->queryTablePage);
    QString fieldName = ui->comboBoxColAdv->currentText();
    QString expression = ui->lineEditValAdv->text();
    QString from = ui->lineEditValFromAdv->text();
    QString to = ui->lineEditValToAdv->text();

    bool use_between = ui->checkBoxAdv->isChecked();
    if (use_between)
         ui->queryTableView->setModel(this->setQueryModel(this->sql->selectByValueBetween(container->currentTableModel->tableName(), fieldName, from, to)).data());
    else
         ui->queryTableView->setModel(this->setQueryModel(this->sql->selectByValueFuzzy(container->currentTableModel->tableName(), fieldName, expression)).data());
}

void MainWindow::onFieldCurrentIndexChanged(int index)
{
    if (index == -1)
        return;
    this->ui->comboBoxVal->clear();
    QString fieldName = ui->comboBoxCol->itemText(index);
    QVector<QString> values = this->sql->getValues(container->currentTableModel->tableName(), fieldName);

    for (QString &value : values)
        this->ui->comboBoxVal->addItem(value);
}


void MainWindow::resetWidgetInfo(QSharedPointer<QSqlTableModel> currentTableModel)
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
    container->currentTableModel.clear();
    // 首次点击添加年份选项
    if (ui->comboBoxStaYear->currentIndex() == -1)
    {
        this->init = true;
        QVector<QString> years = this->sql->getYear();
        for (QString &year : years)
        {
             ui->comboBoxStaYear->addItem(year);
             ui->comboBoxStaYearM->addItem(year);
             ui->sellOpComboBoxYear->addItem(year);
        }

        this->ui->comboBoxStaMonthM->clear();
        this->ui->comboBoxStaDayM->clear();
        QString yearM = ui->comboBoxStaYearM->currentText();
        QVector<QString> monthData = this->sql->getMonth(yearM);
        for (QString & month : monthData)
        {
            this->ui->comboBoxStaMonthM->addItem(month);
            this->ui->sellOpComboBoxMonth->addItem(month);
        }

        QString monthM = ui->comboBoxStaMonthM->currentText();
        QVector<QString> dayData = this->sql->getDay(yearM, monthM);
        for (QString & day : dayData)
            this->ui->comboBoxStaDayM->addItem(day);
        this->init = false;
    }

    ui->workStackedWidget->setCurrentWidget(ui->statisticPage);
    ui->tabWidget_2->setCurrentWidget(ui->tabStaYear);

    QString year = ui->comboBoxStaYear->currentText();
    QSharedPointer<QChart> chart = container->getYearStaChartView(year);
    ui->chartView->setChart(chart.data());

    this->ui->tableViewStaMonth->setModel(this->setQueryModel(container->getMonthTableModel(ui->comboBoxStaYearM->currentText(), ui->comboBoxStaMonthM->currentText())).data());
}

void MainWindow::onComboBoxStaYearChanged(QString year)
{
    if (ui->comboBoxStaYear->currentIndex() == -1)
        return;
    if (!this->isConnected)
        return;
    if (this->init)
        return;

    ui->chartView->setChart(container->getYearStaChartView(year).data());
}

void MainWindow::onStaYearMBtn()
{
    QString yearM = ui->comboBoxStaYearM->currentText();
    QString monthM = ui->comboBoxStaMonthM->currentText();
    QString dayM = ui->comboBoxStaDayM->currentText();

    this->ui->tableViewStaMonth->setModel(this->setQueryModel(container->getDayTableModel(yearM, monthM, dayM)).data());
}

void MainWindow::onStaYearMBtn2()
{
    QString yearM = ui->comboBoxStaYearM->currentText();
    QString monthM = ui->comboBoxStaMonthM->currentText();

    this->ui->tableViewStaMonth->setModel(this->setQueryModel(container->getMonthTableModel(yearM, monthM)).data());
}

// 销售员业绩
void MainWindow::onSellOpYearBtn()
{
    QString year = ui->sellOpComboBoxYear->currentText();
    QSharedPointer<QSqlQueryModel> queryModel = sql->getSellOpYear(year);

    this->ui->sellOpTableView->setModel(this->setQueryModel(queryModel).data());
}

void MainWindow::onSellOpMonthBtn()
{
    QString year = ui->sellOpComboBoxYear->currentText();
    QString month = ui->sellOpComboBoxMonth->currentText();
    QSharedPointer<QSqlQueryModel> queryModel = sql->getSellOpMonth(year, month);

    this->ui->sellOpTableView->setModel(this->setQueryModel(queryModel).data());
}

void MainWindow::onComboBoxSaleYearChanged(QString year)
{
    if (this->ui->comboBoxSaleYear->count() == -1)
        return;
    QSharedPointer<QChart> chart = container->getYearSaleChartView(year);
    ui->chartView2->setChart(chart.data());
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

void MainWindow::onsellOpComboBoxYearChanged(QString year)
{
    if (this->ui->sellOpComboBoxYear->count() == -1)
        return;
    if (this->init)
        return;
    this->ui->sellOpComboBoxMonth->clear();

    QString year_ = ui->sellOpComboBoxYear->currentText();
    QVector<QString> monthData = this->sql->getMonth(year_);
    for (QString & month : monthData)
        this->ui->sellOpComboBoxMonth->addItem(month);
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
    container->currentTableModel.clear();
    // 首次点击添加年份选项
    if (ui->comboBoxSaleYear->currentIndex() == -1)
    {
        QVector<QString> years = this->sql->getYear();
        for (QString &year : years)
            ui->comboBoxSaleYear->addItem(year);
    }

    ui->workStackedWidget->setCurrentWidget(ui->statisticPage);
    ui->tabWidget_2->setCurrentWidget(ui->tabSaleYear);

    QString year = ui->comboBoxSaleYear->currentText();
    QSharedPointer<QChart> chart = container->getYearSaleChartView(year);
    ui->chartView2->setChart(chart.data());
}

void MainWindow::autoFillLoginInfo(QString dbType)
{
    if (dbType == "SQL Server")
    {
        ui->lineEditDbUser->setText("sa");
        ui->lineEditDbPassword->setText("123456");
        ui->lineEditDbName->setText("DB");
        ui->lineEditDbHost->setText("172.16.220.120");
        ui->lineEditDbPort->setText("1433");
    }

}

void MainWindow::onOrderAction()
{
    this->ui->workStackedWidget->setCurrentWidget(ui->guestOrderPage);
    this->ui->operatorComboBox->clear();
    this->ui->ticketTypeComboBox->clear();

    QSharedPointer<QVector<QString> > operators = sql->getOpetarot();
    for (QString & op : *(operators.data()))
        this->ui->operatorComboBox->addItem(op);

    QSharedPointer<QVector<QString> > ticketType = sql->getTicketType();
    for (QString & type : *(ticketType.data()))
        this->ui->ticketTypeComboBox->addItem(type);
}

void MainWindow::onQueryAction()
{
    this->ui->workStackedWidget->setCurrentWidget(ui->guestInfoPage);
    // 显示用户订单
    QSharedPointer<QSqlQueryModel> queryModel = sql->getCustomOrder(user->userName);
    this->setQueryModel(queryModel);
    this->ui->guestOrderTableView->setModel(queryModel.data());
}

void MainWindow::onRefundAction()
{
    this->ui->workStackedWidget->setCurrentWidget(ui->guestRefundPage);
    this->ui->guestOrderComboBox->clear();

    // 获取用户所有订单号
    QSharedPointer<QVector<QString> > customOrderId = sql->getCustomOrderId(user->userName);
    for (QString & orderId : *(customOrderId.data()))
        this->ui->guestOrderComboBox->addItem(orderId);
}

void MainWindow::FUCK()
{
    QMessageBox::warning(this, "FUCK!", "请联系管理员实现该功能（不一定会实现）^_^", QMessageBox::Yes);
}

void MainWindow::THANKS()
{
    QString text = QString("鸣谢：\n \n HMY\n 啵啵鱼🐟\n 你妈\n 罗老师\n \n GitHub: https://github.com/leafhmy/Qt6_Database_Application.git \n \n Copyright: MJUAI19");
    QMessageBox::information(this, "^_^ 鸣谢 ^_^", text, QMessageBox::Yes);
}

// 行为界面注册按钮
void MainWindow::onRgBtn()
{
    if (ui->comboBoxIdt->currentText() == "管理员")
        return;
    else
        ui->workStackedWidget->setCurrentWidget(ui->guestRegisterPage);
}

// 注册界面注册按钮
void MainWindow::onGuestRgBtn()
{
    QString cardNumber = ui->RgCardLineEdit->text();
    QString customName = ui->RgNameLineEdit->text();
    QString phoneNumber = ui->RgPhoneLineEdit->text();
    QString age = ui->RgAgeLineEdit->text();
    QString gender = ui->RgGenderComboBox->currentText();
    QString cumstomType = ui->RgUserTypeComboBox->currentText();
    QString password = ui->RgPasswordLineEdit->text();

    bool ok = this->sql->registerCumstom(cardNumber, password, customName, phoneNumber, age, gender, cumstomType);

    if (ok)
        QMessageBox::information(this, "^_^", "注册成功！", QMessageBox::Yes);
    else
        QMessageBox::information(this, "-_-||", "注册失败！", QMessageBox::Yes);

}

void MainWindow::onOrderTicketBtn()
{
    QString card_number = this->user->userName; // 卡号作为用户名
    QString sell_operator = this->ui->operatorComboBox->currentText();
    QString ticket_type = this->ui->ticketTypeComboBox->currentText();
    QString orderFor = this->ui->orderForLineEdit->text();
    bool ok = this->sql->orderTicket(card_number, sell_operator, ticket_type, orderFor);

    if (ok)
        QMessageBox::information(this, "^_^", "订票成功！", QMessageBox::Yes);
    else
        QMessageBox::information(this, "-_-||", "订票失败！", QMessageBox::Yes);
}

void MainWindow::onRefundBtn()
{
    QString orderId = ui->guestOrderComboBox->currentText();
    bool ok = sql->refund(user->userName, orderId);

    if (ok)
        QMessageBox::information(this, "^_^", "退票成功！", QMessageBox::Yes);
    else
        QMessageBox::information(this, "-_-||", "退票失败！", QMessageBox::Yes);
}


