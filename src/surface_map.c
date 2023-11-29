#include <stdlib.h>
#include <time.h>
#include "surface_map.h"

#define P_GRASS 0.35
#define P_FORREST 0.3
#define P_ROAD 0.1
#define P_CITY 0.15

/**
 * TEMPORARY SOLUTION -> TO BE READ FROM MAP FILE
 * Generates a random surface type.
 * @return the generated surface type.
 */
surface_e generate_random_surface(void)
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

/**
 * Populates the surface map with the surface types.
 * @param map the pointer to the first element of the surface map.
 * @param map_length the length of the surface map.
 * @param settings the settings provided by the user.
 */
void populate_surface_map(surface_e *map, int map_length, settings_t settings)
{
    srand(time(NULL));

    for (int x = 0; x < GRID_SIZE; x++)
        for (int y = 0; y < GRID_SIZE; y++)
            map[y * GRID_SIZE + x] = generate_random_surface();

    // Set a landmine at (3, 18)
    map[18 * GRID_SIZE + 3] = LANDMINE;

    // Set a landmine at (10, 8)
    map[8 * GRID_SIZE + 10] = LANDMINE;

    // Set a landmine at (17, 15)
    map[15 * GRID_SIZE + 17] = LANDMINE;

    // Set a landmine at (18, 4)
    map[4 * GRID_SIZE + 18] = LANDMINE;
}

/**
 * Adds the path to the surface map.
 * @param map the pointer to the first element of the surface map.
 * @param map_length the length of the surface map.
 * @param settings the settings provided by the user.
 * @param path the path to add to the surface map.
 */
void add_path_to_surface_map(surface_e *map, int map_length, settings_t settings, node_t *path)
{
    node_t *current = path;

    while (current != NULL)
    {
        map[current-> coordinates.y * map_length + current->coordinates.x] = PATH;
        current = current->next;
    }

    map[settings.start_coordinates.y * map_length + settings.start_coordinates.x] = START;
    map[settings.end_coordinates.y * map_length + settings.end_coordinates.x] = END;
}