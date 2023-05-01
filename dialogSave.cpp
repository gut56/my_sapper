#include "dialogSave.h"
#include <QtWidgets>

dialog_save::dialog_save(QWidget * pwgt):
    QDialog(pwgt, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    m_ptxtNamePlayer = new QLineEdit("Player_1");
    m_ptxtSeconds = new QLabel;
    m_ptxtComplexity = new QLabel;

    QString s = "size %1x%2 number mine %3";
    s= s.arg(m_cmp.size_x)
        .arg(m_cmp.size_y)
        .arg(m_cmp.num_mine);
    m_ptxtComplexity->setText(s);

    QLabel* pLabelPlayer = new QLabel("&name player");
    QLabel* pLabelSec = new QLabel("time(sec)");
    QLabel* pLabelComplexity = new QLabel("complexity");
    pLabelPlayer->setBuddy(m_ptxtNamePlayer);
    QPushButton* pcmdOk = new QPushButton("&Ok");
    QPushButton* pcmdCancel = new QPushButton("&Cancel");
    connect(pcmdOk, SIGNAL(clicked()),SLOT(accept()));
    connect(pcmdCancel, SIGNAL(clicked()),SLOT(reject()));

    //Layout setup
    QVBoxLayout * pmainLayout = new QVBoxLayout;
    ptopLayout = new QGridLayout;
    pmainLayout->addLayout(ptopLayout);
    QHBoxLayout * pbuttomLayout = new QHBoxLayout;
    pmainLayout->addLayout(pbuttomLayout);

    ptopLayout->addWidget(pLabelPlayer, 0, 0);
    ptopLayout->addWidget(pLabelSec, 1, 0);
    ptopLayout->addWidget(pLabelComplexity, 2, 0);
    ptopLayout->addWidget(m_ptxtNamePlayer,0, 1);
    ptopLayout->addWidget(m_ptxtSeconds, 1, 1);
    ptopLayout->addWidget(m_ptxtComplexity, 2, 1);
    pbuttomLayout ->addWidget(pcmdOk);
    pbuttomLayout->addWidget(pcmdCancel);
    setLayout(pmainLayout);
}

QString dialog_save::namePlayer() const
{
    return m_ptxtNamePlayer->text();
}

void dialog_save::setSeconds(int numSec)
{
    m_ptxtSeconds->setText(QString::number(numSec));
}

void dialog_save::setComplexity(int size_x, int size_y, int num_mine)
{
    m_cmp.size_x = size_x;
    m_cmp.size_x = size_y;
    m_cmp.num_mine = num_mine;

    QString s = "size %1x%2 number mine %3";
    s= s.arg(m_cmp.size_x)
        .arg(m_cmp.size_y)
        .arg(m_cmp.num_mine);
    m_ptxtComplexity->setText(s);
}

void dialog_save::addLineText(const QList<QString> &l) const
{
    int k = 3;
    for (const auto &v : l) {
        auto t = new QLabel(v);
        ptopLayout->addWidget(t,k,0,1,2);
        k++;
    }
}
