#ifndef FACTURY_H
#define FACTURY_H

#include <QSharedPointer>
#include <qsqlquerymodel.h>
#include <QSqlTableModel>
#include <QChart>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QLineSeries>
#include <QValueAxis>

class Container;
class User;
class SQLHelper;
class Administrator;
class Guest;

enum UserType{
    E_Administrator,
    E_Guest,
};


class Factury
{
public:
    Factury();

    QSharedPointer<QSqlTableModel> getQSqlTableModel(QSqlDatabase db);

    QSharedPointer<QSqlQueryModel> getQSqlQueryModel();

    QSharedPointer<QBarSeries> getQBarSeries();

    QSharedPointer<QBarCategoryAxis> getQBarCategoryAxis();

    QSharedPointer<QBarSet> getQBarSet(QString label);

    QSharedPointer<QChart> getQChart();

    QSharedPointer<QLineSeries> getQLineSeries();

    QSharedPointer<QValueAxis> getQValueAxis();

    QSharedPointer<User> getUser(UserType);

    QSharedPointer<SQLHelper> getSQLHelper(Factury* factury);

    QSharedPointer<Container> getContainer(Factury* factury, QSharedPointer<SQLHelper> sql);

    QSharedPointer<QVector<QString>> getQVectorQString();

};

#endif // FACTURY_H
