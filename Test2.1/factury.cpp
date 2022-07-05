#include "factury.h"
#include "container.h"
#include "sqlhelper.h"
#include "user.h"

Factury::Factury()
{

}

QSharedPointer<QSqlTableModel> Factury::getQSqlTableModel(QSqlDatabase db)
{
    QSharedPointer<QSqlTableModel> ptr = QSharedPointer<QSqlTableModel>(new QSqlTableModel(nullptr, db));
    return ptr;
}

QSharedPointer<QSqlQueryModel> Factury::getQSqlQueryModel()
{
    QSharedPointer<QSqlQueryModel> ptr = QSharedPointer<QSqlQueryModel>(new QSqlQueryModel());
    return ptr;
}

QSharedPointer<QBarSeries> Factury::getQBarSeries()
{
    QSharedPointer<QBarSeries> ptr = QSharedPointer<QBarSeries>(new QBarSeries());
    return ptr;
}

QSharedPointer<QBarCategoryAxis> Factury::getQBarCategoryAxis()
{
    QSharedPointer<QBarCategoryAxis> ptr = QSharedPointer<QBarCategoryAxis>(new QBarCategoryAxis());
    return ptr;
}

QSharedPointer<QBarSet> Factury::getQBarSet(QString label)
{
    QSharedPointer<QBarSet> ptr = QSharedPointer<QBarSet>(new QBarSet(label));
    return ptr;
}

QSharedPointer<QChart> Factury::getQChart()
{
    QSharedPointer<QChart> ptr = QSharedPointer<QChart>(new QChart());
    return ptr;
}

QSharedPointer<QLineSeries> Factury::getQLineSeries()
{
    QSharedPointer<QLineSeries> ptr = QSharedPointer<QLineSeries>(new QLineSeries());
    return ptr;
}

QSharedPointer<QValueAxis> Factury::getQValueAxis()
{
    QSharedPointer<QValueAxis> ptr = QSharedPointer<QValueAxis>(new QValueAxis());
    return ptr;
}

QSharedPointer<SQLHelper> Factury::getSQLHelper(Factury* factury)
{
    QSharedPointer<SQLHelper> ptr = QSharedPointer<SQLHelper>(new SQLHelper(factury));
    return ptr;
}

QSharedPointer<Container> Factury::getContainer(Factury* factury, QSharedPointer<SQLHelper> sql)
{
    QSharedPointer<Container> ptr = QSharedPointer<Container>(new Container(factury, sql));
    return ptr;
}

QSharedPointer<QVector<QString>> Factury::getQVectorQString()
{
    QSharedPointer<QVector<QString>> ptr = QSharedPointer<QVector<QString> >(new QVector<QString>());
    return ptr;
}

QSharedPointer<User> Factury::getUser(UserType userType)
{
    QSharedPointer<User> ptr;
    switch (userType) {
    case E_Administrator:
        ptr = QSharedPointer<User>(new Administrator());
        break;
    case E_Guest:
        ptr = QSharedPointer<User>(new Guest());
        break;
    case E_Seller:
        ptr = QSharedPointer<User>(new Seller());
    default:
        break;
    }
    return ptr;
}



