#include "widget.h"
#include <QtWidgets>
#include <dialogSave.h>
#include "databasemanager.h"



Widget::Widget(QWidget *parent)
    : QWidget(parent), mDataManager(&DatabaseManager::instance())
{ 
    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    QVBoxLayout* vLayout = new QVBoxLayout();
    mainLayout->addLayout(vLayout,1);
    tilesLayout = new QGridLayout();
    vLayout->addLayout(tilesLayout,1);
    QVBoxLayout* buttonsLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonsLayout);
    tilesLayout->setSpacing(1);
    vLayout->addStretch();

    QPushButton * bNewG = new QPushButton("New game");
    buttonsLayout->addWidget(bNewG);
//    QPushButton * bShow = new QPushButton("Show all");
//    buttonsLayout->addWidget(bShow);
    mainLayout->addStretch();

    arrRButtons[0] = new QRadioButton("Min");
    arrRButtons[1] = new QRadioButton("Medium");
    arrRButtons[2] = new QRadioButton("Max");

    for (auto& v: arrRButtons) {
        buttonsLayout->addWidget(v);
    }

    m_pTimerLabel = new QLabel();
    buttonsLayout->addWidget(m_pTimerLabel);
    m_pTimerLabel->hide();
//    m_pTimerLabel.si

    arrRButtons[0]->setChecked(true);
    buttonsLayout->addStretch();

    for (auto& v: arrRButtons) {
        connect(v,SIGNAL(clicked()),SLOT(slotRadioButtonClicked()));
    }
//    connect(bShow,&QPushButton::clicked,this,&Widget::slotShowAll);
    connect(bNewG,&QPushButton::clicked,this,[=](){ start(coordinate(0,0),true); });

    setLayout(mainLayout);
    redrawTiles();
 }

Widget::~Widget()
{
}

void Widget::redrawTiles()
{
    this->isStartInit = false;
    if (!m_tiles.empty()) {
        for (auto& v : m_tiles) {
            tilesLayout->removeWidget(v.second);
            v.second->hide();
            delete v.second;
        }
        m_tiles.clear();
    }

    for (int i = 0; i < currSize.first; ++i) { // generate tile
        for (int j = 0; j < currSize.second; ++j) {
            m_tiles.insert(pair(pair(i,j),new Tile(this,pair(i,j))));
        }
    }

    for (int i = 0; i < currSize.first; ++i) {
        for (int j = 0; j < currSize.second; ++j) {
            tilesLayout->addWidget(m_tiles.at(pair(i,j)),i,j);
            connect(m_tiles.at(pair(i,j)),&Tile::signalOnOpen,this,&Widget::slotClickTile);
        }
    }
}

void Widget::openNeighbors(coordinate p, bool isFirst, bool isStart)
{
    if (m_tiles.at(p)->checkHasMine() ) {
        m_tiles.at(p)->setDown(true);
        QPixmap pix(":/mine_red.png");
        m_tiles.at(p)->setIcon(pix);
        m_tiles.at(p)->setMaximumSize(m_tiles.at(p)->size());
        m_tiles.at(p)->setIconSize(m_tiles.at(p)->size());
        slotShowAll();
        showMassage(1);
        return;
    }
    else if(m_tiles.at(p)->getNumMine() > 0 && !m_tiles.at(p)->checkHasMine()) {
        m_tiles.at(p)->setDown(true);
        m_tiles.at(p)->setText(QString::number(m_tiles.at(p)->getNumMine()));
        for (const auto& v : arrForNearCheckTile) {
            try {
                coordinate c(p.first+v.first,p.second+v.second);
                Tile *t = m_tiles.at(c);
                if (!t->isDown() && t->getNumMine() == 0 && !t->checkHasMine() ){
                    t->setDown(true);
                    openNeighbors(c);
                }
            }
            catch (...) {
                continue;
            }
        }
        if (checksWin()) {
            slotShowAll(1);
            showMassage();
        }
        return;
    }

    m_tiles.at(p)->setDown(true);

    for (const auto& v : arrForNearCheckTile) {
        try {
            coordinate c(p.first+v.first,p.second+v.second);
            Tile *t = m_tiles.at(c);
            if (!t->isDown()){
                t->setDown(true);
                if(t->getNumMine() == 0)
                    openNeighbors(c);
                else {
                    t->setText(QString::number(t->getNumMine()));
                }
            }
        }
        catch (...) {
            continue;
        }
    }

    if (checksWin()) {
        slotShowAll(1);
        showMassage();
    }
}

void Widget::slotRadioButtonClicked()
{
    for (int i = 0; i < arrRButtons.size(); ++i) {
        if (arrRButtons[i]->isChecked()) {
            switch (i) {
            case 0:
                currSize = arrSizes[minSize];
                break;
            case 1:
                currSize = arrSizes[mediumSize];
                break;
            case 2:
                currSize = arrSizes[bigSize];
                break;
            case 3:
                currSize = arrSizes[castomSize];
                break;
            }
        }
    }
    qDebug() << "currSize: " << currSize;
    redrawTiles();
}

void Widget::slotClickTile(coordinate &p)
{    
    if(!this->isStartInit)
    {
        start(p);
    }

    if (m_isAtiveGame && !m_tiles[p]->isDown())
    {
        openNeighbors(p,true);
    }
}

void Widget::slotShowAll(int mode) {
    QString pathIm = mode == 0 ? ":/mine.png" : ":/flag_black.png";
    for (auto &v : m_tiles) {
        if (!v.second->isDown()) {
            v.second->setDown(true);
            if (v.second->checkHasMine()) {
                QPixmap pix(pathIm);
                v.second->setIcon(pix);
                v.second->setIconSize(v.second->size());
            }
            else if(v.second->getNumMine() > 0){
                v.second->setText(QString::number(v.second->getNumMine()));
            }
        }
    }
    finish();
}

void Widget::initMines(coordinate posSkip)
{
    auto generateCoordinate = [=]()->coordinate{
        auto x = 0+QRandomGenerator::global()->generate()%(currSize.first-1);
        auto y = 0+QRandomGenerator::global()->generate()%(currSize.second-1);
        return coordinate(x,y);
    };

    amountMines = floor((currSize.first*currSize.second)/6);
    auto i = 0;

    std::array<coordinate,9> arrSkip;
    for (const auto &v : this->arrForNearCheckTile) {
        arrSkip[i]  = coordinate(v.first+posSkip.first,v.second+posSkip.second);
        i++;
    }
    i=0;
    arrSkip[8] = posSkip;

    while(i<amountMines){
        coordinate c = generateCoordinate();
        auto it = std::find(arrSkip.begin(),arrSkip.end(),c);
//        qDebug() << "-*" << c;
        try {
            if ( it == arrSkip.end() && (!m_tiles[c]->checkHasMine())){
//                qDebug() << "--*" << c;
                m_tiles[c]->markHasMine(true);
                i++;
            }
        }
        catch (...) {
            continue;
        }

    }
    this->isStartInit = true;

    for (auto &v  : m_tiles) {
        int n = 0;
        if (!v.second->checkHasMine() ){
            for (auto &p : arrForNearCheckTile) {
                coordinate c(coordinate(v.first.first+p.first,v.first.second+p.second));
                try {
                    if (m_tiles.at(c)->checkHasMine()){
                        ++n;
                    }
                } catch (...) {
                    continue;
                }
            }
        }
        if(n>0)
            v.second->setNumMine(n);
    }
}

bool Widget::checksWin() const
{
    int k = 0;
    for (const auto &v : m_tiles) {
        if(!v.second->checkHasMine() && v.second->isDown())
            ++k;
    }
    qDebug() << "checksWin " << (k+amountMines) << " x " << currSize.first*currSize.second;
    return (k+amountMines) == currSize.first*currSize.second;
}

void Widget::showMassage(int code) {
    QString mStr;    
    QMessageBox msgBox(QMessageBox::Information,"Sapper","");

    if (code == 0 )
        mStr = "<H2>You won!!!</H2><b>time:"+m_time.toString()+"</b>";
    else if (code == 1)
        mStr = "<H2>You lost!!!</H2><b>time:"+m_time.toString()+"</b>";


    msgBox.setText(mStr);
    msgBox.addButton(QMessageBox::Ok);
    msgBox.addButton(QMessageBox::Retry);

    if (code == 0 )
    {
        auto r = mDataManager->checkRecord(m_time.hour()*3600+m_time.minute()*60+m_time.second(),currSize.first,currSize.second,amountMines);
        qDebug() << "is record " << r;
        if (r)
            msgBox.addButton(QMessageBox::Save);
    }

    auto n = msgBox.exec();

    auto excludedTitle = [=]()
    {
        for (auto &v : m_tiles) {
            v.second->setEnabled(false);
        }
    };

    if (n == QMessageBox::Save)
    {
        qDebug() << "SAVE";
        int nSec = m_time.hour()*3600+m_time.minute()*60+m_time.second();
        dialog_save * d = new dialog_save;
        d->setComplexity(currSize.first,currSize.second,amountMines);
        d->setSeconds(nSec);
        d->addLineText(mDataManager->getRecords(nSec,currSize.first,currSize.second,amountMines));

        auto k = d->exec();
        if (k == QDialog::Accepted )
        {
            QString name = d->namePlayer();
            qDebug() << name;
            mDataManager->annNewRecord(name,nSec,currSize.first,currSize.second,amountMines);
        }

        delete d;
        excludedTitle();
    }
    else if(n == QMessageBox::Retry)
    {
        qDebug() << "RETRY";
        start(coordinate(0,0),true);
    }
    else if(n == QMessageBox::Ok) {
        excludedTitle();
    }
}

void Widget::start(coordinate p, bool needRedrawTiles)
{
    if (needRedrawTiles)
        redrawTiles();
    else{
        initMines(p);
        m_isAtiveGame = true;
        m_pTimerLabel->setText("<H3>00:00:00</H3>");
        m_pTimerLabel->show();
        m_time.setHMS(0,0,0);
        m_timerId = startTimer(1000);
    }
}

void Widget::finish()
{
    m_isAtiveGame = false;
    killTimer(m_timerId);
    m_pTimerLabel->hide();
}

void Widget::timerEvent(QTimerEvent * e)
{
    m_time = m_time.addMSecs(1000);
    m_pTimerLabel->setText("<H3>"+m_time.toString()+"</H3>");
    qDebug() << "timer id:" << e->timerId();
}
