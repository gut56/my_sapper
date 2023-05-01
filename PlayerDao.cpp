#include "PlayerDao.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlRecord>


PlayerDao::PlayerDao(QSqlDatabase& database):
    mDatabase(database)
{
}

void PlayerDao::init() const
{
    if (!mDatabase.tables().contains("players")) {
        QSqlQuery query(mDatabase);
        qDebug() << "constr PlayerDao";
        query.exec("CREATE TABLE players (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, is_current boolean default false )");
    }
}

void PlayerDao::addPlayer(const QString &name) const
{
    QSqlQuery q;
    QString strq = "select * from players where name ='"+name+"'";
    qDebug() << "add palayers q:"+strq;
    bool r = q.exec(strq);
    if (!r){
        qDebug() << "Unable to execute query - exiting" ;
        return;
    }

    while (q.next()) {
        qDebug() << "plsyer "+name+" already  exists";
        return;
    }

    // add new name
    strq = "INSERT INTO players (name) values ('"+name+"')";
    qDebug() << "into q:"+strq;
    q.exec(strq);
}
