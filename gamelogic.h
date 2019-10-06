#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>


class GameLogic: public QObject{
    Q_OBJECT
public:
    GameLogic();
    ~GameLogic();

public slots:
    void processMove(int x, int y, int type);
    void setColourAndMode(bool mbeginningColour, int mgameMode);

signals:
    void pieceChanged(int x, int y, int type);
    void displayMessage(QString errorMessage);
    void foundWinner(int type, int turnCount);
    void valueTableUpdated(QVector<QVector<int>> chains);
    void computerTurnDecided(int, int);

private:
    bool beginningColour = 0; //0-black 1-white
    int gameMode = 0;   //0 - singleplayer 1- multiplayer 2- renju (single-player)
    int turnCount = 0;  //number of turns
    int board[15][15]; //representation of the board
    void computersMove();
};

#endif // GAMELOGIC_H
