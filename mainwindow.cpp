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
    /*Algorith for checking the rows:
     * iterate over the list, check every item for connected rows, to to the end, write score into new table. iterate on and try to find longer lists
     * 10111  gives     2 2 3 3 3
     * 01000            2 2 3 3 3
     * technically no complete recounting is required, the new pieces and ajdcacents should suffice
     */

    //connect game logic to UI interaction
    connect(this, SIGNAL(sendMoveToLogic(int,int,int)),gamelog,SLOT(processMove(int,int,int)));
    connect(gamelog,SIGNAL(pieceChanged(int,int,int)), this, SLOT(showMoveOnBoard(int,int,int)));
    connect(gamelog, SIGNAL(foundWinner(int)), this, SLOT(showWinner(int)));

    //ask for begining colour
    QMessageBox msgBox;
    msgBox.setText("Select begining colour.");
    msgBox.setInformativeText("Black or white?");
    msgBox.addButton(tr("Black"),QMessageBox::NoRole);
    msgBox.addButton(tr("White"),QMessageBox::YesRole);
    int ret = msgBox.exec();
    lastMove = ret;

    //set first piece
    emit sendMoveToLogic(7,7,ret+1);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::showMoveOnBoard(int x, int y, int type)
{
    goPieces.at(y*15+x)->setUse(type);
    goPieces.at(y*15+x)->setEnabled(false);
}

void MainWindow::newPieceSet(goPiece* piece)
{
    int index = goPieces.indexOf(piece);
    emit sendMoveToLogic(index%15, index/15, !lastMove+1);
    lastMove = !lastMove;
}

void MainWindow::showWinner(int type)
{
    QString winner;
    if(type == 1){
        winner = "Black";
    }else if (type == 2) {
        winner = "White";
    }
    int ret = QMessageBox::warning(this, tr("Game finished"),
                                   winner + " won!");
}
