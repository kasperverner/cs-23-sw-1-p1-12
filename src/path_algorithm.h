#ifndef PATH_ALGORITHM_H
#define PATH_ALGORITHM_H

#include "settings.h"

#define PYTHAGOREAN_THEOREM 1.4142

struct list_node_t
{
    node_t *data;
    struct list_node_t *next;
};
typedef struct list_node_t list_node_t;

node_t *create_node(coordinates_t, node_t *, long double, long double);
node_t *find_path(const int *, int, node_t *, node_t *);
node_t *recursively_find_path(const int *, int, list_node_t *, list_node_t *, node_t *);
list_node_t *prepend_node_to_list(node_t *, list_node_t *);
node_t *get_first_node_from_list(list_node_t *);
list_node_t *remove_first_node_from_list(list_node_t *);
bool coordinate_is_match(coordinates_t, coordinates_t);
list_node_t *add_node_neighbours_to_open_nodes(const int *, int, list_node_t *, list_node_t *, node_t *, node_t *);
double calculate_heuristic_cost(coordinates_t, coordinates_t);
node_t *find_node_in_list(list_node_t *, coordinates_t);
void remove_coordinates_from_list(list_node_t *, list_node_t *, coordinates_t);
list_node_t *add_node_to_list_ordered_by_f(list_node_t *, list_node_t *, node_t *);
void free_list(list_node_t *);
void free_path(node_t *);

#endif
