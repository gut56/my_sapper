#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <map>
#include "tile.h"
#include <QTime>
#include <databasemanager.h>


using std::map;
using std::pair;
class QRadioButton;
class QLabel;
class PlayerDao;
typedef std::pair<int,int> filedSize;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    static constexpr std::array<coordinate,8> arrForNearCheckTile{
                coordinate(-1,0),coordinate(-1,-1),coordinate(0,-1),
                coordinate(-1,1),coordinate(0,1),coordinate(1,1),
                coordinate(1,0),coordinate(1,-1)};

    enum size {minSize=0,mediumSize, bigSize,castomSize};
    std::array<filedSize,4> arrSizes{filedSize(8,8),filedSize(16,16),filedSize(24,36),filedSize(24,36)};
    QGridLayout* tilesLayout;
    map<coordinate,Tile*> m_tiles;
    int m_timerId = 0;
    QLabel * m_pTimerLabel;
    QTime m_time;
    DatabaseManager * mDataManager;

    int amountMines = 0;
    filedSize currSize = arrSizes[minSize];
    void redrawTiles();
    std::array<QRadioButton*,3> arrRButtons;
    bool m_isAtiveGame = false;

    void openNeighbors(coordinate p,bool isFirst = false, bool isStart = false);
    void slotShowAll(int mode = 0);
    void initMines(coordinate p = coordinate(0,0));
    bool isStartInit = false;
    bool checksWin() const;
    void showMassage(int code = 0);

    void start(coordinate p = coordinate(0,0),bool needRedrawTiles = false);
    void finish();
    virtual void timerEvent(QTimerEvent*);

private slots:
    void slotRadioButtonClicked();
    void slotClickTile(coordinate& p);

};

//constexpr std::array<coordinate,8> Widget::arrForNearCheckTile;

#endif // WIDGET_H
