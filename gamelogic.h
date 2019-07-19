#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>


class GameLogic: public QObject{
    Q_OBJECT
public:
    GameLogic();
    ~GameLogic();

public slots:
    void newPieceSet(int x, int y);
    void processMove(int x, int y, int type);

signals:
    void pieceChanged(int x, int y, int type);
    void sendError(QString errorMessage);
    void foundWinner(int type);

private:
    int boardArray[15][15];
    int xToCheck(int i);
    int yToCheck(int i);
};

#endif // GAMELOGIC_H
