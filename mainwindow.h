#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <memory>

#include "Deck.h"
#include "Player.h"
#include "Dealer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void onHitClicked();
    void onStandClicked();

private:
    void startNewGame();
    void updateUI();
    void addCardToScene(const Card* card, int index, bool isDealer);

    Ui::MainWindow *ui;

    QGraphicsScene *mScene;

    std::unique_ptr<Deck> mDeck;
    std::unique_ptr<Player> mPlayer;
    std::unique_ptr<Dealer> mDealer;
};
#endif // MAINWINDOW_H
