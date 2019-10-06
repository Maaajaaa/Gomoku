#include "highscoredialog.h"
#include "ui_highscoredialog.h"
#include <QtDebug>
#include <ui_highscoredialog.h>

HighscoreDialog::HighscoreDialog(QWidget *parent) :
    ui(new Ui::HighscoreDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Highscores");
    //model->setHorizontalHeaderItem(0, new QStandardItem("Rank"));
    model->setHorizontalHeaderItem(0, new QStandardItem("Name"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Turns to win"));

    QFile highscoreFile("highscore.csv");
    if(highscoreFile.open(QIODevice::ReadOnly)){
        //fill highscore table (current max 10 scores)
        for (int x=0; x<10; x++) {
            //read one line
            QString line =highscoreFile.readLine();
            if(line != "" && line != "\n" && line != "\r" && line !="\r\n"){
                //separate entries
                QStringList rowItems = line.split(";", QString::SkipEmptyParts);
                //fill row
                for (int y=0; y<2; y++) {
                    //simplified removes uneccesary whitespaces at the ends
                    QStandardItem *item =
                            new QStandardItem(rowItems.at(y).simplified());
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
    //delete ui;
}

/*returns place within top ten,
-1 if not within top 10 (as 11th place might be higher but not tracked)*/
bool HighscoreDialog::inTopTen(int score)
{
    //check if candidate fits in, smaller score is better
    if(model->rowCount() < 10
            || model->item(model->rowCount()-1,1)->text().toInt() > score){
        return true;
    }
    else {
        return false;
    }
}

int HighscoreDialog::addToScoreBoard(int score, QString name)
{
    //check if candidate fits in, smaller score is better
    if(model->rowCount() < 10
            || model->item(model->rowCount()-1,1)->text().toInt() > score)
    {
        //check the candidates place
        int i = 0;
        while(i<model->rowCount() -1 && model->item(i,1)->text().toInt()
              < score){
            i++;
        }

        QStringList newRowList = {name, QString::number(score)};
        QList<QStandardItem*> itemList;
        for (int y=0; y<2; y++) {
            QStandardItem *item = new QStandardItem(newRowList.at(y));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            itemList.append(item);
        }
        model->insertRow(i,itemList);
        //if more than 10 delete 11th entry
        if(model->rowCount() == 11){
            model->removeRows(11,1);
        }
        saveModelToCSV();
        //return the rank
        return i+10;
    }else {
        //-1 meaning not withing top 10
        return -1;
    }
}

void HighscoreDialog::saveModelToCSV()
{
    QFile highscoreFile("highscore.csv");
    //delete old highscores
    highscoreFile.resize(0);
    //make new CSV
    if(highscoreFile.open(QIODevice::ReadWrite)){
        QTextStream stream(&highscoreFile);
        //put each row into a CSV-style line
        for (int x=0; x<model->rowCount(); x++) {
            stream << model->item(x,0)->text() << ";"
                   << model->item(x,1)->text() << "\n";
        }
        //save changes
        highscoreFile.close();
    }
}

