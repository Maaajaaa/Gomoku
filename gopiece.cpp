#include "gopiece.h"


goPiece::goPiece(QWidget *parent){
    this->setPixmap(QPixmap(QString(":resources/noPiece.png")).scaledToWidth(50));
}

void goPiece::setUse(int type){
    switch (type) {
    //not used/transparent
    case 0:
        this->setPixmap(QPixmap(QString(":resources/noPiece.png")).scaledToWidth(50));
        break;
    case 1:
        this->setPixmap(QPixmap(QString(":resources/blackPiece.png")).scaledToWidth(50));
        break;
    case 2:
        this->setPixmap(QPixmap(QString(":resources/whitePiece.png")).scaledToWidth(50));
        break;
    case 3:
        this->setPixmap(QPixmap(QString(":resources/blackWinningPiece.png")).scaledToWidth(50));
        break;
    case 4:
        this->setPixmap(QPixmap(QString(":resources/whiteWinningPiece.png")).scaledToWidth(50));
        break;

    }
    useID=type;
}

int goPiece::getUse(){
    return useID;
}

void goPiece::cycleModes(){
    int currentMode = getUse();
    currentMode++;
    if(currentMode !=3){
        setUse(currentMode);
    }
    else {
        setUse(0);
    }
}
