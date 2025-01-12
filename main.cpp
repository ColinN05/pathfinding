#include "pathfinding.h"
#include <iostream>

int main()
{
    // generate the grid
    Grid grid(0,0,GRID_X-1,GRID_Y-1);

    // add some random boundaries 
    for (int i = 0 ; i < GRID_Y-1; i++)
    {
        grid.nodes[5][i]->type = NodeType::wall;
        grid.nodes[9][GRID_Y-1-i]->type = NodeType::wall;
    }
    for (int i = 11; i < GRID_X; i++)
    {
        grid.nodes[i][12]->type = NodeType::wall;
    }

    // compute path with A*
    Node* n = aStar(&grid);
    displayPath(n, &grid);
}