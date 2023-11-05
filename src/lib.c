#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P_GRASS 0.4
#define P_CITY 0.1
#define P_FORREST 0.3


// Temporary random surface generator
enum Surface random_surface()
{
    // Generate a random number between 0 and 1
    double r = (double)rand() / RAND_MAX;

    // Return a surface type based on the probability
    if (r < P_GRASS)
    {
        return GRASS;
    }
    else if (r < P_GRASS + P_CITY)
    {
        return CITY;
    }
    else if (r < P_GRASS + P_CITY + P_FORREST)
    {
        return FORREST;
    }
    else
    {
        return WATER;
    }
}

Surface * generate_surface_map()
{
    Surface * surface_map = (Surface *) malloc(sizeof(Surface) * GRID_SIZE * GRID_SIZE);

    // Initialize the random seed
    srand(time(NULL));

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            surface_map[i * GRID_SIZE + j] = random_surface();

    // Set a landmine at (10, 8)
    surface_map[170] = LANDMINE;

    return surface_map;
}

int * generate_costs_map(const Surface * surface_map, Method method)
{
    int * costs_map = (int *) malloc(sizeof(int) * GRID_SIZE * GRID_SIZE);

    // If method ia FASTEST or SAFEST set the surface costs
    // If method is SHORTEST set all the surface costs to 1 except for mine nodes
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
        costs_map[i] = surface_map[i];

    // If method is SAFEST increase the cost of nearby nodes
    if (method == SAFEST) {
        for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
            if (costs_map[i] == LANDMINE) {
                int x = i % GRID_SIZE;
                int y = i / GRID_SIZE;

                for (int j = -3; j <= 3; j++) {
                    for (int k = -3; k <= 3; k++) {
                        if (x + j < 0 || x + j >= GRID_SIZE || y + k < 0 || y + k >= GRID_SIZE)
                            continue;

                        int index = (y + k) * GRID_SIZE + (x + j);

                        if (costs_map[index] != LANDMINE)
                            costs_map[index] += ((4-abs(j)) * (4-abs(k)) * 10);
                    }
                }
            }
        }
    }

    return costs_map;
}

Node * find_path(Node * start, Node * end, const int * costs_map)
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

                int cost = costs_map[coordinates.y * GRID_SIZE + coordinates.x];

                // If the neighbour is a landmine, skip it
                if (cost == INF)
                    continue;

                // Calculate the g, h and f values for the neighbour
                long double g = current_node->g + cost;
                long double h = calculate_heuristic_cost(coordinates, end->coordinates);

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

Node * create_node(Coordinates coordinates, Node* parent, long double g, long double h)
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

void add_path_to_surface_map(Node * start, Node * end, Node * path, Surface * surface_map)
{
    Node * current = path;

    while (current != NULL) {
        surface_map[current->coordinates.y * GRID_SIZE + current->coordinates.x] = ROAD;
        current = current->parent;
    }

    surface_map[start->coordinates.y * GRID_SIZE + start->coordinates.x] = START;
    surface_map[end->coordinates.y * GRID_SIZE + end->coordinates.x] = END;
}

void print_surface_map(const Surface * surface_map)
{
    printf("MAP ->\n   ");
    // Print X coordinates
    for (int i = 0; i < GRID_SIZE; i++) {
        printf("%.2d ", i);
    }

    printf("\n");

    // For each row print line
    for (int i = 0; i < GRID_SIZE; i++) {
        // For each column in row print row Y coordinate
        printf("%.2d ", i);

        for(int j = 0; j < GRID_SIZE; j++)
            print_surface_node(surface_map[i * GRID_SIZE + j]);

        printf("\n");
    }

    printf("\n");
    print_surface_node(START);
    printf(" -> START");
    printf("\n");
    print_surface_node(END);
    printf(" -> END");
    printf("\n");
    print_surface_node(ROAD);
    printf(" -> PATH");
    printf("\n");
    print_surface_node(GRASS);
    printf(" -> GRASS");
    printf("\n");
    print_surface_node(CITY);
    printf(" -> CITY");
    printf("\n");
    print_surface_node(FORREST);
    printf(" -> FORREST");
    printf("\n");
    print_surface_node(WATER);
    printf(" -> WATER");
    printf("\n");
    print_surface_node(LANDMINE);
    printf(" -> LANDMINE");
    printf("\n");
}

void print_surface_node(Surface surface)
{
    switch (surface) {
        case GRASS:
            printf(ANSI_COLOR_GREEN);
            printf("\u2591\u2591 ");
            printf(ANSI_COLOR_RESET);
            break;
        case CITY:
            printf(ANSI_COLOR_MAGENTA);
            printf("\u2591\u2591 ");
            printf(ANSI_COLOR_RESET);
            break;
        case FORREST:
            printf(ANSI_COLOR_YELLOW);
            printf("\u2591\u2591 ");
            printf(ANSI_COLOR_RESET);
            break;
        case WATER:
            printf(ANSI_COLOR_BLUE);
            printf("\u2591\u2591 ");
            printf(ANSI_COLOR_RESET);
            break;
        case LANDMINE:
            printf(ANSI_COLOR_RED);
            printf("\u2593\u2593 ");
            printf(ANSI_COLOR_RESET);
            break;
        case START:
        case END:
            printf(ANSI_COLOR_CYAN);
            printf("\u2593\u2593 ");
            printf(ANSI_COLOR_RESET);
            break;
        case ROAD:
            printf("\u2591\u2591 ");
            break;
        default:
            break;
    }
}

void print_costs_map(const int * costs_map)
{
    printf("COSTS ->\n   ");
    // Print X coordinates
    for (int i = 0; i < GRID_SIZE; i++) {
        printf(" %.2d ", i);
    }

    printf("\n");

    // For each row print line
    for (int i = 0; i < GRID_SIZE; i++) {
        // For each column in row print row Y coordinate
        printf("%.2d ", i);

        for(int j = 0; j < GRID_SIZE; j++)
        {
            if (costs_map[i * GRID_SIZE + j] == INF)
            {
                printf(ANSI_COLOR_RED);
                printf(" \u2622  ");
                printf(ANSI_COLOR_RESET);
            }
            else
            {
                printf("%.3d ", costs_map[i * GRID_SIZE + j]);
            }
        }

        printf("\n");
    }

    printf("\n");
}
