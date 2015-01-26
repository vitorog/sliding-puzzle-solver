#ifndef PUZZLE_BOARD_H
#define PUZZLE_BOARD_H

#include <vector>
#include <QString>

enum DIRECTIONS { RIGHT = 0, LEFT, UP, DOWN, BLOCKED };

//This class represents a puzzle piece
//Each piece has a position and a value
//The value 0 represents the "gap"piece
class PuzzlePiece{
public:
    PuzzlePiece(int x = 0, int y = 0, int value = 0, int dir = BLOCKED);
    int m_pos_x;
    int m_pos_y;
    int m_value;
    int m_movable_dir;
    void operator=(PuzzlePiece*);
};

typedef std::vector<PuzzlePiece> Board;
typedef std::vector<PuzzlePiece>::const_iterator Board_Const_Iter;
typedef std::vector<PuzzlePiece>::iterator Board_Iter;

class PuzzleBoard
{
public:
    PuzzleBoard(int board_size = 3);
    ~PuzzleBoard();
    //Access the puzzle piece at position x and y in the board
    PuzzlePiece* piece(int x, int y);   
    void update_board();
    void move_piece(int x, int y);
    int check_piece_mov(int x, int y);
    std::vector<PuzzlePiece*> get_movable_pieces();
    int get_size(); 
    const Board* get_board();
    void set_board(PuzzleBoard*);
    bool compare_boards(PuzzleBoard*);
    void scramble_board_with_moves(int num_moves = 100);
    void calculate_unique_id();
    QString* get_unique_id();
private:
    //This variable will store the board size
    int m_board_size;
    //This vector will store the board pieces
    Board m_board;
    QString m_unique_id;
};

#endif // PUZZLE_BOARD_H
