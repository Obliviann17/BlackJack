#include <QPixmap>

#include "CardItem.h"

CardItem::CardItem(const Card* card, QObject* parent)
    : QObject(parent), QGraphicsPixmapItem(), mCard(card)
{
    QString rankStr;

    switch (mCard->getRank()) {
    case Rank::Two:
        rankStr = "2";
        break;
    case Rank::Three:
        rankStr = "3";
        break;
    case Rank::Four:
        rankStr = "4";
        break;
    case Rank::Five:
        rankStr = "5";
        break;
    case Rank::Six:
        rankStr = "6";
        break;
    case Rank::Seven:
        rankStr = "7";
        break;
    case Rank::Eight:
        rankStr = "8";
        break;
    case Rank::Nine:
        rankStr = "9";
        break;
    case Rank::Ten:
        rankStr = "10";
        break;
    case Rank::Jack:
        rankStr = "jack";
        break;
    case Rank::Queen:
        rankStr = "queen";
        break;
    case Rank::King:
        rankStr = "king";
        break;
    case Rank::Ace:
        rankStr = "Ace";
        break;
    }

    QString suitStr;

    switch(mCard->getSuit())
    {
    case Suit::Clubs:
        suitStr = "clubs";
        break;
    case Suit::Diamonds:
        suitStr = "diamonds";
        break;
    case Suit::Hearts:
        suitStr = "hearts";
        break;
    case Suit::Spades:
        suitStr = "spades";
    }

    QString imgPath = QString("assets/%1_of_%2.png").arg(rankStr, suitStr);

    mFrontPixmap = QPixmap(imgPath);

    if(imgPath.isNull())
    {
        mFrontPixmap = QPixmap(100, 145);
        mFrontPixmap.fill(Qt::red);
    }

    mBackPixmap = QPixmap("assets/back.png");
    if(mBackPixmap.isNull())
    {
        mBackPixmap = QPixmap(100, 145);
        mBackPixmap.fill(Qt::blue);
    }

    setPixmap(mFrontPixmap);
    setScale(0.25);
    setPos(-200, -200);
}

void CardItem::flyToPlayer(QPointF endPos, int duration)
{
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(duration);
    animation->setStartValue(pos());
    animation->setEndValue(endPos);
    animation->setEasingCurve(QEasingCurve::OutCubic);

    connect(animation, &QPropertyAnimation::finished, animation, &QObject::deleteLater);

    animation->start();
}

void CardItem::setFaceUp(bool faceUp)
{
    mIsFaceUp = faceUp;
    if(mIsFaceUp)
    {
        setPixmap(mFrontPixmap);
    }
    else
    {
        setPixmap(mBackPixmap);
    }
}
