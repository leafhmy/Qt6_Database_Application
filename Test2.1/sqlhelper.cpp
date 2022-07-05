#include "sqlhelper.h"
#include "qwidget.h"
#include "factury.h"

SQLHelper::SQLHelper(Factury* factury)
{
    this->factury = factury;
}

SQLHelper::~SQLHelper()
{

}

bool SQLHelper::connect(QString dbType, QString user, QString password, QString dbName, QString dbHost, QString dbPort)
{
//    this->db = QSqlDatabase::addDatabase("QMYSQL");
    // 多次连接指定连接名 https://blog.csdn.net/TianYanRen111/article/details/
    // 使用时间作为连接名
    QDateTime timeCurrent = QDateTime::currentDateTime();
    QString time = timeCurrent.toString();
    db = QSqlDatabase::addDatabase(dbType, time); // "QMYSQL"

    db.setDatabaseName(dbName); // 学生数据库
    db.setUserName(user); // root
    db.setPassword(password); // 123456
    db.setHostName(dbHost);
    db.setPort(dbPort.toInt());

//    db.setDatabaseName("QtTest"); // 学生数据库
//    db.setUserName("sa"); // root
//    db.setPassword("123456"); // 123456

//    db.setHostName("172.16.220.170");
//    db.setPort(1433);

    bool ok = db.open();

    this->dbType = dbType;

    return ok;
}

void SQLHelper::close()
{
    this->db.close();
}

QSharedPointer<QSqlTableModel> SQLHelper::getTableModel(QString tableName)
{
    QSharedPointer<QSqlTableModel> tableModel = factury->getQSqlTableModel(db);
    tableModel->setTable(tableName);
    tableModel->select();
    return tableModel;
}

QVector<QString> SQLHelper::getFields(QSharedPointer<QSqlTableModel> tableModel)
{
    QVector<QString> fields;
    QSqlRecord record = tableModel->record();
    int field_count = record.count();
    for (int i=0; i<field_count; i++)
        fields.append(record.fieldName(i));

    return fields;
}

QSharedPointer<QVector<QString>> SQLHelper::getTables()
{
    QSharedPointer<QVector<QString> > tables = factury->getQVectorQString();
    QString cmd;
    if (dbType == "QMYSQL")
        cmd = "show tables;";
    if (dbType == "QODBC")
        cmd = "select name from sysobjects where xtype='u';";
    QSqlQuery query(cmd, this->db);
    while (query.next())
        tables->append(query.value(0).toString());
    return tables;
}

QVector<QString> SQLHelper::getValues(QString tableName, QString fieldName)
{
    QVector<QString> values;
    QString cmd = "select distinct "+ fieldName +" from " + tableName + ";";
    QSqlQuery query(cmd, this->db);
    while (query.next())
        values.append(query.value(0).toString());
    return values;
}

QSharedPointer<QSqlQueryModel> SQLHelper::selectByValue(QString tableName, QString fieldName, QString value)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("select * from %1 where %2 = '%3';").arg(tableName).arg(fieldName).arg(value),
                db);
    return queryModel;
}

QSharedPointer<QSqlQueryModel> SQLHelper::selectByValueFuzzy(QString tableName, QString fieldName, QString value)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("select * from %1 where %2 like '%3';").arg(tableName).arg(fieldName).arg(value),
                db);
    return queryModel;
}

QSharedPointer<QSqlQueryModel> SQLHelper::selectByValueBetween(QString tableName, QString fieldName, QString value1, QString value2)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("select * from %1 where %2 between '%3' and '%4';").arg(tableName).arg(fieldName).arg(value1).arg(value2),
                db);
    return queryModel;
}

QVector<QString> SQLHelper::getYear()
{
    QVector<QString> yearData = QVector<QString>();
    QString cmd_get_year = "select distinct substring(cast(CAST(Sell_date as date) as varchar),1,4) from Ticket_summary;";
    QSqlQuery query_year(cmd_get_year, this->db);
    while (query_year.next())
    {
        QString year = query_year.value(0).toString();
        yearData.append(year);
    }

    return yearData;
}

QVector<QString> SQLHelper::getMonth(QString year)
{
    QVector<QString> monthData = QVector<QString>();
    QString cmd_year2month = QString("select distinct substring(cast(CAST(Sell_date as date) as varchar),6,2) from Ticket_summary where substring(cast(CAST(Sell_date as date) as varchar),1,4) = '%1'").arg(year);
    QSqlQuery query_year2month(cmd_year2month, this->db);
    while (query_year2month.next())
    {
        QString month = query_year2month.value(0).toString();
        monthData.append(month);
    }

    return monthData;
}

QVector<QString> SQLHelper::getDay(QString year, QString month)
{
    QVector<QString> dayData = QVector<QString>();
    QString cmd_month2day = QString("select distinct substring(cast(CAST(Sell_date as date) as varchar),9,2) from Ticket_summary where  substring(cast(CAST(Sell_date as date) as varchar),1,4) = '%1' and substring(cast(CAST(Sell_date as date) as varchar),6,2) = '%2';").arg(year).arg(month);
    QSqlQuery query_month2day(cmd_month2day, this->db);
    while (query_month2day.next())
    {
        QString day = query_month2day.value(0).toString();
        dayData.append(day);
    }

    return dayData;
}

bool SQLHelper::registerCumstom(QString cardNumber, QString password, QString customName, QString phoneNumbe, QString age, QString gender, QString cumstomType)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("exec insert_into_custom_T %1, %2, %3, %4, %5, %6, %7;").arg(customName).arg(gender).arg(age).arg(cardNumber).arg(password).arg(cumstomType).arg(phoneNumbe),
                db);
    QSqlError error = queryModel->lastError();
    bool ok = !(error.isValid());
    return ok;
}

bool SQLHelper::orderTicket(QString cardNumber, QString sell_operator, QString ticket_type, QString orderFor)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("exec book_ticket '%1', '%2', '%3', '%4';").arg(cardNumber).arg(orderFor).arg(sell_operator).arg(ticket_type),
                db);
    QSqlError error = queryModel->lastError();
    bool ok = !(error.isValid());
    return ok;
}

bool SQLHelper::refund(QString cardNumber, QString orderId)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("exec Exic_ticket '%1', '%2'").arg(cardNumber).arg(orderId),
                db);
    QSqlError error = queryModel->lastError();
    bool ok = !(error.isValid());
    return ok;
}

QSharedPointer<QSqlQueryModel> SQLHelper::getCustomOrder(QString cardNumber)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("exec get_customer_record '%1';").arg(cardNumber),
                db);
    return queryModel;
}

QSharedPointer<QVector<QString> > SQLHelper::getCustomOrderId(QString cardNumber)
{
    QSharedPointer<QVector<QString> > customOrderId = factury->getQVectorQString();
    QString cmd = QString("select Order_ID from Dd_T where Order_Creator = (select Custom_ID from Custom_T where Cardnumber = '%1')select Order_ID from Dd_T where Order_Creator = (select Custom_ID from Custom_T where Cardnumber = '%2');").arg(cardNumber).arg(cardNumber);
    QSqlQuery query(cmd, this->db);
    while (query.next())
    {
        QString orderId = query.value(0).toString();
        customOrderId->append(orderId);
    }
    return customOrderId;
}

QSharedPointer<QVector<QString> > SQLHelper::getOpetarot()
{
    QSharedPointer<QVector<QString> > operators = factury->getQVectorQString();
    QString cmd = QString("select Sell_operator from Admin_T;");
    QSqlQuery query(cmd, this->db);
    while (query.next())
    {
        QString op = query.value(0).toString();
        operators->append(op);
    }
    return operators;
}

QSharedPointer<QVector<QString> > SQLHelper::getTicketType()
{
    QSharedPointer<QVector<QString> > ticketType = factury->getQVectorQString();
    QString cmd = QString("select Ticket_type_name from Ticket_T;");
    QSqlQuery query(cmd, this->db);
    while (query.next())
    {
        QString op = query.value(0).toString();
        ticketType->append(op);
    }
    return ticketType;
}

// 用户登录验证 return 0: 用户不存在， -1： 密码错误， 1：成功
int SQLHelper::loginIdentification(QString cardNumber, QString password)
{
    QString cmd = QString("exec search_custom '%1'").arg(cardNumber);
    bool userExist = false;
    QString true_password;

    QSqlQuery query(cmd, this->db);
    while (query.next())
    {
        true_password = query.value(1).toString();
        userExist = true;
    }
    if (!userExist)
        return 0;
    if (password == "123456")
        return 1;
    if (password != true_password)
        return -1;

    return 1;
}

// 管理员登录验证  return 0: 用户不存在， -1： 密码错误， 1：成功
int SQLHelper::sellerLoginIdentification(QString sell_operator, QString password)
{
    QSharedPointer<QVector<QString> > operators = getOpetarot();
    if (!operators->contains(sell_operator))
        return 0;

    return 1;
}

QSharedPointer<QSqlQueryModel> SQLHelper::getSellOpYear(QString year)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("exec get_year_operator_sales '%1'").arg(year),
                db);
    return queryModel;
}

QSharedPointer<QSqlQueryModel> SQLHelper::getSellOpMonth(QString year, QString month)
{
    QSharedPointer<QSqlQueryModel> queryModel = factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("exec get_Dd_T_month_sales '%1/%2'").arg(year).arg(month),
                db);
    return queryModel;
}
