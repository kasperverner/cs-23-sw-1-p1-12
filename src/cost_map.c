#include <stdlib.h>
#include "cost_map.h"

/**
 * Populates the costs map with the costs of each node.
 * If debug is true, only the danger zones will be added to the costs map.
 * If method is SHORTEST, only the mines will have costs.
 * If method is FASTEST, mines and surfaces will have costs.
 * If method is SAFEST, mines and surfaces will have costs and danger zones will be added to the map.
 * @param costs_map the pointer to the first element of the costs map.
 * @param surface_map the pointer to the first element of the surface map.
 * @param map_length the length of the maps.
 * @param danger_zone_radius the radius of the danger zones.
 * @param method the method to use for populating the costs map.
 * @param debug whether to only show danger zones on the costs map.
 */
void generate_costs_map(int * costs_map, const surface_e * surface_map, int map_length, int danger_zone_radius, route_method_e method, bool debug)
{
    if (debug)
        for (int i = 0; i < map_length * map_length; i++)
            if (surface_map[i] == LANDMINE)
                costs_map[i] = INF;
            else
                costs_map[i] = 0;
    else
        for (int i = 0; i < map_length * map_length; i++)
            if (method == FASTEST || method == SAFEST)
                costs_map[i] = surface_map[i];
            else if (surface_map[i] == LANDMINE)
                costs_map[i] = INF;
            else
                costs_map[i] = 1;

    // If method is SAFEST increase the cost of nearby nodes
    if (method == SAFEST)
        add_danger_zones(costs_map, map_length, danger_zone_radius);
}

/**
 * Adds danger zones to the costs map.
 * @param costs_map the pointer to the first element of the costs map.
 * @param map_length the length of the costs map.
 * @param danger_zone_radius the radius of the danger zones.
 */
void add_danger_zones(int * costs_map, int map_length, int danger_zone_radius)
{
    for (int i = 0; i < map_length * map_length; i++)
    {
        if (costs_map[i] == LANDMINE)
        {
            int x = i % map_length;
            int y = i / map_length;

            // Increase the cost of nearby nodes
            for (int j = -danger_zone_radius; j <= danger_zone_radius; j++)
            {
                for (int k = -danger_zone_radius; k <= danger_zone_radius; k++)
                {
                    // If the node is outside the grid, skip it
                    if (x + j < 0 || x + j >= map_length || y + k < 0 || y + k >= map_length)
                        continue;

                    int index = (y + k) * map_length + (x + j);

                    // If the node is not a landmine, increase the costs by the danger zone radius - distance from the mine times 10
                    if (costs_map[index] != LANDMINE)
                        costs_map[index] += calculate_danger_zone_cost(danger_zone_radius, j, k, 10);
                }
            }
        }
    }
}

/**
 * Calculates the cost of a danger zone.
 * @param danger_zone_radius
 * @param horizontal_distance_to_mine
 * @param vertical_distance_to_mine
 * @param amplifier
 * @return the cost to add to the node based on the distance to the mine.
 */
int calculate_danger_zone_cost(int danger_zone_radius, int horizontal_distance_to_mine, int vertical_distance_to_mine, int amplifier)
{
    int horizontal_cost = (danger_zone_radius - abs(horizontal_distance_to_mine) + 1);
    int vertical_cost = (danger_zone_radius - abs(vertical_distance_to_mine) + 1);

    return horizontal_cost * vertical_cost * amplifier;
}