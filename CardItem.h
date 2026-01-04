#ifndef CARDITEM_H
#define CARDITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include "Card.h"

class CardItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    CardItem(const Card* card, QObject* parent = nullptr);

    void flyToPlayer(QPointF endPos, int duration = 600);
    void setSkin(QString skinName);

    void setFaceUp(bool faceUp);
    bool isFaceUp() const  {return mIsFaceUp; };

private:
    const Card* mCard;
    QString mCurrentSkin;

    bool mIsFaceUp = true;
    QPixmap mFrontPixmap;
    QPixmap mBackPixmap;
};
#endif // CARDITEM_H
