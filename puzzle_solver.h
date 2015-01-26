#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include "heuristics.h"
#include "node_container.h"
#include "node.h"

#define DEPTH_COST 1

enum SOLVER_STATUS{RUNNING, NO_SOLUTION_FOUND, SOLUTION_FOUND};
/*This class is responsible for applying the A-Star algorithm and solving the puzzle*/
class PuzzleSolver
{
public:
    PuzzleSolver(PuzzleBoard* initial_board, PuzzleBoard* goal);
    ~PuzzleSolver();
    PuzzleBoard *get_current_board();  
    int get_status();
    int solve();
    int get_num_nodes();
    std::vector<Node*>* get_solution_nodes();
private:
    Node* get_lowest_cost_node();
    void build_solution_steps(Node *node);
    int heuristics_cost(Node *);
    bool is_solution(State *a);
    bool compare_states(State *a, State *b);    
    void check_expanded_node(Node *expanded_node, Node *lowest_cost_node);
    void expand_node(Node*);
    Node *m_root;
    Node *m_curr_node;
    State *m_goal_state;
    bool m_solution_found;
    int m_status;
    NodesContainer *m_nodes;   
    int m_num_nodes;   
};

#endif // SEARCH_TREE_H
