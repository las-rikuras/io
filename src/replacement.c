#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct time_unit_struct {
    int maintenance;
    int resale_price;
} Unit;

typedef struct replacement_struct {
    Unit **years;
    int initial_cost;
    int equipment_lifetime;
    int project_lifetime;
} Replacement;