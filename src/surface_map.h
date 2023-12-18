#ifndef SURFACE_MAP_H
#define SURFACE_MAP_H

#include "settings.h"

#define FILE_NAME "map.txt"

int get_grid_size_from_file(void);
void get_grid_from_file(surface_e *, int);
surface_e get_surface_type_from_identifier(char);
surface_e get_value_from_surface_map(surface_e *, int, int, int);
void add_path_to_surface_map(surface_e *, int, settings_t, node_t *);

#endif
