#ifndef TILE_H
#define TILE_H
#include <QWidget>
#include <QToolButton>

typedef std::pair<int,int> coordinate;

class Tile : public QToolButton
{
    Q_OBJECT
public:
    explicit Tile(QWidget *parent = nullptr, coordinate p = coordinate(0,0),bool hasMine = false );
    QSize sizeHint() const override;

    void markHasMine(bool isMine);
    bool checkHasMine() const;
    void setNumMine(unsigned int n);
    unsigned int getNumMine() const;
    coordinate getPos() const;

private:
    const QSize sizeTile = QSize(40,40);
    coordinate pos;
    bool m_hasMine = false;
    unsigned int numAroundMine;

signals:
    void signalOnOpen(coordinate &p);

};

#endif // TILE_H
