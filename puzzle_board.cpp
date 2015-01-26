#include "puzzle_board.h"

#include <time.h>
#include <stdlib.h>

//Construcs a puzzle piece
PuzzlePiece::PuzzlePiece(int x, int y, int value, int dir){
    m_pos_x = x;
    m_pos_y = y;
    m_value = value;
    m_movable_dir = dir;   
}

//Constructs a puzzle board
PuzzleBoard::PuzzleBoard(int board_size)
{   
    m_board_size = board_size;
    PuzzlePiece *new_piece = NULL;
    int value = 0;    
    for(int i = 0; i < m_board_size; i++){
        for(int j = 0; j < m_board_size; j++){
            value = (i + j*m_board_size)+1;
            //Initially each piece will have the value equivalent to its position
            new_piece = new PuzzlePiece(i,j, value, BLOCKED);
            //This will add the elements to the board
            m_board.push_back(*new_piece);

            delete new_piece;
            new_piece = NULL;
        }
    }   
    m_board[(m_board_size*m_board_size)-1].m_value = 0;
    update_board();
}

PuzzleBoard::~PuzzleBoard(){
    m_unique_id.clear();
    m_board.clear();
}

//Returns the corresponding piece at position x and y
PuzzlePiece* PuzzleBoard::piece(int x, int y){
    if((x >= 0) && (x < m_board_size) && (y>=0) && (y < m_board_size))
    {
        return &m_board[x*m_board_size + y];
    }
    else
    {
        return  NULL;
    }
}

//Moves random pieces num_moves times to scramble the board
void PuzzleBoard::scramble_board_with_moves(int num_moves){
    srand(time(NULL));
    std::vector<PuzzlePiece*> movable_pieces;
    PuzzlePiece *curr_piece;   
    int piece_num = 0;
    for(int i = 0; i < num_moves; i++){
        movable_pieces = this->get_movable_pieces();
        piece_num = rand() % movable_pieces.size();
        curr_piece = movable_pieces[piece_num];
        this->move_piece(curr_piece->m_pos_x,curr_piece->m_pos_y);
        movable_pieces.clear();
    }
    return;
}

//Checks each piece and updates their movable direction
void PuzzleBoard::update_board(){
    for(int i = 0; i < m_board_size; i++){
        for(int j = 0; j < m_board_size; j++){
            if(piece(i,j)->m_value == 0){
                piece(i,j)->m_movable_dir = BLOCKED;
            }else{
                piece(i,j)->m_movable_dir = check_piece_mov(i,j);
            }
        }
    }
    calculate_unique_id();
    return;
}

//Moves the piece at x,y
void PuzzleBoard::move_piece(int x, int y){
    PuzzlePiece *temp_piece = NULL;
    PuzzlePiece *curr_piece = NULL;   
    int backup_value = 0;
    int direction = check_piece_mov(x,y);
    curr_piece = piece(x,y);
    if((direction != BLOCKED) && (curr_piece->m_value != 0)){
        switch(direction){
        case LEFT:
            temp_piece = piece(x-1,y);
            break;
        case RIGHT:
            temp_piece = piece(x+1,y);
            break;
        case UP:
            temp_piece = piece(x,y-1);
            break;
        case DOWN:
            temp_piece = piece(x,y+1);
            break;
        default:
            break;
        }
        backup_value = curr_piece->m_value;
        curr_piece->m_value = temp_piece->m_value;        
        temp_piece->m_value = backup_value;
        update_board();
    }   
}

//Checks if a piece has any movable direction or if its blocked
int PuzzleBoard::check_piece_mov(int x, int y){
    if(x >= m_board_size || y >= m_board_size){
        return BLOCKED;
    }
    if(x-1 >=0){
        if(piece(x-1,y)->m_value == 0){
            return LEFT;
        }
    }
    if(x+1 < m_board_size){
        if(piece(x+1,y)->m_value == 0){
            return RIGHT;
        }
    }
    if(y+1 < m_board_size){
        if(piece(x,y+1)->m_value == 0){
            return DOWN;
        }
    }
    if(y-1 >=0){
        if(piece(x,y-1)->m_value == 0){
            return UP;
        }
    }
    return BLOCKED;
}

//Gets the size of the board
int PuzzleBoard::get_size(){
    return this->m_board_size;
}

//Returns a list of all pieces that can be moved at a given time
std::vector<PuzzlePiece*> PuzzleBoard::get_movable_pieces(){
    update_board();
    std::vector<PuzzlePiece*> mov_pieces;
    for(int i = 0; i < m_board_size; i++){
        for(int j = 0; j < m_board_size; j++){
            if(check_piece_mov(i,j) != BLOCKED){
                mov_pieces.push_back(this->piece(i,j));
            }
        }
    }
    return mov_pieces;
}

//Returns a reference to the board
const Board* PuzzleBoard::get_board(){
    return &this->m_board;
}

//Updates the board with the board contained in b
void PuzzleBoard::set_board(PuzzleBoard *b){
    std::vector<PuzzlePiece>::const_iterator iter;
    int index = 0;
    for(iter = b->get_board()->begin(); iter != b->get_board()->end(); iter++){        
        this->m_board[index].m_pos_x = (*iter).m_pos_x;
        this->m_board[index].m_pos_y = (*iter).m_pos_y;
        this->m_board[index].m_value = (*iter).m_value;
        this->m_board[index].m_movable_dir = (*iter).m_movable_dir;        
        index++;
    }
    this->calculate_unique_id();
}


bool PuzzleBoard::compare_boards(PuzzleBoard *b){
    Board_Iter iter;
    Board_Iter comp_iter;
    comp_iter = b->m_board.begin();
    PuzzlePiece *a = NULL;
    PuzzlePiece *comp = NULL;
    for(iter = m_board.begin(); iter != m_board.end(); iter++){
        a = &(*iter);
        comp = &(*comp_iter);
        if((a->m_movable_dir != comp->m_movable_dir) || (a->m_pos_x != comp->m_pos_x)
                || (a->m_pos_y != comp->m_pos_y) || (a->m_value != comp->m_value)){
            return false;
        }
        comp_iter++;
    }
    return true;
}

void PuzzleBoard::calculate_unique_id(){
    Board_Iter iter;
    m_unique_id = "";   
    char ascii;
    for(iter = m_board.begin(); iter != m_board.end(); iter++){
        //Adds 33 to get corresponding ASCII character. This will decrease memory consumption
        //'A' is an ASCII character that has DEC value of 65. The configuration values will then vary
        //from A to Z (65 to 90 dec values)
        ascii = (*iter).m_value + 'A';
        m_unique_id.append(ascii);
    }
}

QString* PuzzleBoard::get_unique_id(){
    return &m_unique_id;
}
