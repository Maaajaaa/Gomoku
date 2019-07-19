#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <gamelogic.h>
#include <gopiece.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showMoveOnBoard(int x, int y, int type);
    void newPieceSet(goPiece*);
    void showWinner(int type);

signals:
    void sendMoveToLogic(int x, int y, int type);

private:
    Ui::MainWindow *ui;
    QGridLayout *mainGrid;
    GameLogic gamelogic;    
    QVector<goPiece*> goPieces;
    //colour that had last turn
    bool lastMove;
};

#endif // MAINWINDOW_H
