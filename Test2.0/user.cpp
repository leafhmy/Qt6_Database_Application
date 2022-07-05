#include "user.h"

User::User()
{
    isConnected = false;
}

User::~User()
{

}

void User::showHomePage(Ui::MainWindow* ui, QString userName)
{
    ui->userLabel->setText("欢迎您" + userName);
    ui->stackedWidget->setCurrentWidget(ui->UserInfoPage);
}

void User::getTables()
{
    this->tables = this->sql->getTables();
}

void User::setFactury(Factury* factury)
{
    this->factury = factury;
}

void User::setContainer(QSharedPointer<Container> container)
{
    this->container = container;
}

//////////////////////////////////////////////////////////////////

Administrator::Administrator()
{

}

Administrator::~Administrator()
{

}

void Administrator::InitData()
{
    getTables();
}

void Administrator::setVisiabe(Ui::MainWindow* ui)
{
    ui->behaviorDockWidget->show();
    ui->objectDockWidget->show();
    ui->activeDockWidget->show();

    if (isConnected)
    {
        ui->addAction->setEnabled(true);
        ui->deleteAction->setEnabled(true);
        ui->submitAction->setEnabled(true);
        ui->cancelAction->setEnabled(true);
        ui->editAction->setEnabled(true);

        ui->ascAction->setEnabled(true);
        ui->descAction->setEnabled(true);
        ui->filterAction->setEnabled(true);

        ui->sellAction->setEnabled(true);
        ui->fundAction->setEnabled(true);
    }

    ui->orderAction->setDisabled(true);
    ui->queryAction->setDisabled(true);
    ui->refundAction->setDisabled(true);
}

void Administrator::connect(Ui::MainWindow* ui, QString dbUserName, QString dbName)
{
    ui->dbUserLabel->setText("用户：" + dbUserName);
    ui->dbNameLabel->setText("数据库：" + dbName);
    ui->stackedWidget->setCurrentWidget(ui->ConnectInfoPage);
    this->InitData();
    // 添加所有可见表名到tableList
    for (QString &table : *(this->tables.data()))
    {
        QListWidgetItem* item = new QListWidgetItem(table);
        ui->tableList->addItem(item);
    }

    ui->sellAction->setEnabled(true);
    ui->fundAction->setEnabled(true);

    ui->addAction->setEnabled(true);
    ui->deleteAction->setEnabled(true);
    ui->submitAction->setEnabled(true);
    ui->cancelAction->setEnabled(true);
    ui->editAction->setEnabled(true);

    ui->ascAction->setEnabled(true);
    ui->descAction->setEnabled(true);
    ui->filterAction->setEnabled(true);
}

void Administrator::disConnect(Ui::MainWindow* ui)
{
    ui->comboBoxStaYear->clear();
    ui->comboBoxSaleYear->clear();
    ui->comboBoxStaYearM->clear();
    ui->comboBoxStaMonthM->clear();
    ui->comboBoxStaDayM->clear();
    ui->sellOpComboBoxYear->clear();
    ui->sellOpComboBoxMonth->clear();


    ui->tableList->clear();

    ui->comboBoxCol->clear();
    ui->comboBoxColAdv->clear();
    ui->comboBoxVal->clear();
    ui->lineEditVal->clear();
    ui->lineEditValAdv->clear();
    ui->lineEditValFromAdv->clear();
    ui->lineEditValToAdv->clear();
    ui->checkBoxAdv->setCheckState(Qt::Unchecked);
    ui->checkBoxCusVal->setCheckState(Qt::Unchecked);

    ui->addAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->submitAction->setDisabled(true);
    ui->cancelAction->setDisabled(true);
    ui->editAction->setDisabled(true);

    ui->ascAction->setDisabled(true);
    ui->descAction->setDisabled(true);
    ui->filterAction->setDisabled(true);

    ui->sellAction->setDisabled(true);
    ui->fundAction->setDisabled(true);

    this->container->clear();

    ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
    ui->workStackedWidget->setCurrentWidget(ui->welcomePage);
}

//////////////////////////////////////////////////////////////////

Guest::Guest()
{

}

Guest::~Guest()
{

}

void Guest::InitData()
{
//    getTables();
}

void Guest::setVisiabe(Ui::MainWindow* ui)
{
    ui->behaviorDockWidget->show();
    ui->objectDockWidget->hide();
    ui->activeDockWidget->hide();

    ui->addAction->setDisabled(true);
    ui->deleteAction->setDisabled(true);
    ui->submitAction->setDisabled(true);
    ui->cancelAction->setDisabled(true);
    ui->editAction->setDisabled(true);

    ui->sellAction->setDisabled(true);
    ui->fundAction->setDisabled(true);
;

    if (isConnected)
    {
        ui->orderAction->setEnabled(true);
        ui->queryAction->setEnabled(true);
        ui->refundAction->setEnabled(true);
    }

    ui->ascAction->setDisabled(true);
    ui->descAction->setDisabled(true);
    ui->filterAction->setDisabled(true);
}

void Guest::connect(Ui::MainWindow* ui, QString dbUserName, QString dbName)
{
    ui->orderAction->setEnabled(true);
    ui->queryAction->setEnabled(true);
    ui->refundAction->setEnabled(true);
}

void Guest::disConnect(Ui::MainWindow* ui)
{
    ui->orderAction->setDisabled(true);
    ui->queryAction->setDisabled(true);
    ui->refundAction->setDisabled(true);

    this->container->clear();

    ui->stackedWidget->setCurrentWidget(ui->ConnectPage);
    ui->workStackedWidget->setCurrentWidget(ui->welcomePage);
}
