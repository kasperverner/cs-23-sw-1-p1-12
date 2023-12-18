#include <stdlib.h>
#include "settings.h"
#include "surface_map.h"
#include "cost_map.h"
#include "path_algorithm.h"
#include "visualizers.h"

int main(void)
{
    int grid_size = get_grid_size_from_file();

    surface_e surface_map[grid_size * grid_size];
    get_grid_from_file(surface_map, grid_size);

    print_surface_map(surface_map, grid_size);
    print_explanation();

    settings_t settings = scan_settings(surface_map, grid_size);

    int costs_map[grid_size * grid_size];
    generate_costs_map(costs_map, surface_map, grid_size, DANGER_ZONE_RADIUS, settings.method, false);
    print_costs_map(costs_map, grid_size);

    node_t * start_node = create_node(settings.start_coordinates, NULL, 0, INF);
    node_t * end_node = create_node(settings.end_coordinates, NULL, 0, 0);

    node_t * path = find_path(costs_map, grid_size, start_node, end_node);
    print_path_result(settings, surface_map, grid_size, path);

    recursively_free_path(path);

    exit(EXIT_SUCCESS);
}
