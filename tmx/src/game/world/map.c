#include "map.h"
#include <string.h>

char current_map_path[128];

int get_map_id(const char *path)
{
    if (strstr(path, "Village") != NULL)
        return 1;
    else if (strstr(path, "Bar") != NULL)
        return 2;
    else if (strstr(path, "Classe") != NULL)
        return 3;
    else
        return 0;
}
