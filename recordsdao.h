#ifndef RECORDSDAO_H
#define RECORDSDAO_H
#include <QString>

class QSqlDatabase;

class RecordsDao
{
public:
    RecordsDao(QSqlDatabase& database);
    void init() const;
    void addRecord(const QString& namePlayer, int seconds, int s_x, int s_y,int num_mine) const;

private:
    QSqlDatabase& mDatabase;
};

#endif // RECORDSDAO_H
