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

class SQLHelper : public QObject
{
    Q_OBJECT

public:

    SQLHelper();

    ~SQLHelper();

    bool connect(QString dbType, QString user, QString password, QString dbName, QString dbHost, QString dbPort);

    void close();

    // 根据table名称获取tableModel
    QSqlTableModel* getTableModel(QString tableName);

    // select * from tableName where fieldName = value;
    QSqlQueryModel* selectByValue(QString tableName, QString fieldName, QString value);

    // select * from tableName where fieldName like value;
    QSqlQueryModel* selectByValueFuzzy(QString tableName, QString fieldName, QString value);

    // select * from tableName where fieldName between value1 and value2;
    QSqlQueryModel* selectByValueBetween(QString tableName, QString fieldName, QString value1, QString value2);

    // 获取字段
    QVector<QString> getFields(QSqlTableModel* tableModel);

    // 获取字段下的值
    QVector<QString> getValues(QString tableName, QString fieldName);

    // 获取表名称
    QVector<QString>* getTables();

    // 统计一年中每个月各个类型的票的销售情况
    QChart* getYearChartView(QString year);

    // 统计一年中每个月各个类型的票的销售金额
    QChart* getSaleChartView(QString year);

    // 统计一个月的销售情况
    QSqlQueryModel* getMonthTableModel(QString year, QString month);

    // 统计某一天的销售情况
    QSqlQueryModel* getDayTableModel(QString year, QString month, QString day);

    QMap<QString, QVector<QString>> getDataTime();

    QVector<QString> getYear();

    QVector<QString> getMonth(QString year);

    QVector<QString> getDay(QString year, QString month);

    QSqlDatabase db;

    QString dbType;

};

#endif // SQLHELPER_H
