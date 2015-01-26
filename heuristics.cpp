#include "heuristics.h"

int to_positive(int value){
    if(value < 0)
        return -value;
    else
        return value;
}

//Return the calculated cost of a specified heuristic
int Heuristic::get_cost(State *curr_state, State *goal, int heuristic){
    switch(heuristic){
    case MANHATTAN_DISTANCE:
        return get_manhattan_distance(curr_state,goal);
        break;
    case OUT_OF_PLACE_PIECES:
        return get_num_out_of_place_pieces(curr_state,goal);
        break;
    default:
        return 0;
        break;
    }
    return 0;
}

int Heuristic::get_num_out_of_place_pieces(State *curr_state, State *goal_state){
    Board_Const_Iter goal_iter;
    Board_Const_Iter curr_iter;   
    int num_pieces = 0;
    for(curr_iter = curr_state->m_board->get_board()->begin();
        curr_iter != curr_state->m_board->get_board()->end();
        curr_iter++){

        if((*curr_iter).m_value != 0 ){
            for(goal_iter = goal_state->m_board->get_board()->begin();
                goal_iter != goal_state->m_board->get_board()->end();
                goal_iter++){

                if((*curr_iter).m_value == (*goal_iter).m_value ){
                    if(((*curr_iter).m_pos_x != (*goal_iter).m_pos_x) ||
                            ((*curr_iter).m_pos_y != (*goal_iter).m_pos_y)){
                        num_pieces++;
                    }
                }
            }
        }
    }
    return num_pieces;
}

int Heuristic::get_manhattan_distance(State *curr_state, State *goal_state){
    Board_Const_Iter goal_iter;
    Board_Const_Iter curr_iter;  
    int cost = 0;
    int x_distance = 0;
    int y_distance = 0;
    for(curr_iter = curr_state->m_board->get_board()->begin();
        curr_iter != curr_state->m_board->get_board()->end();
        curr_iter++){

        if((*curr_iter).m_value != 0 ){
            for(goal_iter = goal_state->m_board->get_board()->begin();
                goal_iter != goal_state->m_board->get_board()->end();
                goal_iter++){

                if((*curr_iter).m_value == (*goal_iter).m_value ){
                    x_distance = to_positive((*curr_iter).m_pos_x - (*goal_iter).m_pos_x);
                    y_distance = to_positive((*curr_iter).m_pos_y - (*goal_iter).m_pos_y);
                    cost += (x_distance + y_distance);
                    break;
                }
            }
        }
    }
    return cost;
}
