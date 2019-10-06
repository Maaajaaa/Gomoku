#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    this->setWindowTitle("Gomoku");
    showMainMenu();
}

MainWindow::~MainWindow(){
    delete ui;
}

//open
void MainWindow::gameModeSelected(int gameMode)
{
    //clear window
    this->setCentralWidget(nullptr);
    BoardWidget *mBoardWidget = new BoardWidget(this, gameMode);
    this->setCentralWidget(mBoardWidget);
    connect(mBoardWidget, SIGNAL(backToMainMenu()), this, SLOT(showMainMenu()));
    /*this->centralWidget()->adjustSize();

    this->centralWidget()->show();
    mBoardWidget->show();*/
}

void MainWindow::showMainMenu()
{
    this->setCentralWidget(nullptr);
    MainMenu *mMainMenu = new MainMenu(this);
    this->setCentralWidget(mMainMenu);
    this->centralWidget()->show();
    connect(mMainMenu, SIGNAL(gameModeSelected(int)),
            this, SLOT(gameModeSelected(int)));

}
