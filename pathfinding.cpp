#include "pathfinding.h"
#include <queue>
#include <functional>
#include <iostream>
#include <algorithm>

// comparison structure so we can use a priority_queue of Node pointers
struct NodeCompare : public std::binary_function<Node*, Node*, bool>
{
    bool operator()(const Node* n1, const Node* n2)
    {
        return n1->f > n2->f;
    }
};

Grid::Grid(int startX, int startY, int goalX, int goalY)
{
    // generate nodes of grid
    Node* node;
    for (int x = 0; x < GRID_X; x++)
    {
        for (int y = 0; y < GRID_Y; y++)
        {
            node = new Node;
            nodes[x][y] = node;
            node->x = x;
            node->y = y;
        }
    }
    
    // set start/goal nodes
    start = nodes[startX][startY];
    goal = nodes[goalX][goalY];
}

Grid::~Grid()
{
    // delete nodes of grid
    for (int x = 0; x < GRID_X; x++)
    {
        for (int y = 0; y < GRID_Y; y++)
        {
            delete nodes[x][y];
        }
    }
}

std::vector<Node*> Grid::neighbors(Node* n)
{
    int x, y;
    std::vector<Node*> neighbors  = {};
    for (int dx : {-1,0,1})
    {
        for (int dy : {-1,0,1})
        {
            if (dx == 0 && dy == 0) continue; // don't add n itself to vector of neighbors

            x = n->x + dx;
            y = n->y + dy;
            if (x >= 0 && x <= GRID_X - 1 && // ignore nodes outside grid
                y >= 0 && y <= GRID_Y - 1 && 
                nodes[x][y]->type == NodeType::empty) // ignore walls
            {
                neighbors.push_back(nodes[x][y]);
            }
        }
    }
    return neighbors;
}

Node* aStar(Grid* grid)
{
    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> open; // nodes to be examined
    open.push(grid->start);
    grid->start->open = true;

    std::vector<Node*> closed = {}; // already examined nodes

    Node* currentNode;
    std::vector<Node*> neighbors;

    while (open.size() != 0)
    {
        currentNode = open.top();

        if (currentNode == grid->goal) // if the current node is the goal
        {
            return currentNode;
        }
        
        open.pop(); // remove currentNode from open queue
        currentNode->open = false;
        closed.push_back(currentNode);
        currentNode->closed = true;

        neighbors = grid->neighbors(currentNode);

        for (Node* neighborNode : neighbors)
        {
            if (!neighborNode->open && !neighborNode->closed)
            {
                open.push(neighborNode);
                neighborNode->open = true;
                neighborNode->parent = currentNode;
                neighborNode->g = currentNode->g + 1;
                neighborNode->f = neighborNode->g + h(neighborNode,grid);
            } 
        }
    }

    return grid->start;
}

void displayPath(Node* path, Grid* grid)
{
    std::vector<Node*> pathNodes = {}; // nodes in the path
    Node* n = path;
    while (n != nullptr)
    {
        pathNodes.push_back(n);
        n = n->parent;
    }

    std::string gridText = "";
    gridText += "+";
    for (int x = 0; x < GRID_X; x++)
    {
        gridText += "--";
    }
    gridText += "+\n";
    for (int y = 0; y < GRID_Y; y++)
    {
        gridText += "|";
        for (int x = 0; x < GRID_X; x++)
        {
            if (std::find(pathNodes.begin(), pathNodes.end(), grid->nodes[x][y]) != pathNodes.end())
            {
                gridText += ". ";
            }
            else if (grid->nodes[x][y]->type == NodeType::wall)
            {
                gridText += "@@";
            }
            else
            {
                gridText += "  ";
            }
        }
        gridText += "|\n";
    }
    gridText += "+";
    for (int x = 0; x < GRID_X; x++)
    {
        gridText += "--";
    }
    gridText += "+";
    std::cout << gridText << std::endl;
}