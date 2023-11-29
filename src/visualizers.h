#ifndef VISUALIZERS_H
#define VISUALIZERS_H

#include "settings.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

enum fill_mode_e
{
    TRANSPARENT = 0,
    OPAQUE = 1,
    SOLID = 2,
};
typedef enum fill_mode_e fill_mode_e;

void print_surface_map(const surface_e *, int);
void print_surface_node(surface_e);
void print_square(fill_mode_e);
void print_explanation(void);
void print_path_result(settings_t, surface_e *, int, node_t *);
void print_costs_map(const int *, int);

#endif
