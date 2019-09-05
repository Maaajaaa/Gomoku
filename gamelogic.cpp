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
    if(type == 1 || type == 2){

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
            int chains[15][15];
            //fill with values, x and y are reset
            for(int y=0; y<15; y++){
                for (int x=0; x<15; x++) {
                    chains[x][y]=0;
                    if(boardArray[x][y]==type){
                        chains[x][y]=1;
                        //check if there are adjacent stones of the same colour, if so put 2
                        //list of moves that need to checked (top left, top middle, top right, left), others get checked later due to the checked field moving right (x rising) and down (y rising)
                        static int possibleMoves[4][2] = {{-1,-1},{0,-1},{1,-1}, {-1,0}};
                        for (int i=0; i<4; i++) {
                            int j = 1;
                            int xToCheck = x + (j*possibleMoves[i][0]);
                            int yToCheck = y + (j*possibleMoves[i][1]);
                            //as long as the new field is valid and the same colour as the original field
                            while(xToCheck < 15 && xToCheck > 0 && yToCheck < 15 && yToCheck > 0 && boardArray[x][y] == boardArray[xToCheck][yToCheck]){
                                j++;
                                xToCheck = x + (j*possibleMoves[i][0]);
                                yToCheck = y + (j*possibleMoves[i][1]);

                            }
                            if(j>chains[x][y]){
                                chains[x][y]=j;
                            }
                            if(j >= 5){
                                emit foundWinner(type);
                                //show winning combination
                                for (;j > 0; j--) {
                                    emit pieceChanged(x + ((j-1)*possibleMoves[i][0]),y + ((j-1)*possibleMoves[i][1]), type+2);
                                }
                                return;
                            }
                        }
                    }
                }
            }
            for(int y=0; y<15; y++){
                for(int x=0; x<15; x++){
                    if(chains[x][y] !=0)
                        qDebug() << "X:" << x << " Y:" << y << chains[x][y];
                }
            }

        }
    }

}
