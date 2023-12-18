#ifndef SETTINGS_H
#define SETTINGS_H

#define INF 999999
#define DANGER_ZONE_RADIUS 3

typedef enum
{
    SHORTEST = 1,
    FASTEST = 2,
    SAFEST = 3
} route_method_e;

typedef enum
{
    UNKNOWN = -1,
    ROAD = 1,
    DIRT = 11,
    LIGHT_BRUSH = 12,
    FOREST = 25,
    CITY = 26,
    SAND = 41,
    WATER = INF-1,
    LANDMINE = INF,
    START = 1111,
    END = 2222,
    PATH = 3333
} surface_e;

typedef struct coordinates_t
{
    int x;
    int y;
} coordinates_t;

typedef struct settings_t
{
    coordinates_t start_coordinates;
    coordinates_t end_coordinates;
    route_method_e method;
} settings_t;

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
typedef struct node_t
{
    coordinates_t coordinates;
    long double g;
    long double h;
    long double f;
    struct node_t *next;
} node_t;

settings_t scan_settings(surface_e *, int);
coordinates_t get_start_coordinates(surface_e *, int);
coordinates_t get_end_coordinates(surface_e *, int, coordinates_t);
route_method_e get_method(void);

#endif
