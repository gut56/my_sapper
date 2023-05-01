#include "recordsdao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>

RecordsDao::RecordsDao(QSqlDatabase& database):mDatabase(database)
{
}

void RecordsDao::init() const
{
    if (!mDatabase.tables().contains("records")) {
        QSqlQuery query(mDatabase);
        auto r = query.exec("CREATE TABLE records (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "player TEXT NOT NULL,"
                            "time_game_sec INTEGER  default 0,"
                            "size_x INTEGER  default 0,"
                            "size_y INTEGER  default 0,"
                            "num_mine INTEGER  default 0,"
                            "date DATETIME)");
        qDebug() << "create table records" << r;
    }
}

void RecordsDao::addRecord(const QString& namePlayer, int seconds, int s_x, int s_y,int num_mine) const
{
    QSqlQuery q;
    QString  strq = "INSERT INTO records (player,time_game_sec,size_x,size_y,num_mine,date) values ('%1',%2,%3,%4,%5,'%6')";
    strq = strq.arg(namePlayer)
            .arg(seconds)
            .arg(s_x)
            .arg(s_y)
            .arg(num_mine)
            .arg(QDateTime::currentDateTime().toString(Qt::ISODate));

    qDebug() << "into q:"+strq;
    q.exec(strq);
}
