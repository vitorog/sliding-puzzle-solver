#include "node.h"

Action::Action()
{
    m_move = BLOCKED;
    m_piece_value = -1;
    m_piece_x = -1;
    m_piece_y = -1;
}

State::State()
{
    m_board = NULL;
}

State::~State(){
    delete m_board;
}

Node::Node()
{
    m_solution_node = false;
    m_action = NULL;
    m_parent = NULL;
    m_state = NULL;
    m_depth = -1;
}

Node::~Node(){
    delete m_action;
    delete m_state;
}

bool Node::operator <(Node *b){
    return this->m_cost < b->m_cost;
}
