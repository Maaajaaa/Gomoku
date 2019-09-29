#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gopiece.h"
#include "boardwidget.h"
#include <QPainter>
#include <QtDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    this->setWindowTitle("Gomoku");

    HighscoreDialog *highdiag = new HighscoreDialog();
    //highdiag->show();

    BoardWidget *mBoardWidget = new BoardWidget(this);
    //mBoardWidget->show();
    this->setCentralWidget(mBoardWidget);
    this->centralWidget()->show();
}

void MainWindow::showCentralWidget()
{
}

MainWindow::~MainWindow(){
    delete ui;
}
