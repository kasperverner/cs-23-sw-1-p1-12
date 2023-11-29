#ifndef SURFACE_MAP_H
#define SURFACE_MAP_H

#include "settings.h"

void populate_surface_map(surface_e *, int, settings_t);
surface_e generate_random_surface(void);
void add_path_to_surface_map(surface_e *, int, settings_t, node_t *);

#endif
