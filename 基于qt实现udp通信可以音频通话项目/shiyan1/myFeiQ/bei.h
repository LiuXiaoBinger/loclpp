#ifndef BEI_H
#define BEI_H
#include <QLabel>
#include <QEvent>
#include<QPainter>
#include<QImage>

class Bei : public QLabel
{
    Q_OBJECT
public:
    explicit Bei(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    QImage qim;
    private :


signals:

};


#endif // BEI_H
