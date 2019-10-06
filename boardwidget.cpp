#include "boardwidget.h"
#include "ui_boardwidget.h"

#include <QInputDialog>

BoardWidget::BoardWidget(QWidget *parent, int mGameMode) :
    QWidget(parent),
    ui(new Ui::BoardWidget)
{
    mParent = parent;
    ui->setupUi(this);

    for(int x=0; x<15; x++){
        for(int y=0; y<15; y++){
            goPiece* testPiece = new goPiece();
            goPieces.append(testPiece);
            connect(testPiece, SIGNAL(clicked(goPiece*)), this,
                    SLOT(newPieceSet(goPiece*)));
            this->ui->gridLayout->addWidget(testPiece,x,y);
        }
    }

    //show to get right geometry next
    parent->show();
    this->adjustSize();

    //draw board
    QPixmap *pix = new QPixmap(this->size());
    QPainter *paint = new QPainter(pix);
    //set background colour
    pix->fill(QColor(193,190,174));
    //line colour
    paint->setPen(QColor(12,12,12));
    //generate grid lines
    int halfWidth = goPieces.at(0)->geometry().width()/2;
    for(int x=0;x<15;x++){
        //vertical grid lines
        paint->drawLine(
            ui->gridLayout->itemAtPosition(x,0)->geometry().y()+halfWidth,
            ui->gridLayout->itemAtPosition(x,0)->geometry().x(),
            ui->gridLayout->itemAtPosition(x,14)->geometry().y()+halfWidth,
            ui->gridLayout->itemAtPosition(x,14)->geometry().x()+2*halfWidth);
        //horizontal grid lines
        paint->drawLine(
            ui->gridLayout->itemAtPosition(0,x)->geometry().y(),
            ui->gridLayout->itemAtPosition(0,x)->geometry().x()+halfWidth,
            ui->gridLayout->itemAtPosition(14,x)->geometry().y()+2*halfWidth,
            ui->gridLayout->itemAtPosition(14,x)->geometry().x()+halfWidth);
    }
    this->adjustSize();

    QPalette palette;
    //apply background
    palette.setBrush(QPalette::Background, pix->scaled(this->size(),
                                                       Qt::IgnoreAspectRatio));
    parent->setPalette(palette);

    //connect game logic to UI interaction
    connect(this, SIGNAL(sendMoveToLogic(int,int,int)),
            gamelog,SLOT(processMove(int,int,int)));
    connect(gamelog,SIGNAL(pieceChanged(int,int,int)),
            this, SLOT(showMoveOnBoard(int,int,int)));
    connect(gamelog, SIGNAL(foundWinner(int, int)),
            this, SLOT(showWinner(int, int)));
    connect(gamelog, SIGNAL(valueTableUpdated(QVector<QVector<int>>)),
            this, SLOT(displayValuesOnBoard(QVector<QVector<int>>)));
    connect(gamelog, SIGNAL(computerTurnDecided(int,int)),
            this, SLOT(computerTurn(int,int)));
    connect(gamelog, SIGNAL(displayMessage(QString)),
            this, SLOT(showMessage(QString)));


    //set gameMode given by parent
    gameMode = mGameMode;
    qDebug() << mGameMode;

    //ask for begining colour
    if(gameMode != 2){
        QMessageBox msgBox;
        if(gameMode == 0){
            msgBox.setText("Select your colour.");
        }else if(gameMode == 1){
            msgBox.setText("Select begining colour.");
        }
        msgBox.setInformativeText("Black or white?");
        msgBox.addButton(tr("Black"),QMessageBox::NoRole);
        msgBox.addButton(tr("White"),QMessageBox::YesRole);
        //0 - black 1 - white (different to colour-coding in Gamelogic)
        bool selectedColour = msgBox.exec();
        if(gameMode == 0){
            if(selectedColour == 1)beginningColour = 0;
            else if(selectedColour == 0)beginningColour = 1;
        }
    }
    gamelog->setColourAndMode(beginningColour,gameMode);
    lastMove = beginningColour;
    //set first piece
    emit sendMoveToLogic(7,7,beginningColour+1);
}


void BoardWidget::showMoveOnBoard(int x, int y, int type)
{
    goPieces.at(y*15+x)->setUse(type);
    lastMove = type -1;
}

void BoardWidget::computerTurn(int x, int y)
{
    emit sendMoveToLogic(x, y, beginningColour+1);
}

void BoardWidget::displayValuesOnBoard(QVector<QVector<int>> values)
{
    for(int y=0;y<15;y++){
        for (int x=0;x<15;x++) {
            goPieces.at(y*15+x)->setTextAndPixmap(
                        QString::number(values[x][y]));
        }
    }
}

void BoardWidget::newPieceSet(goPiece* piece)
{
    int index = goPieces.indexOf(piece);
    emit sendMoveToLogic(index%15, index/15, !lastMove+1);
}

void BoardWidget::showWinner(int type, int turnCount)
{
    QString winner;
    if(type == 1){
        winner = "Black";
    }else if (type == 2) {
        winner = "White";
    }
    QMessageBox::warning(this, "Game finished", winner +
                                   " won! Within " + QString::number(turnCount)
                                   + " turns.");


    //track highscore
    HighscoreDialog *highScoreDialog = new HighscoreDialog();
    //if eligable for top ten, show dialog and to ask for name
    if( gameMode != 0 && highScoreDialog->inTopTen(turnCount)){
        QString nickName = QInputDialog::getText(this,
                                                 tr("You are in the Top 10!"),
                                                 tr("Enter your nickname for /"
                                                    "the highscore: "),
                                                 QLineEdit::Normal);
        highScoreDialog->addToScoreBoard(turnCount, nickName);
    }
}

void BoardWidget::showMessage(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}


BoardWidget::~BoardWidget()
{
    delete ui;
}

void BoardWidget::on_backToMainMenuPushButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setInformativeText("Are you sure you want to go back to Main Menu");
    msgBox.addButton(tr("Nope"),QMessageBox::NoRole);
    msgBox.addButton(tr("Absolutely"),QMessageBox::YesRole);
    bool goBack = msgBox.exec();
    if(goBack){
        //overdraw board
        QPixmap *pix = new QPixmap(this->size());
        //set background colour
        pix->fill(QColor(220,220,220));
        QPalette palette;
        //apply background
        palette.setBrush(QPalette::Background,
                         pix->scaled(this->size(), Qt::IgnoreAspectRatio));
        mParent->setPalette(palette);

        emit backToMainMenu();
    }
}
