#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include "surface_map.h"

/**
 * Gets the grid size from the map file.
 * @return the grid size.
 */
int get_grid_size_from_file(void)
{
    FILE *file = fopen(FILE_NAME,"r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    int c = fgetc(file);
    int lines = 0;

    while (c != EOF)
    {
        if (c == '\n')
            lines++;

        c = fgetc(file);
    }

    fclose(file);
    return lines;
}

/**
 * Gets the grid from the map file.
 * @param map the pointer to the first element of the surface map.
 * @param map_length the length of the surface map, is used to validate the number of scanned nodes.
 */
void get_grid_from_file(surface_e *map, int map_length)
{
    FILE *file = fopen(FILE_NAME,"r");

    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(EXIT_FAILURE);
    }

    int current_character = fgetc(file);
    int i = 0;

    while (current_character != EOF)
    {
        if (current_character != '\n')
        {
            map[i] = get_surface_type_from_identifier((char)current_character);
            i++;
        }

        current_character = fgetc(file);
    }

    if (i != map_length * map_length)
    {
        printf("Error reading file!\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

/**
 * Gets the surface type from the identifier.
 * @param identifier the identifier of the surface type.
 * @return the surface type.
 */
surface_e get_surface_type_from_identifier(char identifier)
{
    switch (identifier)
    {
        case 'R':
            return ROAD;
        case 'D':
            return DIRT;
        case 'B':
            return LIGHT_BRUSH;
        case 'F':
            return FOREST;
        case 'S':
            return SAND;
        case 'C':
            return CITY;
        case 'W':
            return WATER;
        case 'L':
            return LANDMINE;
        default:
            return UNKNOWN;
    }
}

/**
 * Gets the value from the surface map.
 * @param map the pointer to the first element of the surface map.
 * @param map_length the length of the surface map.
 * @param x the x coordinate of the value.
 * @param y the y coordinate of the value.
 * @return the value.
 */
surface_e get_value_from_surface_map(surface_e * map, int map_length, int x, int y)
{
    return map[y * map_length + x];
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