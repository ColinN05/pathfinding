#pragma once
#include <cmath>
#include <vector>
#define GRID_X 20
#define GRID_Y 20

enum NodeType
{
    empty = 0,
    wall
};

struct Node
{
    NodeType type = empty;
    Node* parent = nullptr;
    int x = 0, y = 0; // position in grid
    float g = 0; // distance from start node
    float f = 0; //  g + heuristic
    bool open = false, closed = false;
};

struct Grid
{
    Grid(int startX, int startY, int goalX, int goalY);
    ~Grid(); // deletes nodes
    Node* nodes[GRID_X][GRID_Y];
    Node* start, *goal;
    std::vector<Node*> neighbors(Node* n); // returns neighbors of node n
};

Node* aStar(Grid* grid);
void displayPath(Node* path, Grid* grid);

// heuristic function (diagonal metric)
inline float h(Node* n, Grid* g)
{
    return std::max(abs(n->x - g->goal->x), abs(n->y - g->goal->y));
}

inline float f(Node* n, Grid* g) 
{ 
    return n->g + h(n, g);
}