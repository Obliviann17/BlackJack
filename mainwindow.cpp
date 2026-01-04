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

    ui->viewGame->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->viewGame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->btnHit, &QPushButton::clicked, this, &MainWindow::onHitClicked);
    connect(ui->btnStand, &QPushButton::clicked, this, &MainWindow::onStandClicked);

    startNewGame();
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
        ui->btnHit->setEnabled(false);
        ui->btnStand->setEnabled(false);
        ui->lblScorePlayer->setText("Busted! Score: " + QString::number(mPlayer->getScore()));
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
        if(cardItem && cardItem->isFaceUp())
        {
            cardItem->setFaceUp(true);
        }
    }

    size_t cardsBefore = mDealer->getHand().getCardCount();

    mDealer->playTurn(*mDeck);

    const auto& dCards = mDealer->getHand().getCards();
    for(size_t i = cardsBefore; i < dCards.size(); ++i)
    {
        addCardToScene(dCards[i].get(), i, true);
    }

    updateUI();

    int pScore = mPlayer->getScore();
    int dScore = mDealer->getScore();
    QString message;

    if (dScore > 21)
    {
        message = "Dealer busted. You Win!";
    }
    else if (pScore > dScore)
    {
        message = "You Win!";
    }
    else if (pScore < dScore)
    {
        message = "Dealre Win!";
    }
    else
    {
        message = "Push";
    }

    ui->lblScorePlayer->setText(ui->lblScorePlayer->text() + " | " + message);
}

void MainWindow::updateUI()
{
    if(mPlayer)
    {
        ui->lblScorePlayer->setText("Score: " + QString::number(mPlayer->getScore()));
    }
}

void MainWindow::addCardToScene(const Card* card, int index, bool isDealer)
{
    CardItem* item = new CardItem(card);

    if(isDealer && index == 1)
    {
        item->setFaceUp(false);
    }

    mScene->addItem(item);

    int xBase = 100;
    int yBase = isDealer ? 50 : 350;
    int offset = 40;

    QPointF finalPos(xBase + (index * offset), yBase);
    item->flyToPlayer(finalPos);
}
