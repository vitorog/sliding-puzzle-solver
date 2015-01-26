#include "node_container.h"

#define USE_UNIQUE_ID_OPTIMIZATION 1

NodeComparison::NodeComparison(bool reverse_comparison){
    m_reverse = reverse_comparison;
}

bool NodeComparison::operator ()(Node *a, Node *b){
    if(!m_reverse){
        return a->m_cost < b->m_cost;
    }else{
        return a->m_cost > b->m_cost;
    }
}

NodesContainer::NodesContainer(){

}

//This is the container destructor.
//It deletes all nodes from all containers, except those that are on the solution
NodesContainer::~NodesContainer(){  
    Node *curr_node = NULL;
    while( !m_open_nodes.empty() ){
//        curr_node = m_open_nodes.top();
//        if(!this->contains_open(curr_node)){
//            if(!curr_node->m_solution_node)
//                delete curr_node;
//        }
        m_open_nodes.pop();
    }

#ifndef USE_UNIQUE_ID_OPTIMIZATION
    std::multimap<int,Node*>::iterator iter;
    iter = m_open_nodes_map.begin();
    while(!m_open_nodes_map.empty()){
        curr_node = (*iter).second;
        m_open_nodes_map.erase(iter);
        if(!curr_node->m_solution_node)
            delete curr_node;
        iter++;
    }
    m_open_nodes_map.clear();
    iter = m_closed_nodes.begin();
    while(!m_closed_nodes.empty()){
        curr_node = (*iter).second;
        m_closed_nodes.erase(iter);
        if(!curr_node->m_solution_node)
            delete curr_node;
        iter++;
    }
    m_closed_nodes.clear();
#else
    QHash<QString,Node*>::iterator hash_iter;
    hash_iter = m_open_nodes_hash.begin();
    while(!m_open_nodes_hash.empty()){
        curr_node = (*hash_iter);
        m_open_nodes_hash.erase(hash_iter);
        delete curr_node;
        hash_iter = m_open_nodes_hash.begin();
    }
    m_open_nodes_hash.clear();

    hash_iter = m_closed_nodes_hash.begin();
    while(!m_closed_nodes_hash.empty()){
        curr_node = (*hash_iter);
        m_closed_nodes_hash.erase(hash_iter);
        delete curr_node;
        hash_iter = m_closed_nodes_hash.begin();
    }
    m_closed_nodes_hash.clear();

//    std::vector<Node*>::iterator vector_iter;
//    vector_iter = m_solution_nodes.begin();
//    while(!m_solution_nodes.empty()){
//        curr_node = (*vector_iter);
//        m_solution_nodes.erase(vector_iter);
//        delete curr_node;
//        vector_iter = m_solution_nodes.begin();
//    }
    m_solution_nodes.clear();
#endif
}

//Inserts a node in the open or closed list
void NodesContainer::insert(Node *node, int node_type){
    switch(node_type){
    case OPEN:
        insert_open_node(node);
        break;
    case CLOSED:
        insert_closed_node(node);
        break;
    case EXPANDED:
        insert_expanded_node(node);
        break;
    case SOLUTION:
        insert_solution_node(node);
        break;
    default:
        break;
    }
}


//Removes a node from a specific list
void NodesContainer::remove(Node *node, int node_type){
    switch(node_type){
    case OPEN:
        remove_open(node);
        break;
    case CLOSED:
        remove_closed(node);
        break;
    default:
        break;
    }
}

//Verifies if a list contains a given node
bool NodesContainer::contains(Node *node, int node_type){
    switch(node_type){
    case OPEN:
        return contains_open(node);
        break;
    case CLOSED:
        return contains_closed(node);
        break;
    default:
        return false;
        break;
    }
}

//Checks if the node is in the open list
bool NodesContainer::contains_open(Node *node){
#ifndef USE_UNIQUE_ID_OPTIMIZATION
    Node *curr_node = NULL;
    curr_node = m_open_nodes.top();
    if(curr_node != NULL){
        if(curr_node->m_cost == node->m_cost){
            if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board))
                return true;
        }
    }
    return false;
    multimap<int,Node*>::iterator iter;
    pair<multimap<int,Node*>::iterator, multimap<int, Node*>::iterator> range;
    //Checks if there is more than one open node with the same cost
    if(m_open_nodes_map.count(node->m_cost)>1){
        //If there is, then get a list of all nodes with this cost
        range = m_open_nodes_map.equal_range(node->m_cost);
        //For every element of this list, iterates through then until all elements have been checked
        //Or a node with the same cost AND board configuration is found
        for(iter = range.first; iter != range.second; iter++){
            curr_node = (*iter).second;
            if(curr_node->m_cost == node->m_cost){
                if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board)){
                    return true;
                }
            }
        }
    }
    if(m_open_nodes_map.count(node->m_cost)==1){
        //If there is only one element with this cost, then checks it and compare with the given node
        curr_node = (*m_open_nodes_map.find(node->m_cost)).second;
        if(curr_node->m_cost == node->m_cost){
            if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board)){
                return true;
            }
        }
    }
    return false;
#else
    return m_open_nodes_hash.contains(*node->m_unique_id);
#endif
}


//Similar to the open_nodes comparison
bool NodesContainer::contains_closed(Node *node){
#ifndef USE_UNIQUE_ID_OPTIMIZATION
    multimap<int,Node*>::iterator iter;
    Node *curr_node = NULL;
    pair<multimap<int,Node*>::iterator, multimap<int, Node*>::iterator> range;
    if(m_closed_nodes.count(node->m_cost)>1){
        range = m_closed_nodes.equal_range(node->m_cost);
        for(iter = range.first; iter != range.second; iter++){
            curr_node = (*iter).second;
            if(curr_node->m_cost == node->m_cost){
                if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board)){
                    return true;
                }
            }
        }
    }
    if(m_closed_nodes.count(node->m_cost)==1){
        curr_node = (*m_closed_nodes.find(node->m_cost)).second;
        if(curr_node->m_cost == node->m_cost){
            if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board)){
                return true;
            }
        }
    }
    return false;
#else
    return m_closed_nodes_hash.contains(*node->m_unique_id);
#endif
}

//Inserts a node in the open nodes list
//The data structure is a priority queue, after each insertion, the nodes will be arranged in a way
//that the lowest cost node stays at the beginning of the queue
//This method also inserts the same node in the open_nodes map, so it will be easy to check if
//the open list contains it
void NodesContainer::insert_open_node(Node *node){
    m_open_nodes.push(node);
#ifndef USE_UNIQUE_ID_OPTIMIZATION
    m_open_nodes_map.insert( pair<int,Node*>(node->m_cost, node) );
#else    
    m_open_nodes_hash.insert(*node->m_unique_id,node);

#endif
}

void NodesContainer::insert_expanded_node(Node *node){
    m_expanded_nodes.push_back(node);
}

void NodesContainer::insert_solution_node(Node *node){   
    m_solution_nodes.push_back(node);
}

//Remove an open node from the open nodes list;
void NodesContainer::remove_open(Node *node){
#ifndef USE_UNIQUE_ID_OPTIMIZATION
    //First it is popped out from the priority queue
    m_open_nodes.pop();
    //Then the map is checked and the node will be removed from it
    multimap<int,Node*>::iterator iter;
    Node *curr_node = NULL;
    pair<multimap<int,Node*>::iterator, multimap<int, Node*>::iterator> range;
    if(m_open_nodes_map.count(node->m_cost) > 1){
        range = m_open_nodes_map.equal_range(node->m_cost);
        iter = range.first;
        while( iter != range.second ){
            curr_node = (*iter).second;
            if(curr_node->m_cost == node->m_cost){
                if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board)){
                    m_open_nodes_map.erase(iter++);
                }else{
                    ++iter;
                }
            }
        }
    }else{
        m_open_nodes_map.erase(node->m_cost);
    }
#else
    m_open_nodes.pop();
    m_open_nodes_hash.remove(*node->m_unique_id);
#endif
}

void NodesContainer::remove_closed(Node *node){
#ifndef USE_UNIQUE_ID_OPTIMIZATION
    //Then the map is checked and the node will be removed from it
    multimap<int,Node*>::iterator iter;
    Node *curr_node = NULL;
    pair<multimap<int,Node*>::iterator, multimap<int, Node*>::iterator> range;
    if(m_closed_nodes_map.count(node->m_cost) > 1){
        range = m_closed_nodes_map.equal_range(node->m_cost);
        iter = range.first;
        while( iter != range.second ){
            curr_node = (*iter).second;
            if(curr_node->m_cost == node->m_cost){
                if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board)){
                    m_open_nodes_map.erase(iter++);
                }else{
                    ++iter;
                }
            }
        }
    }else{
        m_closed_nodes_map.erase(node->m_cost);
    }
#else
    m_closed_nodes_hash.remove(*node->m_unique_id);
#endif
}


//Inserts a node in the closed nodes list
void NodesContainer::insert_closed_node(Node *node){
#ifndef USE_UNIQUE_ID_OPTIMIZATION
    m_closed_nodes.insert(pair<int,Node*>(node->m_cost, node) );
#else    
    m_closed_nodes_hash.insert(*node->m_unique_id,node);    
#endif
}


//Gets the lowest cost node from the priority queue
Node* NodesContainer::get_top_node(){
    if(!m_open_nodes.empty()){
        return m_open_nodes.top();
    }else{
        return NULL;
    }
}

//Get the lists size
int NodesContainer::get_size(int node_type){
    switch(node_type){
    case OPEN:
        return m_open_nodes.size();
        break;
    case CLOSED:
#ifndef USE_UNIQUE_ID_OPTIMIZATION
        return m_closed_nodes.size();
#else
        return m_closed_nodes_hash.size();
#endif
        break;
    case EXPANDED:
        return m_expanded_nodes.size();
    default:
        break;
    }
    return 0;
}

//Get a specific node with cost and board configuration same as node's
Node* NodesContainer::get_node(Node *node, int node_type){
    switch(node_type){
    case OPEN:
        return get_open_node(node);        
        break;
    case CLOSED:
        break;
    default:
        break;
    }
    return 0;
}

Node* NodesContainer::get_expanded_node(){
    Node *node = NULL;
    std::vector<Node*>::iterator iter;
    iter = m_expanded_nodes.begin();
    node = (*iter);
    m_expanded_nodes.erase(iter);
    return node;
}

//Returns an open node with the same cost and board configuration
Node* NodesContainer::get_open_node(Node* node){
#ifndef USE_UNIQUE_ID_OPTIMIZATION
    multimap<int,Node*>::iterator iter;
    Node *curr_node = NULL;
    pair<multimap<int,Node*>::iterator, multimap<int, Node*>::iterator> range;
    if(m_open_nodes_map.count(node->m_cost)>1){
        range = m_open_nodes_map.equal_range(node->m_cost);
        for(iter = range.first; iter != range.second; iter++){
            curr_node = (*iter).second;
            if(curr_node->m_cost == node->m_cost){
                if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board)){
                    return curr_node;
                }
            }
        }
    }
    if(m_open_nodes_map.count(node->m_cost)==1){
        curr_node =(*m_open_nodes_map.find(node->m_cost)).second;
        if(curr_node->m_cost == node->m_cost){
            if(curr_node->m_state->m_board->compare_boards(node->m_state->m_board)){
                return curr_node;
            }
        }
    }
    return NULL;
#else
    if(m_open_nodes_hash.contains(*node->m_unique_id)){
        return m_open_nodes_hash[*node->m_unique_id];
    }
    return NULL;
#endif
}

std::vector<Node*>* NodesContainer::get_solution_nodes(){
    return &(m_solution_nodes);
}
