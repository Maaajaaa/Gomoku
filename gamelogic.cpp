#include "gamelogic.h"
#include <QtDebug>
GameLogic::GameLogic()
{
    for(int x=0; x<15; x++){
        for(int y=0; y<15; y++){
            boardArray[x][y]=0;
        }
    }
}

GameLogic::~GameLogic()
{

}

void GameLogic::newPieceSet(int x, int y)
{
    qDebug() << "new piece set x:" << x << " y: " << y;
}

void GameLogic::processMove(int x, int y, int type=0)
{
    if(!(boardArray[x][y]==0) ){
        ///TODO: proper exception
        emit sendError("Invalid Move, spot already taken");
        return;
    }else {
        //set new move in board array
        boardArray[x][y]=type;
        //send change back to UI
        emit pieceChanged(x,y,type);
        //look for chains

        //make value matrix which will store the length of the longest chain that stones (of the last mover) connected, where 1 single stone is associalted with 1 and no stone with 0
        QVector<QVector<int>> chains (15, QVector<int>(15));
        //fill with values, x and y are reset
        for(int y=0; y<15; y++){
            for (int x=0; x<15; x++) {
                chains[x][y]=0;
                if(boardArray[x][y]>0){
                    chains[x][y]=1;
                    //check if there are adjacent stones of the same colour, if so put 2
                    //list of moves that need to checked (top left, top middle, top right, left), others get checked later due to the checked field moving right (x rising) and down (y rising)
                    static int possibleMoves[4][2] = {{-1,-1},{0,-1},{1,-1}, {-1,0}};
                    for (int i=0; i<4; i++) {
                        //chain length is 1 by default (different to similar algorithm in computersMove)
                        int chainLength = 1;
                        int xToCheck = x + possibleMoves[i][0];
                        int yToCheck = y + possibleMoves[i][1];
                        //as long as the new field is valid and the same colour as the original field
                        while(xToCheck < 15 && xToCheck > 0 && yToCheck < 15 && yToCheck > 0 && boardArray[x][y] == boardArray[xToCheck][yToCheck]){
                            chainLength++;
                            xToCheck = x + (chainLength*possibleMoves[i][0]);
                            yToCheck = y + (chainLength*possibleMoves[i][1]);

                        }
                        if(chainLength>chains[x][y]){
                            chains[x][y]=chainLength;
                            //update chain numbers upstream if the stone's ID is not higher already
                            for (int jj = chainLength;jj > 0; jj--) {
                               int xToUpdate = x + ((jj-1)*possibleMoves[i][0]);
                               int yToUpdate = y+((jj-1)*possibleMoves[i][1]);
                               if(chains[xToUpdate][yToUpdate] < chainLength){
                                   chains[xToUpdate][yToUpdate] = chainLength;
                               }
                            }
                        }
                        if(chainLength >= 5){
                            emit foundWinner(type);
                            //show winning combination
                            for (;chainLength > 0; chainLength--) {
                                emit pieceChanged(x + ((chainLength-1)*possibleMoves[i][0]),y + ((chainLength-1)*possibleMoves[i][1]), type+2);
                            }
                            return;
                        }
                    }
                }
            }
        }

        //emit valueTableUpdated(chains);
        if(gameMode == 0 && type == beginningColour){
            //computer's turn
            computersMove(chains);
        }
    }

}

void GameLogic::setColourAndMode(bool mbeginningColour, int mgameMode)
{
    beginningColour = mbeginningColour;
    gameMode = mgameMode;
}

void GameLogic::computersMove(QVector<QVector<int>> chains)
{
    /*make value matrix which will value:
     * NOTE: having a defense and an offense rating would allow for different playing styles
     * opponent's chains (starting at a length of 3 they need to be blocked to not loose):
     *  - length of 1: rating 2
     *  - length of 2: rating 2
     *  - length of 3: rating 5
     *  - length of 4: rating 5 (only completing a chain is higher)
     * unused fields that would continue a chain (space needs to be free):
     *  - 0 existing stones: rating 1
     *  - 1 existing stone: rating 2
     *  - 2 existing stones: rating 3
     *  - 3 existing stones: rating 4
     *  - 4 existing stones: rating 6 (higest rating as this moves makes for a win)
     * all other fields that don't provide enough space for a chain:
     *  - rating 0 (if all fields are 0 the game should end in a tie if the computer blocks the player's chains)
     *  - rating -1 field taken
    */
    //fill with values, x and y are reset
    QVector<QVector<int>> ratings (15, QVector<int>(15));
    for(int y=0; y<15; y++){
         for (int x=0; x<15; x++) {
             switch (boardArray[x][y]) {
                case 0:{ // {} needed due to variable declaration in case
                    //field not in any chain, surroundings need to be checked
                    static int possibleMoves[8][2] = {{-1,-1},{0,-1},{1,-1}, {-1,0},{1,0},{-1,1},{0,1},{1,1}};
                    int rating = 0;
                    for (int i=0; i<8; i++) {
                        //chain length is 0 by default (different to similar algorithm in processMove)
                        int chainLength = 0;
                        int xToCheck = x + possibleMoves[i][0];
                        int yToCheck = y + possibleMoves[i][1];
                        int firstXinDirection = xToCheck;
                        int firstYinDirection = yToCheck;
                        //as long as the new field is valid and the same colour as the first adjacent field in the current direction
                        while(xToCheck < 15 && xToCheck > 0 && yToCheck < 15 && yToCheck > 0 && boardArray[firstXinDirection][firstYinDirection] == boardArray[xToCheck][yToCheck] && boardArray[xToCheck][yToCheck] != 0){
                            chainLength++;
                            xToCheck = x + ((chainLength+1)*possibleMoves[i][0]);
                            yToCheck = y + ((chainLength+1)*possibleMoves[i][1]);
                            if(chainLength == 2){
                                qDebug() << "X|Y: " << x << "|" << y << " xtc|ytc: " << xToCheck << "|" << yToCheck << " direction" << possibleMoves[i];
                            }
                        }
                        //chain length is actually one ahead since the checked field
                        //get a temporary rating which might be applied
                        int tempRating = 0;
                        //determine whose chain it is (remember different colour coding)

                        if(boardArray[firstXinDirection][firstYinDirection] - 1 == !beginningColour){
                            //player's chain
                            switch (chainLength) {
                                case 1:
                                case 2:
                                    tempRating = 2;
                                    break;
                                case 3:
                                case 4:
                                    tempRating = 5;
                                    break;
                            }
                        }else if(boardArray[firstXinDirection][firstYinDirection] - 1 == beginningColour) {
                            //computer's chain
                            switch (chainLength) {
                                case 1:
                                    tempRating = 2;
                                    break;
                                case 2:
                                    tempRating = 3;
                                    break;
                                case 3:
                                    tempRating = 4;
                                    break;
                                case 4:
                                    tempRating = 6;
                                    break;
                            }
                        }
                        //apply tempRating (if bigger than old rating)
                        if(tempRating > rating){
                            rating = tempRating;
                        }

                    }
                    //0 chain length is irrelevant
                    if(rating > 0){
                        ratings[x][y] = rating;
                    }
                    break;
                }
                case 1:
                case 2:
                    //sole stone and field taken, rating should be -1
                    ratings[x][y] = -1;
                    break;
             }
         }
    }
    emit valueTableUpdated(ratings);
}
