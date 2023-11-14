#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define P_GRASS 0.4
#define P_ROAD 0.1
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
    else if (r < P_GRASS + P_ROAD)
    {
        return ROAD;
    }
    else if (r < P_GRASS + P_ROAD + P_CITY)
    {
        return CITY;
    }
    else if (r < P_GRASS + P_ROAD + P_CITY + P_FORREST)
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
    // Allocate memory for the surface map
    Surface * surface_map = (Surface *) malloc(sizeof(Surface) * GRID_SIZE * GRID_SIZE);

    // Initialize the random seed
    srand(time(NULL));

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            surface_map[i * GRID_SIZE + j] = random_surface();

    // Set a landmine at (10, 8)
    surface_map[8 * GRID_SIZE + 10] = LANDMINE;

    return surface_map;
}

int * generate_costs_map(const Surface * surface_map, Method method, char only_danger_zones)
{
    // Allocate memory for the costs map
    int * costs_map = (int *) malloc(sizeof(int) * GRID_SIZE * GRID_SIZE);

    // If method ia FASTEST or SAFEST set the surface costs
    // If method is SHORTEST set all the surface costs to 1 except for mine nodes
    if (only_danger_zones)
        for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
            if (surface_map[i] == LANDMINE)
                costs_map[i] = INF;
            else
                costs_map[i] = 0;
    else
        for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
            if (method == FASTEST || method == SAFEST)
                costs_map[i] = surface_map[i];
            else if (surface_map[i] == LANDMINE)
                costs_map[i] = INF;
            else
                costs_map[i] = 1;

    // If method is SAFEST increase the cost of nearby nodes
    if (method == SAFEST)
        add_danger_zones(costs_map);

    return costs_map;
}

void add_danger_zones(int * costs_map)
{
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
    {
        if (costs_map[i] == LANDMINE)
        {
            int x = i % GRID_SIZE;
            int y = i / GRID_SIZE;

            // Increase the cost of nearby nodes
            for (int j = -DANGER_ZONE_RADIUS; j <= DANGER_ZONE_RADIUS; j++)
            {
                for (int k = -DANGER_ZONE_RADIUS; k <= DANGER_ZONE_RADIUS; k++)
                {
                    // If the node is outside the grid, skip it
                    if (x + j < 0 || x + j >= GRID_SIZE || y + k < 0 || y + k >= GRID_SIZE)
                        continue;

                    int index = (y + k) * GRID_SIZE + (x + j);

                    // If the node is not a landmine, increase the costs by the danger zone radius - distance from the mine times 10
                    if (costs_map[index] != LANDMINE)
                        costs_map[index] += ((DANGER_ZONE_RADIUS-abs(j)+1) * (DANGER_ZONE_RADIUS-abs(k)+1) * 10);
                }
            }
        }
    }
}

Node * find_path(Node * start, Node * end, const int * costs_map)
{
    Node * open_nodes[GRID_SIZE * GRID_SIZE] = {start};
    Node * closed_nodes[GRID_SIZE * GRID_SIZE] = {NULL};
    
    int open_nodes_length = 1;
    int closed_nodes_length = 0;

    while (open_nodes_length > 0)
    {
        // TODO: Extract this to find_next_node function
        // Find the next node to be evaluated
        Node * current_node = open_nodes[0];
        int current_node_index = 0;

        for (int i = 0; i < open_nodes_length; i++)
            if (open_nodes[i]->f < current_node->f)
            {
                current_node = open_nodes[i];
                current_node_index = i;
            }

        // If the current node is the end node, return the path
        if (coordinate_is_match(current_node->coordinates, end->coordinates))
            return current_node;

        // Close the current node and remove it from the open nodes
        close_open_node(open_nodes, &open_nodes_length, closed_nodes, &closed_nodes_length, current_node, current_node_index);

        // Add the neighbours of the current node to the open nodes
        add_node_neighbours_to_open_nodes(open_nodes, &open_nodes_length, closed_nodes, closed_nodes_length, current_node, end, costs_map);
    }

    // Return NULL if no path was found to the end node
    return NULL;
}

int find_next_node(Node * open_nodes[], int open_nodes_length, Node * current_node)
{
    int current_index = 0;

    for (int i = 0; i < open_nodes_length; i++)
    {
        if (open_nodes[i]->f < current_node->f)
        {
            current_node = open_nodes[i];
            current_index = i;
        }
    }

    return current_index;
}

void close_open_node(Node * open_nodes[], int * open_nodes_length, Node * closed_nodes[], int * closed_nodes_length, Node * current_node, int current_node_index)
{
    // Remove node from array by shifting all nodes after the index one to the left and lowering length by 1
    for (int i = current_node_index; i < * open_nodes_length - 1; i++)
    {
        open_nodes[i] = open_nodes[i + 1];
    }

    (*open_nodes_length) -= 1;

    // Add the current node to the closed nodes
    closed_nodes[(* closed_nodes_length)++] = current_node;
}

void add_node_neighbours_to_open_nodes(
        Node * open_nodes[], int * open_nodes_length,
        Node * closed_nodes[], int closed_nodes_length,
        Node * current_node, Node * end,
        const int * costs_map)
{
    // For each horizontal neighbour of the current node
    for (int i = -1; i <= 1; i++)
    {
        // For each vertical neighbour of the current node
        for (int j = -1; j <= 1; j++)
        {
            // Skip the current node
            if (i == 0 && j == 0)
                continue;

            // Assign neighbour coordinates
            Coordinates coordinates = (Coordinates) {
                current_node->coordinates.x + i,
                current_node->coordinates.y + j};

            // If the neighbour is outside the grid, skip it
            if (coordinates.x < 0 || coordinates.x >= GRID_SIZE || coordinates.y < 0 || coordinates.y >= GRID_SIZE)
                continue;

            // Find the coordinate cost in the costs map
            int cost = costs_map[coordinates.y * GRID_SIZE + coordinates.x];

            // If the neighbour is a landmine, skip it
            if (cost == INF)
                continue;

            // Assign neighbour node
            Node * neighbour = create_node(
                    coordinates,
                    current_node,
                    current_node->g + cost,
                    calculate_heuristic_cost(coordinates, end->coordinates));

            // If the coordinate has been closed, skip it as it will never be the best path
            if (coordinates_are_closed(neighbour, closed_nodes, closed_nodes_length))
                continue;

            // If the coordinate is already open and the new f is higher, skip it
            if (coordinates_are_open(neighbour, open_nodes, *open_nodes_length))
                continue;

            // Add the neighbour to the open nodes
            open_nodes[(* open_nodes_length)++] = neighbour;
        }
    }
}

char coordinates_are_closed(Node * node, Node * closed_nodes[], int closed_nodes_length)
{
    for (int i = 0; i < closed_nodes_length; i++)
        if (coordinate_is_match(closed_nodes[i]->coordinates, node->coordinates))
            return 1;

    return 0;
}

char coordinates_are_open(Node * node, Node * open_nodes[], int open_nodes_length)
{
    for (int i = 0; i < open_nodes_length; i++)
        if (coordinate_is_match(open_nodes[i]->coordinates, node->coordinates))
        {
            if (open_nodes[i]->f > node->f)
                open_nodes[i] = node;

            return 1;
        }

    return 0;
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

int coordinate_is_match(Coordinates a, Coordinates b)
{
    return a.x == b.x && a.y == b.y;
}

// Traverse through the path and set the surface map nodes to ROAD
// Set the start and end nodes to their respective values
void add_path_to_surface_map(Node * start, Node * end, Node * path, Surface * surface_map)
{
    Node * current = path;

    while (current != NULL)
    {
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
    for (int i = 0; i < GRID_SIZE; i++)
    {
        printf("%.2d ", i);
    }

    printf("\n");

    // For each row print line
    for (int i = 0; i < GRID_SIZE; i++)
    {
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
    switch (surface)
    {
        case ROAD:
            print_square(TRANSPARENT);
        case GRASS:
            printf(ANSI_COLOR_GREEN);
            print_square(OPAQUE);
            printf(ANSI_COLOR_RESET);
            break;
        case CITY:
            printf(ANSI_COLOR_MAGENTA);
            print_square(OPAQUE);
            printf(ANSI_COLOR_RESET);
            break;
        case FORREST:
            printf(ANSI_COLOR_YELLOW);
            print_square(OPAQUE);
            printf(ANSI_COLOR_RESET);
            break;
        case WATER:
            printf(ANSI_COLOR_BLUE);
            print_square(OPAQUE);
            printf(ANSI_COLOR_RESET);
            break;
        case LANDMINE:
            printf(ANSI_COLOR_RED);
            print_square(SOLID);
            printf(ANSI_COLOR_RESET);
            break;
        case START:
        case END:
            printf(ANSI_COLOR_CYAN);
            print_square(SOLID);
            printf(ANSI_COLOR_RESET);
            break;
        case PATH:
            print_square(SOLID);
            break;
        default:
            break;
    }
}

void print_costs_map(const int * costs_map)
{
    printf("COSTS ->\n   ");
    // Print X coordinates
    for (int i = 0; i < GRID_SIZE; i++)
    {
        printf(" %.2d ", i);
    }

    printf("\n");

    // For each row print line
    for (int i = 0; i < GRID_SIZE; i++)
    {
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

void print_square(Fill mode)
{
    switch (mode)
    {
        case TRANSPARENT:
            printf("\u2591\u2591 ");
            break;
        case OPAQUE:
            printf("\u2592\u2592 ");
            break;
        case SOLID:
            printf("\u2593\u2593 ");
            break;
    }
}
