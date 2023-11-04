#include "lib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


// Fill the grid with dummy data
void populate_grid(int grid [GRID_SIZE][GRID_SIZE])
{
    time_t t;
    srand((unsigned) time(&t));

    for (int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = rand() % 100;
        }
    }
}


void print_grid(int grid [GRID_SIZE][GRID_SIZE])
{
    printf("COSTS ->\n   ");
    // Print X coordinates
    for (int i = 0; i < GRID_SIZE; i++) {
        printf("%.2d ", i);
    }

    printf("\n");

    // For each row print line
    for (int i = 0; i < GRID_SIZE; i++) {
        // For each column in row print row Y coordinate
        printf("%.2d ", i);

        // For each column in row print node cost
        for(int j = 0; j < GRID_SIZE; j++) {
            printf("%.2d ", grid[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

Node * find_path(Node * start, Node * end, int costs [GRID_SIZE][GRID_SIZE])
{
    Node * open_nodes[GRID_SIZE * GRID_SIZE] = {start};
    Node * closed_nodes[GRID_SIZE * GRID_SIZE] = {NULL};
    
    int open_nodes_length = 1;
    int closed_nodes_length = 0;

    while (open_nodes_length > 0) {
        Node * current_node = open_nodes[0];
        int current_index = 0;

        // Set current to the open node with the lowest f
        for (int i = 0; i < open_nodes_length; i++) {
            if (open_nodes[i]->f < current_node->f) {
                current_node = open_nodes[i];
                current_index = i;
            }
        }

        // If the current node is the end node, return the path
        if (current_node->coordinates.x == end->coordinates.x && current_node->coordinates.y == end->coordinates.y)
        {
            return current_node;
        }

        // Remove the current node from the open nodes
        delete_node(open_nodes, &open_nodes_length, current_index);

        // Add the current node to the closed nodes
        closed_nodes[closed_nodes_length++] = current_node;

        // Get the neighbours of the current node
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (i == 0 && j == 0)
                    continue;

                // Assign neighbour coordinates
                Coordinates coordinates = (Coordinates) {current_node->coordinates.x + i, current_node->coordinates.y + j};

                // If the neighbour is outside the grid, skip it
                if (coordinates.x < 0 || coordinates.x >= GRID_SIZE || coordinates.y < 0 || coordinates.y >= GRID_SIZE)
                    continue;

                // Calculate the g, h and f values for the neighbour
                double g = current_node->g + costs[coordinates.y][coordinates.x];
                double h = calculate_heuristic_cost(coordinates, end->coordinates);

                // Assign neighbour node
                Node * neighbour = create_node(coordinates, current_node, g, h);

                // If the coordinate has been closed, skip it
                int skip = 0;
                for (int k = 0; k < closed_nodes_length; k++) {
                    if (closed_nodes[k]->coordinates.x == coordinates.x && closed_nodes[k]->coordinates.y == coordinates.y) {
                        skip = 1;
                        break;
                    }
                }

                // If the neighbour is already in the open nodes with lower f, skip it
                for (int k = 0; k < open_nodes_length; k++) {
                    if (open_nodes[k]->coordinates.x == neighbour->coordinates.x && open_nodes[k]->coordinates.y == neighbour->coordinates.y && open_nodes[k]->f < neighbour->f) {
                        skip = 1;
                        break;
                    }
                }

                if (skip)
                    continue;

                // Add the neighbour to the open nodes
                open_nodes[open_nodes_length++] = neighbour;
            }
        }
    }

    return NULL;
}

Node * create_node(Coordinates coordinates, Node* parent, double g, double h)
{
    Node * node = (Node *) malloc(sizeof(Node));
    node->coordinates = coordinates;
    node->g = g;
    node->h = h;
    node->f = g + h;
    node->parent = parent;

    return node;
}


// The heuristic cost for a node is the distance from the current node to the end node
// We use the pythagorean theorem to find the direct distance from current to end
double calculate_heuristic_cost(Coordinates current, Coordinates end)
{
    return sqrt(pow(current.x - end.x, 2) + pow(current.y - end.y, 2));
}

// Remove node from array by shifting all nodes after the index one to the left and lowering length by 1
void delete_node(Node * nodes[], int * length, int index)
{
    for (int i = index; i < *length - 1; i++) {
        nodes[i] = nodes[i + 1];
    }

    *length -= 1;
}

void print_path(Node * path)
{
    Node * current = path;
    if (path == NULL) {
        printf("NO PATH FOUND\n");
    } else {
        printf("PATH FOUND ->\n");
    }

    while (current != NULL) {
        printf("(%d, %d) -> %.2lf\n", current->coordinates.x, current->coordinates.y, current->f);
        current = current->parent;
    }
}

void print_colored()
{
    // NODE -> \u2593
    // MINE -> \u2622

    printf("\n");
    printf("Color example ->\n");

    printf(ANSI_COLOR_RED);
    printf("R E D \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2622 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf(ANSI_COLOR_GREEN);
    printf("G R E E N \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf(ANSI_COLOR_YELLOW);
    printf("Y E L L O W \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2593 \u2622 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf(ANSI_COLOR_BLUE);
    printf("B L U E \u2622 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf(ANSI_COLOR_MAGENTA);
    printf("M A G E N T A \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2593\n");
    printf(ANSI_COLOR_CYAN);
    printf("C Y A N \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2622 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593\n");
    printf("\u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593 \u2622 \u2593 \u2593 \u2593\n");
    printf(ANSI_COLOR_RESET);
}
