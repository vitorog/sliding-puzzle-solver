#include "board_widget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <QPixmap>

#define SQUARE_SIZE 100

BoardWidget::BoardWidget(PuzzleBoard *board,bool locked,int img, QWidget *parent) :
    QWidget(parent)
{
    m_board = board;
    m_painter = new QPainter();
    m_img_index = img;
    load_images();
    m_locked = locked;   
}

BoardWidget::~BoardWidget(){
    delete m_painter;
    m_pieces_image.clear();
}

//Load the images that will be displayed in the board
void BoardWidget::load_images(){
    int size = m_board->get_size();
    QImage image;  
    QString img_path;
    QString type;
    switch(m_img_index){
        case 0:
            img_path = ":/images/awesome";
            type = ".jpg";
            break;
        case 1:
            img_path = ":/images/troll_face";
            type = ".png";
            break;
        case 2:
            img_path = ":/images/rage";
            type = ".jpg";
            break;
    }

    switch(size){
    case 3:
        img_path.append("3x3");
        break;
    case 4:
        img_path.append("4x4");
        break;
    case 5:
        img_path.append("5x5");
        break;
    }
    img_path.append(type);
    image.load(img_path);
    m_pieces_image.clear();
    m_pieces_image.resize(size*size);
    for(int i = 0; i < size; i++){
         for(int j = 0; j < size; j++){         
          m_pieces_image[i + (size*j)] = image.copy(QRect(i*SQUARE_SIZE,j*SQUARE_SIZE,SQUARE_SIZE,SQUARE_SIZE));
         }
    }
    update();
}

//Locks the board so it does not allow user to move the pieces
void BoardWidget::lock_board(bool lock){
    m_locked = lock;
}

//Change the board's image
void BoardWidget::change_image(int img){
    m_img_index = img;
    m_pieces_image.clear();
    load_images();
}


void BoardWidget::paintEvent(QPaintEvent *e){
    m_painter->begin(this);  
    QPen gap_pen;
    gap_pen.setColor(QColor(200,200,200));
    m_painter->setBackgroundMode(Qt::OpaqueMode);
    int board_size = m_board->get_size();
    int image_index = 0;
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            image_index = m_board->piece(i,j)->m_value-1;
            if(m_board->piece(i,j)->m_value==0){
                m_painter->fillRect(i*SQUARE_SIZE,j*SQUARE_SIZE,SQUARE_SIZE,SQUARE_SIZE,gap_pen.brush());
            }else{
               m_painter->drawImage(QRect(SQUARE_SIZE*i,SQUARE_SIZE*j,SQUARE_SIZE,SQUARE_SIZE),m_pieces_image[image_index]);
               m_painter->drawLine(i*SQUARE_SIZE,j*SQUARE_SIZE,board_size*SQUARE_SIZE,j*SQUARE_SIZE);
               m_painter->drawLine(i*SQUARE_SIZE,j*SQUARE_SIZE,i*SQUARE_SIZE,board_size*SQUARE_SIZE);
            }
        }
    }
    m_painter->drawLine(0,board_size*SQUARE_SIZE,board_size*SQUARE_SIZE,board_size*SQUARE_SIZE);
    m_painter->drawLine(board_size*SQUARE_SIZE,0,board_size*SQUARE_SIZE,board_size*SQUARE_SIZE);
    m_painter->end();
}

void BoardWidget::mousePressEvent(QMouseEvent *e){
    if(!m_locked){
        int mouse_x;
        int mouse_y;
        mouse_x = e->x();
        mouse_y = e->y();
        m_board->move_piece(mouse_x / SQUARE_SIZE, mouse_y / SQUARE_SIZE);
        emit piece_moved();
        update();
    }
}

//Updates the current board being displayed with another board
void BoardWidget::set_curr_board(PuzzleBoard *board){
    this->m_board = board;
    update();
}
