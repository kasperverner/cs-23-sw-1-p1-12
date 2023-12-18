#ifndef COSTS_MAP_H
#define COSTS_MAP_H

#include "settings.h"
#include <stdbool.h>

void generate_costs_map(int *, const surface_e *, int, int, route_method_e, bool);
void add_danger_zones(int *, int, int);
int calculate_danger_zone_cost(int, int, int, int);

#endif
