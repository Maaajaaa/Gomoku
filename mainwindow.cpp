#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    this->setWindowTitle("Gomoku");


    MainMenu *mMainMenu = new MainMenu(this);
    this->setCentralWidget(mMainMenu);
    this->centralWidget()->show();
    connect(mMainMenu, SIGNAL(gameModeSelected(int)), this, SLOT(gameModeSelected(int)));

}

void MainWindow::showCentralWidget()
{
}

MainWindow::~MainWindow(){
    delete ui;
}

//open
void MainWindow::gameModeSelected(int gameMode)
{
    //clear window
    qDebug() << gameMode;
    this->setCentralWidget(nullptr);
    BoardWidget *mBoardWidget = new BoardWidget(this, gameMode);
    this->setCentralWidget(mBoardWidget);
    this->centralWidget()->adjustSize();

    this->centralWidget()->show();
    mBoardWidget->show();
}
