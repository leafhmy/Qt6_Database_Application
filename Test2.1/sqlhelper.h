#ifndef SQLHELPER_H
#define SQLHELPER_H


#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <qsqlquerymodel.h>
#include <QSqlTableModel>
#include <QDateTime>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QLineSeries>
#include <QValueAxis>
#include "factury.h"
#include <QSqlError>


class SQLHelper : public QObject
{
    Q_OBJECT

public:

    SQLHelper(Factury* factury);

    ~SQLHelper();

    Factury* factury;

    bool connect(QString dbType, QString user, QString password, QString dbName, QString dbHost, QString dbPort);

    void close();

    // 顾客注册
    bool registerCumstom(QString cardNumber, QString password, QString customName, QString phoneNumbe, QString age, QString gender, QString cumstomType);

    // 订票
    bool orderTicket(QString cardNumber, QString sell_operator, QString ticket_type, QString orderFor);

    // 退票
    bool refund(QString cardNumber, QString orderId);

    // 用户登录验证
    int loginIdentification(QString cardNumber, QString password);

    // 管理员登录验证
    int sellerLoginIdentification(QString sell_operator, QString password);

    QSharedPointer<QVector<QString> > getOpetarot();

    QSharedPointer<QVector<QString> > getTicketType();

    QSharedPointer<QSqlQueryModel> getCustomOrder(QString cardNumber);

    QSharedPointer<QSqlQueryModel> getSellOpYear(QString year);

    QSharedPointer<QSqlQueryModel> getSellOpMonth(QString year, QString month);

    QSharedPointer<QVector<QString> > getCustomOrderId(QString cardNumber);

    // 根据table名称获取tableModel
    QSharedPointer<QSqlTableModel> getTableModel(QString tableName);

    // select * from tableName where fieldName = value;
    QSharedPointer<QSqlQueryModel> selectByValue(QString tableName, QString fieldName, QString value);

    // select * from tableName where fieldName like value;
    QSharedPointer<QSqlQueryModel> selectByValueFuzzy(QString tableName, QString fieldName, QString value);

    // select * from tableName where fieldName between value1 and value2;
    QSharedPointer<QSqlQueryModel> selectByValueBetween(QString tableName, QString fieldName, QString value1, QString value2);

    // 获取字段
    QVector<QString> getFields(QSharedPointer<QSqlTableModel> tableModel);

    // 获取字段下的值
    QVector<QString> getValues(QString tableName, QString fieldName);

    // 获取表名称
    QSharedPointer<QVector<QString>> getTables();

    QVector<QString> getYear();

    QVector<QString> getMonth(QString year);

    QVector<QString> getDay(QString year, QString month);

    QSqlDatabase db;

    QString dbType;

};

#endif // SQLHELPER_H
