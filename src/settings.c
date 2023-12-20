#include <stdio.h>
#include "settings.h"
#include "surface_map.h"

/**
 * Builds the settings object by calling the functions that request the user to enter the settings.
 * @param surface_map is used for validation, to ensure user doesn't choose LANDMINE or WATER nodes.
 * @param map_length is used for validation, as the start and end coordinates cannot be outside the map.
 * @return The settings.
 */
settings_t scan_settings(surface_e * surface_map, int map_length)
{
    printf("\n");
    settings_t settings;
    settings.start_coordinates = get_start_coordinates(surface_map, map_length);
    settings.end_coordinates = get_end_coordinates(surface_map, map_length, settings.start_coordinates);
    settings.method = get_method();

    return settings;
}

/**
 * Requests the user to enter the start coordinates.
 * Prompts the user until valid input is entered.
 * @param surface_map is used for validation, to ensure user doesn't choose LANDMINE or WATER nodes.
 * @param map_length is used for validation, as the start coordinates cannot be outside the map.
 * @return The start coordinates.
 */
coordinates_t get_start_coordinates(surface_e * surface_map, int map_length)
{
    coordinates_t coordinates;

    while (1)
    {
        printf("Enter the start coordinates: x, y\n> ");
        int scanned_values = scanf(" %d, %d", &coordinates.x, &coordinates.y);

        if (scanned_values != 2)
            continue;

        if (coordinates.x < 0 || coordinates.x > map_length || coordinates.y < 0 || coordinates.y > map_length)
        {
            printf("Invalid input. Please enter coordinates between 0 and %d\n", map_length - 1);
        }
        else if (get_value_from_surface_map(surface_map, map_length, coordinates.x, coordinates.y) == LANDMINE || get_value_from_surface_map(surface_map, map_length, coordinates.x, coordinates.y) == WATER)
        {
            printf("Invalid input. Please enter coordinates that are not LANDMINE or WATER.\n");
        }
        else
        {
            return coordinates;
        }
    }
}

/**
 * Requests the user to enter the end coordinates.
 * Prompts the user until valid input is entered.
 * @param surface_map is used for validation, to ensure user doesn't choose LANDMINE or WATER nodes.
 * @param start_coordinates is used for validation, as the end coordinates cannot be the same as the start coordinates.
 * @param map_length is used for validation, as the end coordinates cannot be outside the map.
 * @return The end coordinates.
 */
coordinates_t get_end_coordinates(surface_e * surface_map, int map_length, coordinates_t start_coordinates)
{
    coordinates_t coordinates;

    while (1)
    {
        printf("Enter the end coordinates: x, y\n> ");
        int scanned_values = scanf(" %d, %d", &coordinates.x, &coordinates.y);

        if (scanned_values != 2)
            continue;

        if (coordinates.x < 0 || coordinates.x > map_length || coordinates.y < 0 || coordinates.y > map_length)
        {
            printf("Invalid input. Please enter coordinates between 0 and %d\n", map_length - 1);
        }
        else if (coordinates.x == start_coordinates.x && coordinates.y == start_coordinates.y)
        {
            printf("Invalid input. The end coordinates cannot be (%d, %d) as the start coordinates.\n", start_coordinates.x, start_coordinates.y);
        }
        else if (get_value_from_surface_map(surface_map, map_length, coordinates.x, coordinates.y) == LANDMINE || get_value_from_surface_map(surface_map, map_length, coordinates.x, coordinates.y) == WATER)
        {
            printf("Invalid input. Please enter coordinates that are not LANDMINE or WATER.\n");
        }
        else
        {
            return coordinates;
        }
    }
}

/**
 * Requests the user to enter the method.
 * Prompts the user until valid input is entered.
 * @return The method used for calculating surface costs.
 */
route_method_e get_method(void)
{
    route_method_e method;

    while (1)
    {
        printf("Choose which kind of path, you want the program to find:\n");
        printf("1) Shortest path - The most direct path when not accounting for terrain traversal time\n");
        printf("2) Fastest path - The most direct path when accounting for terrain traversal time\n");
        printf("3) Safest path - The most direct path when accounting for terrain traversal time and landmine blast radius\n");
        printf("> ");
        int scanned_values = scanf(" %d", &method);

        if (scanned_values != 1)
            continue;

        if (method == SHORTEST || method == FASTEST || method == SAFEST)
        {
            return method;
        }
        else
        {
            printf("Invalid input. Please select a setting from the list.\n");
        }
    }
}