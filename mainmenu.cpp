#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    HighscoreDialog *mHighscoreDialog = new HighscoreDialog(this);
    //ui->horizontalLayout->addWidget(mHighscoreDialog);
    ui->highScore_layout->addWidget(mHighscoreDialog);
}

MainMenu::~MainMenu()
{
    delete ui;
}

//return selected game mode to main window

void MainMenu::on_multiPlayerPushButton_clicked()
{
    //0 - single player, 1 - multiplayer 2- renju (single-player)
    emit gameModeSelected(1);
}

void MainMenu::on_renjuPushButton_clicked()
{
    emit gameModeSelected(2);
}

void MainMenu::on_SinglePlayerPushButton_clicked()
{
    emit gameModeSelected(0);
}
