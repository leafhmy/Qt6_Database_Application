#include "container.h"

Container::Container(Factury* factury, QSharedPointer<SQLHelper> sql)
{
    this->factury = factury;
    this->sql = sql;
}

Container::~Container()
{

}


void Container::clear()
{
    for (QSharedPointer<QSqlTableModel> table : openedTableModel)
        table.clear();
    openedTableModel.clear();

    for (QSharedPointer<QChart> chart : openedChart)
        chart.clear();
    openedChart.clear();

    queryModel.clear();

    currentTableModel.clear();
}

QSharedPointer<QSqlTableModel> Container::getTableModel(QString tableName)
{
    if (openedTableModel.find(tableName) == openedTableModel.end())
        openedTableModel[tableName] = this->sql->getTableModel(tableName);

    return openedTableModel[tableName];

}

// 统计一年中每个月各个类型的票的销售情况
QSharedPointer<QChart> Container::__getYearStaChartView(QString year)
{
    QVector<QString> monthes = this->sql->getMonth(year);

    // 设置柱状集
    QBarSeries* series = new QBarSeries();

    // 为图表添加坐标轴内容
    QStringList categories;
    QBarCategoryAxis* axis = new QBarCategoryAxis();

    QMap<QString, QBarSet*> type2set;

    for (QString & month : monthes)
    {
        categories.append(month+"月份");
        QString cmd = QString("exec get_month_kind_sales '%1/%2';").arg(year).arg(month);
        QSqlQuery query(cmd, this->sql->db);

        while (query.next())
        {
            QString Ticket_type = query.value("Ticket_type_name").toString();
            qDebug() << Ticket_type << "!";
            QString kind_sell_amount = query.value("kind_sell_amount").toString();
            if (type2set.find(Ticket_type) == type2set.end())
                type2set[Ticket_type] = new QBarSet(Ticket_type);
            type2set[Ticket_type]->append(kind_sell_amount.toInt());
        }
    }

    for (QBarSet* set : type2set)
        series->append(set);

    // 添加图表
    QSharedPointer<QChart> chart = factury->getQChart();
    chart->addSeries(series);
    chart->setTitle("销售情况一览"); // 设置图表的标题
    chart->setAnimationOptions(QChart::SeriesAnimations);

    axis->append(categories);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}

QSharedPointer<QChart> Container::getYearStaChartView(QString year)
{
    QString tagName = "year_sta_" + year;
    if (openedChart.find(tagName) == openedChart.end())
        openedChart[tagName] = this->__getYearStaChartView(year);

    return openedChart[tagName];

}

// 统计一年中每个月各个类型的票的销售金额
QSharedPointer<QChart> Container::__getYearSaleChartView(QString year)
{
    QVector<QString> monthes = this->sql->getMonth(year);

    QMap<QString, QLineSeries*> type2series;

    double max_kind_sell_money = 0;
    int end_month = 0;

    for (QString & month : monthes)
    {
        QString cmd = QString("exec get_month_kind_sales '%1/%2';").arg(year).arg(month);
        QSqlQuery query(cmd, this->sql->db);

        while (query.next())
        {
            QString Ticket_type = query.value("Ticket_type_name").toString();
            double kind_sell_money = query.value("kind_sell_money").toDouble();

            if (kind_sell_money > max_kind_sell_money)
                max_kind_sell_money = kind_sell_money;

            if (type2series.find(Ticket_type) == type2series.end())
            {
                QLineSeries* s = new QLineSeries();
                s->setName(Ticket_type);
                type2series[Ticket_type] = s;
            }

            int month_ = month.toInt();
            if (month_ > end_month)
                end_month = month_;

            type2series[Ticket_type]->append(month_, kind_sell_money);

        }
    }

    QSharedPointer<QChart> chart = factury->getQChart();
    chart->setTitle("销售金额一览");

    QValueAxis* axisX = new QValueAxis;
    axisX->setTitleText("月份"); //设置标题
    axisX->setRange(1, end_month); //设置范围
    axisX->setTickCount(end_month);      //设置主刻度个数
    axisX->setLineVisible(true); //设置轴线和刻度线可见性

    QValueAxis* axisY = new QValueAxis;
    axisY->setTitleText("销售总额"); //设置标题
    axisY->setRange(0, max_kind_sell_money); //设置范围
    axisY->setTickCount(10);      //设置主刻度个数
    axisY->setLineVisible(true); //设置轴线和刻度线可见性

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // chart->createDefaultAxes();
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    for (QLineSeries* s : type2series)
    {
        chart->addSeries(s);
        s->attachAxis(axisX);
        s->attachAxis(axisY);
    }

    return chart;
}

QSharedPointer<QChart> Container::getYearSaleChartView(QString year)
{
    QString tagName = "year_sale_" + year;
    if (openedChart.find(tagName) == openedChart.end())
        openedChart[tagName] = this->__getYearSaleChartView(year);

    return openedChart[tagName];
}

// 统计一个月的销售情况
QSharedPointer<QSqlQueryModel> Container::getMonthTableModel(QString year, QString month)
{
    QSharedPointer<QSqlQueryModel> queryModel = this->factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("exec get_date_kind_sales '%1/%2';").arg(year).arg(month),
                sql->db);
    return queryModel;
}

// 统计某一天的销售情况
QSharedPointer<QSqlQueryModel> Container::getDayTableModel(QString year, QString month, QString day)
{
    QSharedPointer<QSqlQueryModel> queryModel = this->factury->getQSqlQueryModel();
    queryModel->setQuery(
                QString("exec get_date_day_kind_sales '%1/%2/%3';").arg(year).arg(month).arg(day),
                sql->db);
    return queryModel;
}

