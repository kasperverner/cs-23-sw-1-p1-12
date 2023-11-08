#include "lib.h"
#include <stdio.h>
#include <stdlib.h>

/*
  The variables f, g, and h here are used as part of the A* search algorithm.

  g:    The cost it takes to move from the starting point to a given node on the grid.
  h:    The heuristic — estimated — cost to move from a given node to the end point.
        This is the distance between the current node and the end point.
  f:    This is the sum of g and h.
        This value is used to sort the nodes and choose which one should be explored next.
        Nodes with lower f scores are prioritized as they are assumed to be faster to reach the end point.
        The f score is the value which the algorithm uses to make decisions.
*/

int main(void)
{
    // TODO: Ask the user for the start and end coordinates and the method to use
    Method method = SAFEST;
    Coordinates start_coordinates = {0, 0};
    Coordinates end_coordinates = {36, 37};

    // Get the surface map
    Surface * surface_map = generate_surface_map();

    // Generate the costs map using the surface map and the chosen method
    int * costs_map = generate_costs_map(surface_map, method, 0);
    print_costs_map(costs_map);
    
    // Create the start and end nodes
    Node * start = create_node(start_coordinates, NULL, 0, INF);
    Node * end = create_node(end_coordinates, NULL, 0, 0);

    // Find the path
    Node * path = find_path(start, end, costs_map);

    // If a path was found, add it to the surface map and print it
    if (path != NULL)
    {
        printf("PATH FOUND FROM (%d, %d) -> (%d, %d)\n",
               start_coordinates.x, start_coordinates.y, end_coordinates.x, end_coordinates.y);

        add_path_to_surface_map(start, end, path, surface_map);
        print_surface_map(surface_map);
    }
    else
    {
        printf("NO PATH FOUND FROM (%d, %d) -> (%d, %d)\n",
               start_coordinates.x, start_coordinates.y, end_coordinates.x, end_coordinates.y);
    }


    // Free the memory
    free(surface_map);
    free(costs_map);
}
