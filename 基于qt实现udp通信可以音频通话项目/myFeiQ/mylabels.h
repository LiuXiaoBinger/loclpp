
#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QEvent>
#include<QPainter>
#include<QImage>
class mylabels : public QLabel
{
    Q_OBJECT
public:
    explicit mylabels(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);
    QImage qim;
    private :


signals:

};

#endif // MYLABEL_H
