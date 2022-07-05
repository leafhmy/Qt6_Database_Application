#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>
#include <QSharedPointer>
#include "sqlhelper.h"
#include "factury.h"

class Container : public QObject
{
    Q_OBJECT
public:
    Factury* factury;

    QSharedPointer<SQLHelper> sql;

    Container(Factury* factury, QSharedPointer<SQLHelper> sql);

    ~Container();

    void clear();

    // 记录打开的tableodel
    QMap<QString, QSharedPointer<QSqlTableModel> > openedTableModel;

    // 记录打开的Chart
    QMap<QString, QSharedPointer<QChart> > openedChart;

    // 查询model，需要及时清除
    QSharedPointer<QSqlQueryModel> queryModel;

    // 当前TableView显示的tableModel
    QSharedPointer<QSqlTableModel> currentTableModel;

    // 获取tableModel
    QSharedPointer<QSqlTableModel> getTableModel(QString tableName);

    // 统计一年中每个月各个类型的票的销售情况
    QSharedPointer<QChart> getYearStaChartView(QString year);

    // 统计一年中每个月各个类型的票的销售金额
    QSharedPointer<QChart> getYearSaleChartView(QString year);

    // 统计一个月的销售情况
    QSharedPointer<QSqlQueryModel> getMonthTableModel(QString year, QString month);

    // 统计某一天的销售情况
    QSharedPointer<QSqlQueryModel> getDayTableModel(QString year, QString month, QString day);


private:

    QSharedPointer<QChart> __getYearStaChartView(QString year);

    QSharedPointer<QChart> __getYearSaleChartView(QString year);


};

#endif // CONTAINER_H
