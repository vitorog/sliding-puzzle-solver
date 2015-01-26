#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "node.h"

enum HEURISTICS{ MANHATTAN_DISTANCE, OUT_OF_PLACE_PIECES };

class State;

//This class represents heuristics. It receives two nodes and can return
//the cost of a given state for a given heuristic. If a new heuristic
//needs to be added, you only need to implement a new function and add the heuristic
//to the HEURISTICS enum.
class Heuristic{
public:
    static int get_cost(State *a, State *b, int heuristic = MANHATTAN_DISTANCE);
private:
    static int get_manhattan_distance(State *a, State *b);
    static int get_num_out_of_place_pieces(State *a, State *b);
};


#endif // HEURISTICS_H
