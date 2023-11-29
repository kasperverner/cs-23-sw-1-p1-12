#include <stdlib.h>
#include "settings.h"
#include "surface_map.h"
#include "cost_map.h"
#include "path_algorithm.h"
#include "visualizers.h"

int main(void)
{
    settings_t settings = scan_settings(GRID_SIZE);

    surface_e surface_map[GRID_SIZE * GRID_SIZE];
    populate_surface_map(surface_map, GRID_SIZE, settings);

    int costs_map[GRID_SIZE * GRID_SIZE];
    generate_costs_map(costs_map, surface_map, GRID_SIZE, DANGER_ZONE_RADIUS, settings.method, false);
    print_costs_map(costs_map, GRID_SIZE);

    node_t * start_node = create_node(settings.start_coordinates, NULL, 0, INF);
    node_t * end_node = create_node(settings.end_coordinates, NULL, 0, 0);

    node_t * path = find_path(costs_map, GRID_SIZE, start_node, end_node);
    print_path_result(settings, surface_map, GRID_SIZE, path);

    free_path(path);

    exit(EXIT_SUCCESS);
}
