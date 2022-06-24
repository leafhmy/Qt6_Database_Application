#include "sqlhelper.h"
#include "qwidget.h"

SQLHelper::SQLHelper()
{

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

QSqlTableModel* SQLHelper::getTableModel(QString tableName)
{
    QSqlTableModel* tableModel = new QSqlTableModel(nullptr, db);
    tableModel->setTable(tableName);
    tableModel->select();
    return tableModel;
}

QVector<QString> SQLHelper::getFields(QSqlTableModel* tableModel)
{
    QVector<QString> fields;
    QSqlRecord record = tableModel->record();
    int field_count = record.count();
    for (int i=0; i<field_count; i++)
        fields.append(record.fieldName(i));

    return fields;
}

QVector<QString>* SQLHelper::getTables()
{
    QVector<QString>* tables = new QVector<QString>();
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

QSqlQueryModel* SQLHelper::selectByValue(QString tableName, QString fieldName, QString value)
{
    QSqlQueryModel* queryModel = new QSqlQueryModel();
    queryModel->setQuery(
                QString("select * from %1 where %2 = '%3';").arg(tableName).arg(fieldName).arg(value),
                db);
    return queryModel;
}

QMap<QString, QVector<QString>> SQLHelper::getDataTime()
{
    QMap<QString, QVector<QString>> dataTime;
    QString cmd = "select distinct Sell_month from Ticket_summary;";
    QSqlQuery query(cmd, this->db);
    while (query.next())
    {
        QString Sell_month = query.value(0).toString();
        QStringList year_month = Sell_month.split('/');
        QString year = year_month[0];
        QString month = year_month[1];
        if (dataTime.find(year) == dataTime.end())
            dataTime[year] = QVector<QString>();
        dataTime[year].append(month);
    }
    return dataTime;
}

QChart* SQLHelper::getYearChartView(QString year)
{
    QMap<QString, QVector<QString>> dataTime = this->getDataTime();
    QVector<QString> monthes = dataTime[year];

    // 设置柱状集
    QBarSeries* series = new QBarSeries();

    // 为图表添加坐标轴内容
    QStringList categories;
    QBarCategoryAxis* axis = new QBarCategoryAxis();

    bool init = false;

    QMap<QString, QBarSet*> type2set;

    for (QString & month : monthes)
    {
        categories.append(month+"月份");
        QString cmd = QString("exec get_month_kind_sales '%1/%2';").arg(year).arg(month);
        QSqlQuery query(cmd, this->db);

        while (query.next())
        {
            QString Ticket_type = query.value("Ticket_type_name").toString();
            qDebug() << Ticket_type << "!";
            QString kind_sell_amount = query.value("kind_sell_amount").toString();
//            if (!init)
//                type2set[Ticket_type] = new QBarSet(Ticket_type);
            if (type2set.find(Ticket_type) == type2set.end())
                type2set[Ticket_type] = new QBarSet(Ticket_type);
            type2set[Ticket_type]->append(kind_sell_amount.toInt());
        }
        init = true;
    }

    for (QBarSet* set : type2set)
        series->append(set);

    // 添加图表
    QChart *chart = new QChart();
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

QChart* SQLHelper::getSaleChartView(QString year)
{
    QMap<QString, QVector<QString>> dataTime = this->getDataTime();
    QVector<QString> monthes = dataTime[year];

    QMap<QString, QLineSeries*> type2series;

    bool init = false;

    double max_kind_sell_money = 0;
    int end_month = 0;

    for (QString & month : monthes)
    {
        QString cmd = QString("exec get_month_kind_sales '%1/%2';").arg(year).arg(month);
        QSqlQuery query(cmd, this->db);

        while (query.next())
        {
            QString Ticket_type = query.value("Ticket_type_name").toString();
            double kind_sell_money = query.value("kind_sell_money").toDouble();

            if (kind_sell_money > max_kind_sell_money)
                max_kind_sell_money = kind_sell_money;

//            if (!init)
//            {
//                QLineSeries* s = new QLineSeries();
//                s->setName(Ticket_type);
//                type2series[Ticket_type] = s;
//            }
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
        init = true;
    }

    QChart *chart = new QChart;
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
