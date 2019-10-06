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
    bool inTopTen(int scoreCount);
    int addToScoreBoard(int score, QString name);

private:
    void saveModelToCSV();
    Ui::HighscoreDialog *ui;    
    QStandardItemModel *model = new QStandardItemModel;
};

#endif // HIGHSCOREDIALOG_H
