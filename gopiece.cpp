#include "gopiece.h"


goPiece::goPiece(QWidget *parent){
    this->setPixmap(QPixmap(QString(":resources/noPiece.png"))
                    .scaledToWidth(50));
}

void goPiece::setUse(int type){
    switch (type) {
    //not used/transparent
    case 0:
        this->setPixmap(QPixmap(QString(":resources/noPiece.png"))
                        .scaledToWidth(50));
        break;
    case 1:
        this->setPixmap(QPixmap(QString(":resources/blackPiece.png"))
                        .scaledToWidth(50));
        break;
    case 2:
        this->setPixmap(QPixmap(QString(":resources/whitePiece.png"))
                        .scaledToWidth(50));
        break;
    case 3:
        this->setPixmap(QPixmap(QString(":resources/blackWinningPiece.png"))
                        .scaledToWidth(50));
        break;
    case 4:
        this->setPixmap(QPixmap(QString(":resources/whiteWinningPiece.png"))
                        .scaledToWidth(50));
        break;

    }
    useID=type;
}

int goPiece::getUse(){
    return useID;
}

void goPiece::setTextAndPixmap(QString text)
{
    QString html1 = QString("<html><style>.container {  /"
                            "position: relative; text-align: center;  /"
                            "color: white;}.centered /"
                            "{ position: absolute; top: 50%; left: 50%;/"
                            "transform: translate(-50%, -50%);}</style> /"
                            "<div class='container'>  /"
                            "<img src=':resources/");
    QString html2 = QString("' width='50'> /"
                            "<div class='centered'><font color='red'>"
                            + text + "</font></div></div> </html>");
    switch (useID) {
        case 0:
            this->setText(html1 + QString("noPiece.png") + html2);
            break;
        case 1:
            this->setText(html1 + QString("blackPiece.png") + html2);
            break;
        case 2:
            this->setText(html1 + QString("whitePiece.png") + html2);
            break;
        case 3:
            this->setText(html1 + QString("blackWinningPiece.png") + html2);
            break;
        case 4:
            this->setText(html1 + QString("whiteWinningPiece.png") + html2);
            break;
    }
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
