#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gopiece.h"
#include <QPainter>
#include <QtDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    //this->setStyleSheet("background-color: blue;");

    this->setWindowTitle("Gomoku");
    GameLogic *gamelog = new GameLogic();

    for(int x=0; x<15; x++){
        for(int y=0; y<15; y++){
            goPiece* testPiece = new goPiece();
            goPieces.append(testPiece);
            connect(testPiece, SIGNAL(clicked(goPiece*)), this, SLOT(newPieceSet(goPiece*)));
           this->ui->gridLayout_2->addWidget(testPiece,x,y);
        }
    }

    //show to get right geometry next
    ui->mainWidget->show();
    this->show();

    //draw board
    QPixmap *pix = new QPixmap(this->size());
    QPainter *paint = new QPainter(pix);
    //set background colour
    pix->fill(QColor(193,190,174));
    //line colour
    paint->setPen(QColor(12,12,12));
    //generate grid lines
    int halfWidth = goPieces.at(0)->geometry().width()/2;
    qDebug() << this->size();
    for(int x=0;x<15;x++){
        //vertical grid lines
        paint->drawLine(
                    ui->gridLayout_2->itemAtPosition(x,0)->geometry().y()+halfWidth,
                    ui->gridLayout_2->itemAtPosition(x,0)->geometry().x(),
                    ui->gridLayout_2->itemAtPosition(x,14)->geometry().y()+halfWidth,
                    ui->gridLayout_2->itemAtPosition(x,14)->geometry().x()+2*halfWidth);
        //horizontal grid lines
        paint->drawLine(
                    ui->gridLayout_2->itemAtPosition(0,x)->geometry().y(),
                    ui->gridLayout_2->itemAtPosition(0,x)->geometry().x()+halfWidth,
                    ui->gridLayout_2->itemAtPosition(14,x)->geometry().y()+2*halfWidth,
                    ui->gridLayout_2->itemAtPosition(14,x)->geometry().x()+halfWidth);
    }

    QPalette palette;
    //set board as background
    palette.setBrush(QPalette::Background, pix->scaled(this->size(), Qt::IgnoreAspectRatio));
    this->setPalette(palette);

    //connect game logic to UI interaction
    connect(this, SIGNAL(sendMoveToLogic(int,int,int)),gamelog,SLOT(processMove(int,int,int)));
    connect(gamelog,SIGNAL(pieceChanged(int,int,int)), this, SLOT(showMoveOnBoard(int,int,int)));
    connect(gamelog, SIGNAL(foundWinner(int)), this, SLOT(showWinner(int)));
    connect(gamelog, SIGNAL(valueTableUpdated(QVector<QVector<int>>)), this, SLOT(displayValuesOnBoard(QVector<QVector<int>>)));
    connect(gamelog, SIGNAL(computerTurnDecided(int,int)), this, SLOT(computerTurn(int,int)));
    connect(gamelog, SIGNAL(displayMessage(QString)), this, SLOT(showMessage(QString)));

    //determine single-player/multi player mode
    QMessageBox playerBox;
    playerBox.setText("Select the Game mode");
    playerBox.setInformativeText("What Game wouldt you like?");
    playerBox.addButton(tr("Single Player (computer begins)"),QMessageBox::NoRole);
    playerBox.addButton(tr("Multi Player"),QMessageBox::YesRole);
    gameMode = playerBox.exec();

    //If single-player ask for Go-Moku or Renju
    if(gameMode == 1){
        QMessageBox renjuBox;
        renjuBox.setText("Select the Game");
        renjuBox.setInformativeText("Go-Moku is classic but not fair since beginning player has much better odds. Renju is more balanced.");
        renjuBox.addButton(tr("Go-Moku"),QMessageBox::NoRole);
        renjuBox.addButton(tr("Renju"),QMessageBox::YesRole);
        bool renju = renjuBox.exec();
        if(renju){
            gameMode = 3;
            //in Renju black always begins
            beginningColour = 0;
        }
    }

    //ask for begining colour
    if(gameMode != 3){
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

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::showMoveOnBoard(int x, int y, int type)
{
    qDebug() << goPieces.at(y*15+x)->text();
    goPieces.at(y*15+x)->setUse(type);
    ///TODO: fix this properly
    //goPieces.at(y*15+x)->setEnabled(false);
    lastMove = type -1;
}

void MainWindow::computerTurn(int x, int y)
{
    //lastMove = !lastMove;
    emit sendMoveToLogic(x, y, beginningColour+1);
}

void MainWindow::displayValuesOnBoard(QVector<QVector<int>> values)
{
    //qDebug() << values[7][7];
    for(int y=0;y<15;y++){
        for (int x=0;x<15;x++) {
            goPieces.at(y*15+x)->setTextAndPixmap(QString::number(values[x][y]));
        }
    }
}

void MainWindow::newPieceSet(goPiece* piece)
{
    int index = goPieces.indexOf(piece);
    emit sendMoveToLogic(index%15, index/15, !lastMove+1);
}

void MainWindow::showWinner(int type)
{
    QString winner;
    if(type == 1){
        winner = "Black";
    }else if (type == 2) {
        winner = "White";
    }
    int ret = QMessageBox::warning(this, "Game finished",
                                   winner + " won!");
    ///TODO: new game
}

void MainWindow::showMessage(QString message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}
