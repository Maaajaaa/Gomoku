#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QtDebug>
#include <QMessageBox>
#include "gamelogic.h"
#include "gopiece.h"
#include "highscoredialog.h"

namespace Ui {
class BoardWidget;
}

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWidget(QWidget *parent = nullptr, int mGameMode=-1);
    ~BoardWidget();

public slots:
    void showMoveOnBoard(int x, int y, int type);
    void computerTurn(int x, int y);
    void displayValuesOnBoard(QVector<QVector<int>> chains);
    void newPieceSet(goPiece*);
    void showWinner(int type, int turnCount);
    void showMessage(QString message);

signals:
    void sendMoveToLogic(int x, int y, int type);
    void backToMainMenu();

private slots:
    void on_backToMainMenuPushButton_clicked();

private:
    Ui::BoardWidget *ui;
    QVector<goPiece*> goPieces;
    GameLogic *gamelog = new GameLogic();
    QWidget *mParent;
    bool lastMove = false;  //colour that had last turn
    int gameMode = 0; //0 - single player, 1 - multiplayer 2- renju (single-player)
    bool beginningColour = 0; //0 - black, 1 - white
};

#endif // BOARDWIDGET_H
