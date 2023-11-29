#include <stdio.h>
#include "visualizers.h"
#include "surface_map.h"

/**
 * Prints the surface map to the console.
 * @param map the pointer to the first element of the surface map.
 * @param map_length the length of the surface map.
 */
void print_surface_map(const surface_e *map, int map_length)
{
    printf("MAP ->\n   ");
    // Print X coordinates
    for (int i = 0; i < map_length; i++)
    {
        printf("%.2d ", i);
    }

    printf("\n");

    // For each row print line
    for (int i = 0; i < map_length; i++)
    {
        // For each column in row print row Y coordinate
        printf("%.2d ", i);

        for(int j = 0; j < map_length; j++)
            print_surface_node(map[i * map_length + j]);

        printf("\n");
    }
}

/**
 * Prints a single surface node to the console.
 * @param node the surface node to be printed.
 */
void print_surface_node(surface_e node)
{
    switch (node)
    {
        case ROAD:
            print_square(TRANSPARENT);
            break;
        case GRASS:
            printf(ANSI_COLOR_GREEN);
            print_square(OPAQUE);
            printf(ANSI_COLOR_RESET);
            break;
        case CITY:
            printf(ANSI_COLOR_MAGENTA);
            print_square(OPAQUE);
            printf(ANSI_COLOR_RESET);
            break;
        case FORREST:
            printf(ANSI_COLOR_YELLOW);
            print_square(OPAQUE);
            printf(ANSI_COLOR_RESET);
            break;
        case WATER:
            printf(ANSI_COLOR_BLUE);
            print_square(OPAQUE);
            printf(ANSI_COLOR_RESET);
            break;
        case LANDMINE:
            printf(ANSI_COLOR_RED);
            print_square(SOLID);
            printf(ANSI_COLOR_RESET);
            break;
        case START:
        case END:
            printf(ANSI_COLOR_CYAN);
            print_square(SOLID);
            printf(ANSI_COLOR_RESET);
            break;
        case PATH:
            print_square(SOLID);
            break;
        default:
            break;
    }
}

/**
 * Prints a square to the console.
 * @param mode the fill mode of the square.
 */
void print_square(fill_mode_e mode)
{
    switch (mode)
    {
        case TRANSPARENT:
            printf("\u2591\u2591 ");
            break;
        case OPAQUE:
            printf("\u2592\u2592 ");
            break;
        case SOLID:
            printf("\u2593\u2593 ");
            break;
    }
}

/**
 * Prints the explanation of the surface map to the console.
 */
void print_explanation(void)
{
    printf("\n");
    print_surface_node(START);
    printf(" -> START");
    printf("\n");
    print_surface_node(END);
    printf(" -> END");
    printf("\n");
    print_surface_node(PATH);
    printf(" -> PATH");
    printf("\n");
    print_surface_node(ROAD);
    printf(" -> ROAD");
    printf("\n");
    print_surface_node(GRASS);
    printf(" -> GRASS");
    printf("\n");
    print_surface_node(CITY);
    printf(" -> CITY");
    printf("\n");
    print_surface_node(FORREST);
    printf(" -> FORREST");
    printf("\n");
    print_surface_node(WATER);
    printf(" -> WATER");
    printf("\n");
    print_surface_node(LANDMINE);
    printf(" -> LANDMINE");
    printf("\n");
}

/**
 * Prints the path result to the console.
 * @param settings the settings provided by the user.
 * @param map the pointer to the first element of the surface map.
 * @param map_length the length of the surface map.
 * @param path the path to print.
 */
void print_path_result(settings_t settings, surface_e *map, int map_length, node_t *path)
{
    if (path != NULL)
    {
        printf("PATH FOUND FROM (%d, %d) -> (%d, %d)\n",
               settings.start_coordinates.x, settings.start_coordinates.y,
               settings.end_coordinates.x, settings.end_coordinates.y);

        add_path_to_surface_map(map, map_length, settings, path);
        print_surface_map(map, map_length);
        print_explanation();
    }
    else
    {
        printf("NO PATH FOUND FROM (%d, %d) -> (%d, %d)\n",
               settings.start_coordinates.x, settings.start_coordinates.y,
               settings.end_coordinates.x, settings.end_coordinates.y);
    }
}

/**
 * Prints the costs map to the console.
 * @param map the pointer to the first element of the costs map.
 * @param map_length the length of the costs map.
 */
void print_costs_map(const int * map, int map_length)
{
    printf("\nCOSTS\n   ");
    // Print X coordinates
    for (int i = 0; i < map_length; i++)
    {
        printf(" %.2d ", i);
    }

    printf("\n");

    // For each row print line
    for (int i = 0; i < map_length; i++)
    {
        // For each column in row print row Y coordinate
        printf("%.2d ", i);

        for(int j = 0; j < map_length; j++)
        {
            if (map[i * map_length + j] == INF)
            {
                printf(ANSI_COLOR_RED);
                //printf(" \u2622  ");
                printf("%.3d ", map[i * map_length + j] % 1000);
                printf(ANSI_COLOR_RESET);
            }
            else
            {
                printf("%.3d ", map[i * map_length + j]);
            }
        }

        printf("\n");
    }

    printf("\n");
}