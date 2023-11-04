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

int main(void) {
    int * grid = init_grid(SAFEST);
    print_grid(grid);

    Node * start = create_node((Coordinates) {0, 0}, NULL, 0, INF);
    Node * end = create_node((Coordinates) {14, 12}, NULL, 0, 0);

    Node * path = find_path(start, end, grid);
    print_path(path);

    // print_colored();

    free(grid);
}
