#ifndef DIALOGSAVE_H
#define DIALOGSAVE_H
#include <QDialog>

class QLineEdit;
class QLabel;
class QGridLayout;

struct complexity
{
    int size_x = 8;
    int size_y = 8;
    int num_mine = 10;
};

class dialog_save : public QDialog
{
    Q_OBJECT
public:
    dialog_save(QWidget * pwgt = 0);
    QString namePlayer() const;
    void setSeconds(int numSec);
    void setComplexity(int size_x, int size_y, int num_mine);
    void addLineText(const QList<QString> & l) const;

private:
    QLineEdit* m_ptxtNamePlayer;
    QLabel * m_ptxtSeconds;
    QLabel * m_ptxtComplexity;
    complexity m_cmp;
    QGridLayout* ptopLayout;

};

#endif // DIALOGSAVE_H
