#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include "recordsdao.h"

class QSqlDatabase;

const QString DATABASE_FILENAME = "records.db";

class DatabaseManager
{
public:
    static DatabaseManager& instance();
    ~DatabaseManager();
    bool checkRecord(int sec, int sizeX, int sizeY, int numMine ) const;
    void annNewRecord(const QString & name, int sec, int sizeX, int sizeY, int numMine) const;
    const QList<QString> getRecords(int sec, int sizeX, int sizeY, int numMine );

protected:
    DatabaseManager(const QString& path = DATABASE_FILENAME);
    DatabaseManager& operator=(const DatabaseManager& rhs);
private:
    QSqlDatabase* mDatabase;
    const RecordsDao mRecordsDao;
};

#endif // DATABASEMANAGER_H
