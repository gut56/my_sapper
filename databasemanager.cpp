#include "databasemanager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

DatabaseManager::DatabaseManager(const QString& path):
    mDatabase(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"))),
    mRecordsDao(*mDatabase)
{
    mDatabase->setDatabaseName(path);
    mDatabase->open();

    mRecordsDao.init();
//    mRecordsDao.addRecord("Player1",120,8,8,10);
}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager singleton;
    return singleton;
}

DatabaseManager::~DatabaseManager()
{
    mDatabase->close();
    delete mDatabase;
}


bool DatabaseManager::checkRecord(int sec, int sizeX, int sizeY, int numMine ) const
{
    QSqlQuery q("select * from records where size_x ="+QString::number(sizeX)+" and size_y = "
           +QString::number(sizeY)+" and num_mine = "
           +QString::number(numMine)+" and time_game_sec < "+QString::number(sec));
    while (q.next()) {
       qDebug() << q.value(1);
       return false;
    }
    return true;
}

void DatabaseManager::annNewRecord(const QString & name, int sec, int sizeX, int sizeY, int numMine) const
{
    mRecordsDao.addRecord(name,sec,sizeX,sizeY,numMine);
}

const QList<QString> DatabaseManager::getRecords(int sec, int sizeX, int sizeY, int numMine)
{
    QSqlQuery q("select * from records where size_x ="+QString::number(sizeX)+" and size_y = "
           +QString::number(sizeY)+" and num_mine = "
           +QString::number(numMine)+" and time_game_sec > "+QString::number(sec)+" limit 3");

    QList<QString> l;
    QSqlRecord rec = q.record();

    while (q.next()) {
        auto row = q.value(rec.indexOf("player")).toString()
                +" "+QString::number(sizeX)+"x"+QString::number(sizeY)
                +" mines:"+QString::number(numMine)
                +" time(sec):"+QString::number(q.value(rec.indexOf("time_game_sec")).toInt());
        l.push_back(row);
    }
    return l;
}

DatabaseManager &DatabaseManager::operator=(const DatabaseManager &rhs)
{
    return instance();
}
