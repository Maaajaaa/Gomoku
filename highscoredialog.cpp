#include "highscoredialog.h"
#include "ui_highscoredialog.h"
#include <QtDebug>

HighscoreDialog::HighscoreDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HighscoreDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Highscores");
    QStandardItemModel *model = new QStandardItemModel;
    model->setHorizontalHeaderItem(0, new QStandardItem("Rank"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Name"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Turns to win"));

    QFile highscoreFile("highscore.csv");
    if(highscoreFile.open(QIODevice::ReadWrite)){
        //fill highscore table (current max 10 scores)
        for (int x=0; x<10; x++) {
            //read one line
            QString line =highscoreFile.readLine();
            if(line != "" && line != "\n"){
                //separate entries
                QStringList rowItems = line.split(";", QString::SkipEmptyParts);
                //fill row
                for (int y=0; y<3; y++) {
                    QStandardItem *item = new QStandardItem(rowItems.at(y));
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                    model->setItem(x,y,item);
                }
            }

        }
    }
    highscoreFile.close();
    ui->tableView->setModel(model);
}

HighscoreDialog::~HighscoreDialog()
{
    delete ui;
}
