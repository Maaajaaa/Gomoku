#ifndef GOPIECE_H
#define GOPIECE_H

#include <QLabel>

class goPiece : public QLabel
{
    Q_OBJECT
public:
    goPiece(QWidget *parent = nullptr);
    void setUse(int type);
    int getUse();
    void setTextAndPixmap(QString text);
    ~goPiece(){};

signals:
    void clicked(goPiece*);

public slots:

    void cycleModes();

private:
    int useID=0;

protected:
    void mousePressEvent(QMouseEvent *event) { emit clicked(this); }
 };


#endif // GOPIECE_H
