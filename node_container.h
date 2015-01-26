#ifndef NODE_CONTAINER_H
#define NODE_CONTAINER_H

#include <queue>
#include <map>
#include "node.h"
#include <QHash>

enum NODE_TYPE{ OPEN, CLOSED, EXPANDED, SOLUTION };

using std::multimap;
using std::queue;
using std::pair;

class NodeComparison{
public:
    NodeComparison(bool reverse_comparison = true);
    bool operator()(Node *a, Node *b);
private:
    bool m_reverse;
};


/* This class stores the nodes and is responsible for controlling all nodes in the application,
including open nodes, closed nodes, expanded nodes and nodes that are part of the solution*/
class NodesContainer{
public:
    NodesContainer();
    ~NodesContainer();
    void insert(Node *, int node_type = OPEN);
    void remove(Node *, int node_type = OPEN);
    bool contains(Node *, int node_type = OPEN);
    int get_size(int node_type = OPEN);
    Node* get_top_node();
    Node* get_node(Node *node, int node_type = OPEN);
    Node* get_expanded_node();
    std::vector<Node*>* get_solution_nodes();
private:
    void insert_open_node(Node *);
    void insert_closed_node(Node *);
    void insert_expanded_node(Node *);
    void insert_solution_node(Node *);

    bool contains_open(Node *);
    bool contains_closed(Node *);
    void remove_open(Node *);
    void remove_closed(Node *);

    Node* get_open_node(Node *);
    //This holds the expanded nodes  of a given state
    std::vector<Node*> m_expanded_nodes;
    //This holds the nodes that are part of the solution
    std::vector<Node*> m_solution_nodes;

    //void remove_closed(Node *);
    //The open nodes list is a priority queue and the cheaper node will always be at front
    std::priority_queue< Node* , std::vector<Node*>, NodeComparison > m_open_nodes;
    //This is the closed nodes list, it uses a multimap structure to accelarate lookups
    //and store multiple nodes with the same key (the cost)
    std::multimap <int, Node*> m_closed_nodes;
    QHash<QString, Node*> m_closed_nodes_hash;
    //This is a auxiliary structure which also holds open nodes and is used together with
    //the priority queue. When a node needs to be looked up in the priority queue, it is
    //looked up here, which is faster than checking on the priority queue, though uses more memory
    std::multimap <int, Node*> m_open_nodes_map;
    QHash<QString, Node*> m_open_nodes_hash;
};

#endif // NODE_CONTAINER_H
