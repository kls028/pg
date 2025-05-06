#include "load_config.h"
#include <cstdio>
int *load_config() {
    FILE* file;
    file = fopen("config.txt", "r");
    int *config = new int[2];
    fscanf(file, "%d %d", &config[0], &config[1]);
    fclose(file);
    return config;
}
