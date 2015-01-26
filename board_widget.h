#ifndef BOARD_WIDGET_H
#define BOARD_WIDGET_H

#include <QWidget>
#include "puzzle_board.h"

/* This widget get a puzzle boar and paint it on the screen */

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(PuzzleBoard *board,bool locked = false, int img =  0, QWidget *parent = 0);
    void set_curr_board(PuzzleBoard*);
    ~BoardWidget();

signals:
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
public slots:
    void change_image(int img);
    void lock_board(bool lock);
signals:
    void piece_moved();
private:    
    void load_images();
    PuzzleBoard *m_board;
    QPainter *m_painter;
    QVector<QImage> m_pieces_image;
    int m_pieces_size;
    int m_img_index;
    bool m_locked;
};

#endif // BOARD_WIDGET_H
