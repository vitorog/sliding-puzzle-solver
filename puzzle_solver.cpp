#include "puzzle_solver.h"

#include <QDebug>

//Constructor, it receives a initial board configuration and a final board configuration
PuzzleSolver::PuzzleSolver(PuzzleBoard *initial_board, PuzzleBoard *goal_board)
{
    m_nodes = new NodesContainer();

    m_goal_state = new State;
    m_goal_state->m_board = new PuzzleBoard(goal_board->get_size());
    m_goal_state->m_board->set_board(goal_board);

    State *root_state = new State;
    root_state->m_board = new PuzzleBoard(initial_board->get_size());
    root_state->m_board->set_board(initial_board);

    m_root = new Node;
    m_root->m_depth = 0;
    m_root->m_state = root_state;
    m_root->m_cost_to_node = 0;
    m_root->m_cost_to_goal = heuristics_cost(m_root);
    m_root->m_cost = m_root->m_cost_to_node + m_root->m_cost_to_goal;
    m_root->m_unique_id = m_root->m_state->m_board->get_unique_id();
    m_root->m_action = NULL;
    m_root->m_parent = NULL;

    m_curr_node = m_root;
    m_solution_found = false;

    m_nodes->insert(m_root,OPEN);
    m_status = RUNNING;  
    m_num_nodes = 0;  
    return;
}

PuzzleSolver::~PuzzleSolver(){
    delete m_goal_state;
    delete m_nodes;
}

//This is the main function. Each time its called, it will check the open list, expand the lowest cost nodes
//and add the expanded nodes to the open or closed list until
int PuzzleSolver::solve(){
    Node *lowest_cost_node = NULL;
    Node *expanded_node = NULL;
    std::vector<Node*>::iterator expanded_iter;
    //If there are still open nodes to check
    if((m_nodes->get_size(OPEN)>0)){
        //Updates the total numbers of nodes
        m_num_nodes = m_nodes->get_size(OPEN) + m_nodes->get_size(CLOSED);
        //Gets the lowest cost element
        lowest_cost_node = get_lowest_cost_node();
        //If the board configuration of this node is a solution
        //then stop
        if(is_solution(lowest_cost_node->m_state)){
            build_solution_steps(lowest_cost_node);
            m_solution_found = true;
            m_status = SOLUTION_FOUND;
            return 1;
        }
        //Expands the node and put the expanded nodes in a vector
        expand_node(lowest_cost_node);
        //While there are expanded nodes
        while(m_nodes->get_size(EXPANDED)>0){
            expanded_node = m_nodes->get_expanded_node();
            //For every expanded node, a check will be performed in this method
            check_expanded_node(expanded_node, lowest_cost_node);
        }
    }else{
        if(!m_solution_found)
            m_status = NO_SOLUTION_FOUND;
            return -1;
    }
    return 0;
}


//Returns the node with the lowest cost
Node* PuzzleSolver::get_lowest_cost_node(){
    Node* lowest_cost_node = m_nodes->get_top_node();    
    m_nodes->remove(lowest_cost_node,OPEN);
    m_nodes->insert(lowest_cost_node,CLOSED);
    m_curr_node = lowest_cost_node;
    return lowest_cost_node;
}


//Expands a node
void PuzzleSolver::expand_node(Node *node){
    PuzzleBoard *board  = NULL;
    PuzzlePiece *piece  = NULL;
    Action *action  = NULL;
    State *state  = NULL;
    Node *new_node  = NULL;
    std::vector<PuzzlePiece*> mov_pieces;
    std::vector<PuzzlePiece*>::iterator iter;
    mov_pieces = node->m_state->m_board->get_movable_pieces();

    //If there are movable pieces, it means that the node can be expanded
    if(mov_pieces.size() > 0){
        //For all movable pieces do
        for(iter = mov_pieces.begin(); iter != mov_pieces.end(); iter++){
            piece = (*iter);
            //If the node does not have any action (it means its the root node) or
            //if this node contains an action and the movable piece is different from its own movable piece
            //(this is done to avoid a backward step)
            if(((node->m_depth > 0) && (node->m_action->m_piece_value != piece->m_value)) || (node->m_depth == 0)){
                action = new Action;
                action->m_piece_value = piece->m_value;
                action->m_piece_x = piece->m_pos_x;
                action->m_piece_y = piece->m_pos_y;
                action->m_move = piece->m_movable_dir;             

                board = new PuzzleBoard(node->m_state->m_board->get_size());
                board->set_board(node->m_state->m_board);

                state = new State;
                state->m_board = board;
                state->m_board->move_piece(action->m_piece_x,action->m_piece_y);

                new_node = new Node;
                new_node->m_depth = node->m_depth + 1;
                new_node->m_parent = node;
                new_node->m_action = action;
                new_node->m_state = state;
                new_node->m_cost_to_goal = heuristics_cost(new_node);
                new_node->m_cost_to_node = new_node->m_depth*DEPTH_COST;
                new_node->m_cost = new_node->m_cost_to_node + new_node->m_cost_to_goal;
                new_node->m_unique_id = state->m_board->get_unique_id();
                //Adds the node to a expanded nodes list
                m_nodes->insert(new_node,EXPANDED);
            }
        }
    }
    return;
}

//Checks the expanded node
void PuzzleSolver::check_expanded_node(Node *node, Node *lowest_cost_node){
    Node *open_node = NULL;
    if(!m_nodes->contains(node,CLOSED) ){        
        if(!m_nodes->contains(node,OPEN)){
            //If the node is not in the open list, put it in the open list and calculate the costs;
            //The cost is already calculated on the expand function
            m_nodes->insert(node,OPEN);
            return;
        }else{
            open_node = m_nodes->get_node(node,OPEN);
            //If the node is already on the open list, check if this is a better path to the node          
            if(node->m_cost_to_node < open_node->m_cost_to_node){
                //If it is, update the costs
                open_node->m_parent = lowest_cost_node;
                open_node->m_depth = lowest_cost_node->m_depth + 1;
                open_node->m_cost_to_goal = node->m_cost_to_goal;
                open_node->m_cost_to_node = node->m_cost_to_node;
                open_node->m_cost = node->m_cost;
            }         
            delete node;
            return;
        }
    }else{
        delete node;
        return;
    }    
}

//Get the cost calculated from the heuristics
int PuzzleSolver::heuristics_cost(Node *n){
    int manhattan_distance = Heuristic::get_cost(n->m_state,m_goal_state, MANHATTAN_DISTANCE);
    int unordered_pieces = Heuristic::get_cost(n->m_state,m_goal_state, OUT_OF_PLACE_PIECES);
    return (manhattan_distance + unordered_pieces);
}

//When the solution is found, this method will check each node, starting from the last node
//and finishing on the root node and will build a list with these nodes
void PuzzleSolver::build_solution_steps(Node *node){
    Node *curr_node = NULL;
    node->m_solution_node = true;
    m_nodes->insert(node,SOLUTION);
    curr_node = node->m_parent;
    while(curr_node != NULL){
         curr_node->m_solution_node = true;
         m_nodes->insert(curr_node,SOLUTION);
         curr_node = curr_node->m_parent;
    }
}


//Returns the current board configuration
PuzzleBoard* PuzzleSolver::get_current_board(){
    return m_curr_node->m_state->m_board;
}


//Checks if to states are the same (they are the same if their board has the same configuration)
bool PuzzleSolver::compare_states(State *a, State *b){
    return a->m_board->compare_boards(b->m_board);
}

//Checks if a state has the same board configuration as the goal state. If it has, its a solution
bool PuzzleSolver::is_solution(State *a){
    return compare_states(a, m_goal_state);
}


//Returns the current states
int PuzzleSolver::get_status(){
    return m_status;
}

//Returns the total number of nodes
int PuzzleSolver::get_num_nodes(){
    return m_num_nodes;
}

//Returns the list containing the nodes that are part of the solution
std::vector<Node*>* PuzzleSolver::get_solution_nodes(){
    return m_nodes->get_solution_nodes();
}
