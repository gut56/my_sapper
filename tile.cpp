#include "tile.h"
#include <QMouseEvent>

Tile::Tile(QWidget *parent, coordinate p,bool hasMine):QToolButton(parent),m_hasMine(hasMine),numAroundMine(0)
{ 
    this->setMaximumSize(sizeTile);
    this->setMinimumSize(sizeTile);
    pos = p;
    this->connect(this,&Tile::clicked,this, [=](){emit signalOnOpen(this->pos);});
}

QSize Tile::sizeHint() const
{
    QSize size = QToolButton::sizeHint();
    size.rheight() += 20;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}

void Tile::markHasMine(bool isMine)
{
    m_hasMine = isMine;
}

bool Tile::checkHasMine() const
{
    return m_hasMine;
}

void Tile::setNumMine(unsigned int n)
{
  numAroundMine = n;
}

unsigned int Tile::getNumMine() const
{
    return numAroundMine;
}

coordinate Tile::getPos() const
{
    return pos;
}
