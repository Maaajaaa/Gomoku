#ifndef HIGHSCOREDIALOG_H
#define HIGHSCOREDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QFile>
#include <QStringList>

namespace Ui {
class HighscoreDialog;
}

class HighscoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HighscoreDialog(QWidget *parent = nullptr);
    ~HighscoreDialog();

private:
    Ui::HighscoreDialog *ui;
};

#endif // HIGHSCOREDIALOG_H
