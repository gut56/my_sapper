#ifndef PLAYERDAO_H
#define PLAYERDAO_H

class QString;
class QSqlDatabase;

class PlayerDao
{
public:
    PlayerDao(QSqlDatabase& database);
    void init() const;
    void addPlayer(const QString & name) const;

private:
    QSqlDatabase& mDatabase;

};

#endif // PLAYERDAO_H
