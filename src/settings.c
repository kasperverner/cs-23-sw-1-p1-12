#include <printf.h>
#include "settings.h"

/**
 * Builds the settings object by calling the functions that request the user to enter the settings.
 * @param map_length is used for validation, as the start and end coordinates cannot be outside the map.
 * @return The settings.
 */
settings_t scan_settings(int map_length)
{
    settings_t settings;
    settings.start_coordinates = get_start_coordinates(map_length);
    settings.end_coordinates = get_end_coordinates(map_length, settings.start_coordinates);
    settings.method = get_method();

    return settings;
}

/**
 * Requests the user to enter the start coordinates.
 * Prompts the user until valid input is entered.
 * @param map_length is used for validation, as the start coordinates cannot be outside the map.
 * @return The start coordinates.
 */
coordinates_t get_start_coordinates(int map_length)
{
    coordinates_t coordinates;

    while (1)
    {
        printf("Enter the start coordinates: x, y\n> ");
        scanf(" %d, %d", &coordinates.x, &coordinates.y);

        if (coordinates.x < 0 || coordinates.x > map_length || coordinates.y < 0 || coordinates.y > map_length)
        {
            printf("Invalid input. Please enter coordinates between 0 and %d\n", map_length - 1);
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
 * @param start_coordinates is used for validation, as the end coordinates cannot be the same as the start coordinates.
 * @param map_length is used for validation, as the end coordinates cannot be outside the map.
 * @return The end coordinates.
 */
coordinates_t get_end_coordinates(int map_length, coordinates_t start_coordinates)
{
    coordinates_t coordinates;

    while (1)
    {
        printf("Enter the end coordinates: x, y\n> ");
        scanf(" %d, %d", &coordinates.x, &coordinates.y);

        if (coordinates.x < 0 || coordinates.x > map_length || coordinates.y < 0 || coordinates.y > map_length || (coordinates.x == start_coordinates.x && coordinates.y == start_coordinates.y))
        {
            printf("Invalid input. Please enter coordinates between 0 and %d\n", map_length - 1);
            printf("The end coordinates cannot be (%d, %d) as the start coordinates.\n", start_coordinates.x, start_coordinates.y);
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
        printf("Choose the which kind of path, you want to find:\n");
        printf("1) Shortest path\n");
        printf("2) Fastest path\n");
        printf("3) Safest path\n");
        printf("> ");
        scanf(" %d", &method);

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