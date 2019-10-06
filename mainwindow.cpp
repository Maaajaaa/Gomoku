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

void MainWindow::gameModeSelected(int gameMode)
{
    //clear window
    this->setCentralWidget(nullptr);
    this->setFixedSize(860,890);
    BoardWidget *mBoardWidget = new BoardWidget(this, gameMode);
    this->setCentralWidget(mBoardWidget);
    connect(mBoardWidget, SIGNAL(backToMainMenu()), this, SLOT(showMainMenu()));
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
