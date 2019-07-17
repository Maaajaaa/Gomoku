#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gopiece.h"
#include <QPainter>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    //this->setStyleSheet("background-color: blue;");

    this->setWindowTitle("Gomoku");
    QVector<goPiece*> goPieces;


    for(int x=0; x<15; x++){
        for(int y=0; y<15; y++){
            goPiece* testPiece = new goPiece();
            connect(testPiece, SIGNAL(clicked()), testPiece, SLOT(cycleModes()));

            goPieces.append(testPiece);
           this->ui->gridLayout_2->addWidget(testPiece,x,y);
        }
    }

    //show to get right geometry next
    ui->mainWidget->show();
    this->show();
    QPixmap *pix = new QPixmap(this->size());
    QPainter *paint = new QPainter(pix);
    pix->fill(QColor(193,190,174));
    paint->setPen(QColor(12,12,12));

    int halfWidth = goPieces.at(0)->geometry().width()/2;
    int margin=ui->gridLayout_2->margin();
    qDebug() << this->size();
    for(int x=0;x<15;x++){
        //vertical gridlines
        paint->drawLine(
                    ui->gridLayout_2->itemAtPosition(x,0)->geometry().y()+halfWidth,
                    ui->gridLayout_2->itemAtPosition(x,0)->geometry().x(),
                    ui->gridLayout_2->itemAtPosition(x,14)->geometry().y()+halfWidth,
                    ui->gridLayout_2->itemAtPosition(x,14)->geometry().x()+2*halfWidth);
        //horizontal gridlines
        paint->drawLine(
                    ui->gridLayout_2->itemAtPosition(0,x)->geometry().y(),
                    ui->gridLayout_2->itemAtPosition(0,x)->geometry().x()+halfWidth,
                    ui->gridLayout_2->itemAtPosition(14,x)->geometry().y()+2*halfWidth,
                    ui->gridLayout_2->itemAtPosition(14,x)->geometry().x()+halfWidth);
    }


    //paint->drawLine(QLine(QPoint(21,11),QPoint(11,375)));
    QPalette palette;
    QPixmap pixie = pix->scaled(this->size(), Qt::IgnoreAspectRatio);
    palette.setBrush(QPalette::Background, pixie);
    this->setPalette(palette);
    /*Algorith for checking the rows:
     * iterate over the list, check every item for connected rows, to to the end, write score into new table. iterate on and try to find longer lists
     * 10111  gives     2 2 3 3 3
     * 01000            2 2 3 3 3
     * technically no complete recounting is required, the new pieces and ajdcacents should suffice
     */
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    qDebug() << ui->mainWidget->geometry();
}
