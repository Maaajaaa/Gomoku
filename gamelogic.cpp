#include "gamelogic.h"
#include <QtDebug>
GameLogic::GameLogic()
{
    for(int x=0; x<15; x++){
        for(int y=0; y<15; y++){
            board[x][y]=0;
        }
    }
}

GameLogic::~GameLogic()
{

}

void GameLogic::processMove(int x, int y, int type=0)
{
    if(!(board[x][y]==0) ){
        ///TODO: proper exception
        emit displayMessage("Invalid Move, spot already taken");
        return;
    }else {

        //count up turn count
        turnCount++;

        //check Renju opening for validity
        if(gameMode == 3 && (turnCount == 2 || turnCount == 3)){
            switch (turnCount) {
            case 2:
                //2nd move must be within center 3x3 square
                if(!(x>=6 && x<=8 && y>=6 && y<=8)){
                    //invalid move
                    turnCount--;
                    emit displayMessage("Invalid move, 2nd move must be within \
                                            center 3x3 square");
                    return;
                }
            break;
            case 3:
                //3rd move must be within center 5x5 square
                if(!(x>=5 && x<=9 && y>=5 && y<=9)){
                    //invalid move
                    turnCount--;
                    emit displayMessage("Invalid move, 3rd move must be within \
                                            center 5x5 square");
                    return;
                }

            break;
            }
        }

        //set new move in board array
        board[x][y]=type;
        //send change back to UI
        emit pieceChanged(x,y,type);

        //look for chains

        /*make value matrix which will store the length of the longest chain
         * that stones (of the last mover) connected,
         * where 1 single stone is associalted with 1 and no stone with 0 */
        QVector<QVector<int>> chains (15, QVector<int>(15));
        //fill with values, x and y are reset
        for(int y=0; y<15; y++){
            for (int x=0; x<15; x++) {
                chains[x][y]=0;
                if(board[x][y]>0){
                    chains[x][y]=1;
                    /*check if there are adjacent stones of the same colour,
                     * if so put 2.
                     * List of moves that need to checked
                     * (top left, top middle, top right, left),
                     * others get checked later due to the checked field moving
                     * right (x rising) and down (y rising) */
                    static int moves[4][2] = {{-1,-1},{0,-1},{1,-1}, {-1,0}};
                    for (int i=0; i<4; i++) {
                        /*row length is 1 by default (different to similar
                         * algorithm in computersMove) */
                        int rowLen = 1;
                        int xCheck = x + moves[i][0];
                        int yCheck = y + moves[i][1];
                        /*as long as the new field is valid
                         * and the same colour as the original field */
                        while(xCheck < 15 && xCheck > 0 && yCheck < 15 &&
                              yCheck > 0
                              && board[x][y] == board[xCheck][yCheck]){
                            rowLen++;
                            xCheck = x + (rowLen*moves[i][0]);
                            yCheck = y + (rowLen*moves[i][1]);

                        }
                        if(rowLen>chains[x][y]){
                            chains[x][y]=rowLen;
                            /*update chain numbers upstream if the stone's ID
                             * is not higher already */
                            for (int jj = rowLen;jj > 0; jj--) {
                               int xToUpdate = x + ((jj-1)*moves[i][0]);
                               int yToUpdate = y+((jj-1)*moves[i][1]);
                               if(chains[xToUpdate][yToUpdate] < rowLen){
                                   chains[xToUpdate][yToUpdate] = rowLen;
                               }
                            }
                        }
                        if(rowLen >= 5){
                            emit foundWinner(type, turnCount);
                            //show winning combination
                            for (;rowLen > 0; rowLen--) {
                                emit pieceChanged(x + ((rowLen-1)*moves[i][0]),
                                        y + ((rowLen-1)*moves[i][1]), type+2);
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
            computersMove();
        }

        /*if 3rd turn of Renju is played there's a notification reminding
         * the second player to pick their colour now */
        if(gameMode == 3 && turnCount == 3){
            emit displayMessage("Player 2's turn to pick their colour. \
                                    Next turn will be white");
        }
    }

}

void GameLogic::setColourAndMode(bool mbeginningColour, int mgameMode)
{
    beginningColour = mbeginningColour;
    gameMode = mgameMode;
}

void GameLogic::computersMove()
{
    /*make value matrix which will value:
     * NOTE: having a defense and an offense rating could help further
     * opponent's chains (starting at a length of 3 they need to be blocked):
     *  - length of 1: rating 2
     *  - length of 2: rating 2
     *  - length of 3: rating 5
     *  - length of 4: rating 5 (only completing a chain is higher)
     * unused fields that would continue a chain (space needs to be free):
     *  - 0 existing stones: rating 1
     *  - 1 existing stone: rating 2
     *  - 2 existing stones: rating 3
     *  - 3 existing stones: rating 4
     *  - 4 existing stones: rating 6 (instant win, hence highest rating)
     * all other fields that don't provide enough space for a chain:
     *  - rating 0 (if all fields are 0 the game should end in a tie if the
     * computer blocks the player's chains)
     *  - rating -1 field taken
    */
    //fill with values, x and y are reset
    QVector<QVector<int>> ratings (15, QVector<int>(15));
    for(int y=0; y<15; y++){
         for (int x=0; x<15; x++) {
             switch (board[x][y]) {
                case 0:{ // {} needed due to variable declaration in case
                    //field not in any chain, surroundings need to be checked
                    static int moves[8][2] = {{-1,-1},{0,-1},{1,-1},
                                              {-1,0},{1,0},{-1,1},{0,1},{1,1}};
                    int rating = 0;
                    for (int i=0; i<8; i++) {
                        /*row length is 0 by default (different to similar
                         * algorithm in processMove) */
                        int rowLen = 0;
                        int xToCheck = x + moves[i][0];
                        int yToCheck = y + moves[i][1];
                        //original coordinates
                        int xo = xToCheck;
                        int yo = yToCheck;
                        QVector<QVector<int>> gaps;
                        /*as long as the new field is valid and the same colour
                         *as the first adjacent field in the current direction*/
                        while(xToCheck < 15 && xToCheck > 0 && yToCheck < 15
                              && yToCheck > 0
                              && board[xo][yo] == board[xToCheck][yToCheck]
                              && board[xToCheck][yToCheck] != 0){
                            rowLen++;
                            xToCheck = x + ((rowLen+1)*moves[i][0]);
                            yToCheck = y + ((rowLen+1)*moves[i][1]);
                            /*detect complete-in-the-middle chains,
                             * there could be very combinations of gaps such as
                             * 1 stone 1 free 2 stones 1 free 1 stone where in
                             * order to intercept awareness of every gap is
                             * required */
                            if(board[xToCheck][yToCheck] == 0){
                                //see what's behind the gap
                                int tmpRowLen = rowLen + 1;
                                int xjmp = x + ((tmpRowLen+1)*moves[i][0]);
                                int yjmp = y + ((tmpRowLen+1)*moves[i][1]);
                                /*if chain does continue after the gap
                                 * (only relevant for player's colour */
                                if(xjmp < 15 && xjmp > 0
                                      && yjmp < 15 && yjmp > 0
                                      && board[xo][yo] == board[xjmp][yjmp]
                                      && board[xo][yo] == !beginningColour + 1){
                                    /*save the gap position, it's needed to give
                                     * it the right rating later on */
                                    gaps.append({xToCheck, yToCheck});
                                    //continue scanning the regular way
                                    xToCheck = xjmp;
                                    yToCheck = yjmp;
                                    rowLen = tmpRowLen;
                                }
                            }
                        }
                        //get a temporary rating which might be applied
                        int tempRating = 0;
                        /*determine whose chain it is
                         * (remember different colour coding) */
                        if(board[xo][yo] - 1 == !beginningColour){
                            //player's chain
                            switch (rowLen-gaps.length()) {
                                case 1:
                                case 2:
                                    tempRating = 2;
                                    break;
                                case 3:
                                case 4:
                                    tempRating = 5;
                                    break;
                            }

                            /*correct the rating if the other end of the chain
                             * is blocked
                             * (as it can be blocked by just one stone)*/
                            if(board[xToCheck][yToCheck] -1 ==
                                    beginningColour && rowLen !=4){
                                tempRating--;
                            }

                            //apply gap ratings
                            for(int i = 0; i < gaps.length(); i++){
                                //apply chain end rating to the gap as well
                                ratings[gaps[i][0]][gaps[i][1]] = tempRating;
                                if(rowLen==5){
                                    ratings[gaps[i][0]][gaps[i][1]] = 5;
                                }
                            }

                        }else if(board[xo][yo] - 1 == beginningColour) {
                            //computer's chain
                            switch (rowLen) {
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

                            /*correct the rating if the other
                             * end of the chain is blocked */
                            if(board[xToCheck][yToCheck] -1 ==
                                    !beginningColour && rowLen !=4){
                                tempRating--;
                            }
                        }
                        //apply tempRating (if bigger than old rating)
                        if(tempRating > rating){
                            rating = tempRating;
                        }
                    }
                    //0 chain length is irrelevant
                    if(rating > 0 && ratings[x][y] < rating){
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
    //emit valueTableUpdated(ratings);
    int highestX = 0;
    int highestY = 0;
    for (int x = 0; x<15; x++) {
        for(int y = 0; y<15; y++){
            if(ratings[x][y] > ratings[highestX][highestY]){
                highestX = x;
                highestY = y;
            }
        }
    }
    emit computerTurnDecided(highestX, highestY);
}
