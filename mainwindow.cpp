#include <QTimer>

#include "CardItem.h"

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mScene = new QGraphicsScene(this);
    ui->viewGame->setScene(mScene);
    ui->viewGame->setSceneRect(0, 0, 800, 600);

    ui->viewGame->setStyleSheet("background-color: #0d4d0d; border: none; ");

    QString btnStyle = "QPushButton { "
                       " background-color: white; "
                       " border-radius: 5px; "
                       " font-weight: bold; "
                       " padding: 5px; "
                       "}"
                       "QPushButton::disabled {"
                       " background-color: #aaaaaa; "
                       "}";

    ui->btnHit->setStyleSheet(btnStyle);
    ui->btnStand->setStyleSheet(btnStyle);
    ui->btnDeal->setStyleSheet(btnStyle);

    ui->lblScorePlayer->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    ui->lblBalance->setStyleSheet("color: yellow; font-size: 16px; font-weight: bold;");
    ui->lblBet->setStyleSheet("color: white; font-size: 14px; ");

    ui->viewGame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->viewGame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->btnDeal, &QPushButton::clicked, this, &MainWindow::onDealClicked);
    connect(ui->btnBetMinus, &QPushButton::clicked, this, &MainWindow::onBetMinus);
    connect(ui->btnBetPlus, &QPushButton::clicked, this, &MainWindow::onBetPlus);

    connect(ui->btnHit, &QPushButton::clicked, this, &MainWindow::onHitClicked);
    connect(ui->btnStand, &QPushButton::clicked, this, &MainWindow::onStandClicked);

    ui->btnHit->setEnabled(false);
    ui->btnStand->setEnabled(false);
    ui->btnDeal->setEnabled(true);

    updateUI();

    connect(ui->comboSkins, &QComboBox::currentIndexChanged, this, [this](int index)
    {
        QString newPath;

        if(index == 0)
            newPath = "assets/back_red.png";
        else
            newPath = "assets/back_blue.png";

        CardItem::currentSkinPath = newPath;

        auto items = mScene->items();
        for(auto* item : items)
        {
            CardItem* card = dynamic_cast<CardItem*>(item);
            if(card)
                card->updateSkin(newPath);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startNewGame()
{
    mScene->clear();

    mDeck = std::make_unique<Deck>();
    mDeck->shuffle();

    mPlayer = std::make_unique<Player>();
    mDealer = std::make_unique<Dealer>();

    ui->btnHit->setEnabled(true);
    ui->btnStand->setEnabled(true);

    mPlayer->hit(*mDeck);
    mPlayer->hit(*mDeck);

    const auto& pCards = mPlayer->getHand().getCards();
    for(size_t i = 0; i < pCards.size(); ++i)
    {
        addCardToScene(pCards[i].get(), i, false);
    }

    mDealer->hit(*mDeck);
    mDealer->hit(*mDeck);

    const auto& dCards = mDealer->getHand().getCards();
    for(size_t i = 0; i < dCards.size(); ++i)
    {
        addCardToScene(dCards[i].get(), i, true);
    }

    updateUI();
}

void MainWindow::onHitClicked()
{
    mPlayer->hit(*mDeck);

    int index = mPlayer->getHand().getCardCount() - 1;
    const Card* lastCard = mPlayer->getHand().getCards().back().get();

    addCardToScene(lastCard, index, false);

    updateUI();

    if(mPlayer->isBusted())
    {
        ui->lblScorePlayer->setText("Busted! Score: " + QString::number(mPlayer->getScore()));

        ui->btnHit->setEnabled(false);
        ui->btnStand->setEnabled(false);

        ui->btnBetMinus->setEnabled(true);
        ui->btnBetPlus->setEnabled(true);
        ui->btnDeal->setEnabled(true);
        mGameInProgress = false;
    }
}

void MainWindow::onStandClicked()
{
    ui->btnHit->setEnabled(false);
    ui->btnStand->setEnabled(false);

    auto items = mScene->items();
    for(auto* item : items)
    {
        CardItem* cardItem = dynamic_cast<CardItem*>(item);
        if(cardItem && !cardItem->isFaceUp())
        {
            cardItem->setFaceUp(true);
        }
    }

    size_t cardsBefore = mDealer->getHand().getCardCount();

    mDealer->playTurn(*mDeck);

    const auto& dCards = mDealer->getHand().getCards();

    int delay = 0;

    for(size_t i = cardsBefore; i < dCards.size(); ++i)
    {
        delay += 500;

        QTimer::singleShot(delay, this, [this, i]()
        {
            const auto& cards = mDealer->getHand().getCards();
            if(i < cards.size())
            {
                addCardToScene(cards[i].get(), i, true);
            }
        });
    }

    QTimer::singleShot(delay + 500, this, [this]()
    {
        int pScore = mPlayer->getScore();
        int dScore = mDealer->getScore();
        QString message;

        if (dScore > 21)
        {
            message = "Dealer busted. You Win!";
            mBalance += mCurrentBet * 2;
        }
        else if (pScore > dScore)
        {
            message = "You Win!";
            mBalance += mCurrentBet * 2;
        }
        else if (pScore < dScore)
        {
            message = "Dealer Wins!";
        }
        else
        {
            message = "Push";
            mBalance += mCurrentBet;
        }

        updateUI();

        ui->lblScorePlayer->setText(ui->lblScorePlayer->text() + " | " + message);

        ui->btnDeal->setEnabled(true);
        ui->btnBetPlus->setEnabled(true);
        ui->btnBetMinus->setEnabled(true);
        mGameInProgress = false;
    });
}

void MainWindow::updateUI()
{
    if(mPlayer)
    {
        ui->lblScorePlayer->setText("Score: " + QString::number(mPlayer->getScore()));
    }

    ui->lblBalance->setText("Balance: $" + QString::number(mBalance));
    ui->lblBet->setText("Bet: $" + QString::number(mCurrentBet));
}

void MainWindow::addCardToScene(const Card* card, int index, bool isDealer)
{
    CardItem* item = new CardItem(card);

    if(isDealer && index == 1)
    {
        item->setFaceUp(false);
    }

    item->setZValue(index);

    mScene->addItem(item);

    int xBase = 100;
    int yBase = isDealer ? 50 : 350;
    int offset = 40;

    QPointF finalPos(xBase + (index * offset), yBase);
    item->flyToPlayer(finalPos);
}


void MainWindow::onBetPlus()
{
    if(mGameInProgress)
        return;
    if(mCurrentBet + 10 <= mBalance)
        mCurrentBet += 10;

    updateUI();
}

void MainWindow::onBetMinus()
{
    if(mGameInProgress)
        return;
    if(mCurrentBet - 10 >= 10)
        mCurrentBet -= 10;

    updateUI();
}

void MainWindow::onDealClicked()
{
    if(mBalance < mCurrentBet)
        return;

    mBalance -= mCurrentBet;
    mGameInProgress = true;

    ui->btnBetMinus->setEnabled(false);
    ui->btnBetPlus->setEnabled(false);
    ui->btnDeal->setEnabled(false);

    ui->btnHit->setEnabled(true);
    ui->btnStand->setEnabled(true);

    startNewGame();
}
