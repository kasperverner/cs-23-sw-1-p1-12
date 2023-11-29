#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "path_algorithm.h"

/**
 *
 * @param coordinates the coordinates of the node.
 * @param parent the parent node.
 * @param g the cost it takes to move from the starting point to the given node.
 * @param h the heuristic cost to move from the given node to the end point.
 * @return the created map node.
 */
node_t *create_node(coordinates_t coordinates, node_t * next, long double g, long double h)
{
    node_t * new_node = (node_t *) malloc(sizeof(node_t));

    if (new_node == NULL)
    {
        printf("Error: failed to allocated new_node\n");
        exit(EXIT_FAILURE);
    }

    new_node->coordinates = coordinates;
    new_node->g = g;
    new_node->h = h;
    new_node->f = g + h;
    new_node->next = next;

    return new_node;
}

/**
 * Find the path from the start node to the end node.
 * @param map the pointer to the first element of the costs map.
 * @param map_length the length of the costs map.
 * @param start_node the pointer to the the start node.
 * @param end_node the pointer to the the end node.
 * @return the pointer to the last node in the path if a path is found.
 */
node_t *find_path(const int * map, int map_length, node_t * start_node, node_t * end_node)
{
    list_node_t * open_nodes = prepend_node_to_list(start_node, NULL);
    list_node_t * closed_nodes = NULL;

    node_t * path = recursively_find_path(map, map_length, open_nodes, closed_nodes, end_node);

    // Free the lists first to avoid memory leaks
    free_list(open_nodes);
    free_list(closed_nodes);

    return path;
}

/**
 * Recursively find the path from the start node to the end node.
 * @param map the pointer to the first element of the costs map.
 * @param map_length the length of the costs map.
 * @param open_nodes the pointer to the first entry in the list of open nodes sorted by f score from lowest to highest.
 * @param closed_nodes the pointer to the first entry in the list of closed nodes.
 * @param end_node the pointer to the end node.
 * @return the pointer to the last node in the path if a path is found.
 */
node_t *recursively_find_path(const int * map, int map_length,
                         list_node_t *open_nodes, list_node_t *closed_nodes,
                         node_t * end_node)
{
    // If there are no open nodes, there is no path
    if (open_nodes == NULL)
        return NULL;

    // The open nodes are sorted by f score from lowest to highest
    // The first node in the list will always have the lowest f score
    node_t * current_node = get_first_node_from_list(open_nodes);

    // Return the path if the current node is the end_node node
    if (coordinate_is_match(current_node->coordinates, end_node->coordinates))
        return current_node;

    // Move the current node from the open nodes to the closed nodes
    open_nodes = remove_first_node_from_list(open_nodes);
    closed_nodes = prepend_node_to_list(current_node, closed_nodes);

    // Add the neighbours of the current node to the open nodes
    open_nodes = add_node_neighbours_to_open_nodes(map, map_length, open_nodes, closed_nodes, current_node, end_node);

    return recursively_find_path(map, map_length, open_nodes, closed_nodes, end_node);
}

/**
 * Allocate a new node and prepend it to the list.
 * @param data the data to be stored in the node.
 * @param next the next node in the list.
 * @return
 */
list_node_t *prepend_node_to_list(node_t *data, list_node_t *next)
{
    list_node_t *list_node = (list_node_t *) malloc(sizeof(list_node_t));

    if (list_node == NULL)
    {
        printf("Error: failed to allocated list_node\n");
        exit(EXIT_FAILURE);
    }

    list_node->data = data;
    list_node->next = next;
    next = list_node;
    return next;
}

/**
 * Get the data from the first node in the list.
 * @param list the list to get the first node of.
 * @return the node_t data from the first node of the list.
 */
node_t *get_first_node_from_list(list_node_t * list)
{
    if (list == NULL)
    {
        return NULL;
    }

    return list->data;
}

/**
 * Removes the first node from the list.
 * @param list the list to remove the first node from.
 * @return the list_node_t without the first node.
 */
list_node_t *remove_first_node_from_list(list_node_t * list)
{
    if (list == NULL)
    {
        return NULL;
    }

    return list->next;
}

/**
 * Check if two coordinates are the same.
 * @param a the first coordinate.
 * @param b the second coordinate.
 * @return true if the coordinates are the same, false otherwise.
 */
bool coordinate_is_match(coordinates_t a, coordinates_t b)
{
    return a.x == b.x && a.y == b.y;
}

/**
 * Add the neighbours of the current node to the open nodes if the neighbour is open with a lower f score or neighbour is already closed.
 * @param map the pointer to the first element of the costs map.
 * @param map_length the length of the costs map.
 * @param open_nodes the pointer to the first entry in the list of open nodes sorted by f score from lowest to highest.
 * @param closed_nodes the pointer to the first entry in the list of closed nodes.
 * @param current_node the pointer to the current node.
 * @param end_node the pointer to the end node.
 */
list_node_t *add_node_neighbours_to_open_nodes(const int * map, int map_length,
                                       list_node_t *open_nodes, list_node_t *closed_nodes,
                                       node_t *current_node, node_t * end_node)
{
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            // Skip the current node
            if (y == 0 && x == 0)
                continue;

            // Assign neighbour coordinates
            coordinates_t coordinates = {
                    current_node->coordinates.x + x,
                    current_node->coordinates.y + y
            };

            // If the neighbour is outside the grid, skip it
            if (coordinates.x < 0 || coordinates.x >= map_length || coordinates.y < 0 || coordinates.y >= map_length)
                continue;

            // Find the coordinate cost in the costs map
            double cost = map[coordinates.y * map_length + coordinates.x];

            // If the neighbour is a landmine, skip it
            if (cost == INF)
                continue;

            // If the neighbour is a diagonal, account for the diagonal distance being longer than the horizontal or vertical distance
            // The diagonal distance is calculated using the pythagorean theorem.
            //if (x != 0 && y != 0)
                //cost *= PYTHAGOREAN_THEOREM;

            double heuristic_cost = calculate_heuristic_cost(coordinates, end_node->coordinates);

            // Allocated the new neighbour node
            node_t * new_node = create_node(
                    coordinates,
                    current_node,
                    current_node->g + cost,
                    heuristic_cost);

            node_t * closed_node_with_same_coordinates = find_node_in_list(closed_nodes, coordinates);

            // If the coordinate has been closed, skip it as it will never be the best path
            if (closed_node_with_same_coordinates != NULL)
                continue;

            node_t * open_node_with_same_coordinates = find_node_in_list(open_nodes, coordinates);

            // If the coordinate is already open with lower or same f score, do not add it to the open nodes
            if (open_node_with_same_coordinates != NULL && open_node_with_same_coordinates->f <= new_node->f)
                continue;

            // If the coordinate is already open with higher f score, remove coordinates from the open nodes
            if (open_node_with_same_coordinates != NULL && open_node_with_same_coordinates->f > new_node->f)
                remove_coordinates_from_list(open_nodes, NULL, coordinates);

            // Add the neighbour node to the open nodes
            open_nodes = add_node_to_list_ordered_by_f(open_nodes, open_nodes, NULL, new_node);
        }
    }

    return open_nodes;
}

/**
 * Calculate the heuristic cost to move from the current node to the end node.
 * The heuristic cost is the distance between the current node and the end node.
 * Since the map is a grid, the heuristic cost can be calculated using the the pythagorean theorem.
 * @param current_coordinates the coordinates of the current node.
 * @param end_coordinates the coordinates of the end node.
 * @return the heuristic cost to move from the current node to the end node.
 */
double calculate_heuristic_cost(coordinates_t current_coordinates, coordinates_t end_coordinates)
{
    return sqrt(pow(current_coordinates.x - end_coordinates.x, 2) + pow(current_coordinates.y - end_coordinates.y, 2));
}

/**
 * Find a node in a list of nodes.
 * @param list the list of nodes.
 * @param coordinates the coordinates of the node to find.
 * @return the node if it is found, NULL otherwise.
 */
node_t *find_node_in_list(list_node_t *list, coordinates_t coordinates)
{
    while (list != NULL)
    {
        if (coordinate_is_match(list->data->coordinates, coordinates))
            return list->data;

        list = list->next;
    }

    return NULL;
}

/**
 * Remove a node from a list of nodes by setting the previous node's next pointer to the current node's next pointer if the coordinates match.
 * @param current_node the current node.
 * @param previous_node the previous node.
 * @param coordinates the coordinates of the node to remove.
 */
void remove_coordinates_from_list(list_node_t *current_node, list_node_t *previous_node, coordinates_t coordinates)
{
    if (current_node == NULL)
        return;

    if (coordinate_is_match(current_node->data->coordinates, coordinates))
    {
        list_node_t *next_node = current_node->next;
        free(current_node);

        // If the current node is the first node, set previous node to the next node
        if (previous_node == NULL)
            previous_node = next_node;
        else
            previous_node->next = next_node;

        return;
    }

    remove_coordinates_from_list(current_node->next, current_node, coordinates);
}

/**
 * Recursively add node_to_add to a list of nodes sorted by f score from lowest to highest.
 * @param first_node the first node in the list.
 * @param current_node the current node_to_add.
 * @param previous_node the previous node_to_add.
 * @param node_to_add the node_to_add to add.
 * @return the list of nodes with the new node_to_add added.
 */
list_node_t *add_node_to_list_ordered_by_f(list_node_t *first_node, list_node_t *current_node, list_node_t *previous_node, node_t * node_to_add)
{
    // If the list is empty, add the node_to_add to the list
    if (first_node == NULL)
        return prepend_node_to_list(node_to_add, NULL);

    // If the current node is NULL, add the node_to_add to the end of the list
    if (current_node == NULL)
    {
        list_node_t *new_node = prepend_node_to_list(node_to_add, NULL);
        previous_node->next = new_node;
        return first_node;
    }

    // If the node_to_add has a lower f score than the current node, add the node_to_add before the current node
    if (node_to_add->f < current_node->data->f)
    {
        list_node_t *new_node = prepend_node_to_list(node_to_add, current_node);

        // If the current node is the first node, set the first node to the new node
        if (previous_node == NULL)
            first_node = new_node;
        else
            previous_node->next = new_node;

        return first_node;
    }

    // If the node_to_add has a higher f score than the current node, recursively add the node_to_add to the next node
    return add_node_to_list_ordered_by_f(first_node, current_node->next, current_node, node_to_add);
}

/**
 * Recursively free a list of nodes.
 * @param list the list of nodes to free.
 */
void free_list(list_node_t *list)
{
    if (list == NULL)
        return;

    list_node_t *next = list->next;

    free(list);

    free_list(next);
}

/**
 * Recursively free a path of nodes.
 * @param path the path of nodes to free.
 */
void free_path(node_t * path)
{
    if (path == NULL)
        return;

    node_t *next = path->next;

    free(path);

    free_path(next);
}