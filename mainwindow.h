#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <gamelogic.h>
#include <gopiece.h>
#include <highscoredialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void showCentralWidget();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGridLayout *mainGrid;
    GameLogic gamelogic;    
    QVector<goPiece*> goPieces;
    bool lastMove = false;  //colour that had last turn
    int gameMode = 0; //0 - single player, 1 - multiplayer 2- renju (single-player)
    bool beginningColour = 0; //0 - black, 1 - white
};

#endif // MAINWINDOW_H
