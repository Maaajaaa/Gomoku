#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include "highscoredialog.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

signals:
    void gameModeSelected(int gameMode);

private slots:
    void on_multiPlayerPushButton_clicked();

    void on_renjuPushButton_clicked();

    void on_SinglePlayerPushButton_clicked();

private:
    Ui::MainMenu *ui;
};

#endif // MAINMENU_H
