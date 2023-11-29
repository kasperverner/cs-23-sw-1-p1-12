#ifndef SETTINGS_H
#define SETTINGS_H

#define INF 999999
#define GRID_SIZE 40
#define DANGER_ZONE_RADIUS 4

enum route_method_e
{
    SHORTEST = 1,
    FASTEST = 2,
    SAFEST = 3
};
typedef enum route_method_e route_method_e;

enum surface_e
{
    ROAD = 1,
    GRASS = 5,
    CITY = 10,
    FORREST = 15,
    WATER = 100,
    START = 101,
    END = 102,
    PATH = 103,
    OBSTRUCTION = 10000,
    LANDMINE = INF
};
typedef enum surface_e surface_e;

struct coordinates_t
{
    int x;
    int y;
};
typedef struct coordinates_t coordinates_t;

struct settings_t
{
    coordinates_t start_coordinates;
    coordinates_t end_coordinates;
    route_method_e method;
};
typedef struct settings_t settings_t;

/**
 * node used for the A* search algorithm.
 *
 * coordinates: The coordinates of the node on the grid.
 * g:           The cost it takes to move from the starting point to a given node on the grid.
 * h:           The heuristic — estimated — cost to move from a given node to the end point.
 *              This is the distance between the current node and the end point.
 * f:           The sum of g and h.
 *              This value is used to sort the nodes and choose which one should be explored next.
 *              Nodes with lower f scores are prioritized as they are assumed to be faster to reach the end point.
 *              The f score is the value which the algorithm uses to make decisions.
 * next:        The previous node in the path.
 *              This is used to trace the path back to the starting point once the end point has been reached.
 */
struct node_t
{
    coordinates_t coordinates;
    long double g;
    long double h;
    long double f;
    struct node_t *next;
};
typedef struct node_t node_t;

settings_t scan_settings(int);
coordinates_t get_start_coordinates(int);
coordinates_t get_end_coordinates(int, coordinates_t);
route_method_e get_method(void);

#endif
