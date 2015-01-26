#ifndef NODE_H
#define NODE_H

#include "puzzle_board.h"

/*This represents an action which consists of a puzzle piece and the direction that this piece moved*/
class Action{
public:
    Action();   
    int m_move;
    int m_piece_value;
    int m_piece_x;
    int m_piece_y;
};

/*This represents the current state of a node, which is the board configuration*/
class State{
public:
    State();
    ~State();
    PuzzleBoard *m_board;
};

/*
  This class represents a Node in the search tree
  A node contains a State (a board with the pieces position), an Action (the action that
  resulted in this node's state/the action applied to its parent), a parent (the previous node)
  amd a cost (calculated through depth and heuristics)
*/
class Node{
public:
    Node();   
    ~Node();
    Action *m_action;
    Node *m_parent;
    State *m_state;
    int m_depth;
    //Cost to get to this node
    int m_cost_to_node;
    //Cost from this node to the goal
    int m_cost_to_goal;
    int m_cost;
    bool m_solution_node;
    bool operator<(Node *b);
    QString *m_unique_id;
};

#endif // NODE_H
