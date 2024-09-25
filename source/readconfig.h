#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "langdict.h"

#if !defined(TRUE)
#define TRUE 1U
#endif // TRUE

#if !defined(FALSE)
#define FALSE 0U
#endif // FALSE

#define MAX_LINE 1024

typedef struct {
    char key[256];
    char value[256];
} Config;

typedef struct{
    Config *array;
    int *lines;
} ConfigSet;

int get_config_lines(const char *filename, Dictionary *lang){
    int lines_count = 0;
    char line[MAX_LINE];
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(lang->FOPENERROR, filename);
        return FALSE;
    }
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }
        lines_count++;
    }
    fclose(file);
    return lines_count;
}

void read_conf_file(const char *filename, Dictionary *lang, int *lines_count, ConfigSet *set, Config *config_array) {
    char line[MAX_LINE];

    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(lang->FOPENERROR, filename);
        return;
    }

    if (!config_array) {
        perror(lang->ALLOCATIONERROR);
        fclose(file);
        return;
    }

    int index = 0;
    while (fgets(line, sizeof(line), file)) {

        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        line[strcspn(line, "\n")] = '\0';

        char *delimiter = strchr(line, '=');
        if (delimiter) {
            *delimiter = '\0';
            char *variable = line;
            char *value = delimiter + 1;
            strcpy(config_array[index].key, variable);
            strcpy(config_array[index].value, value);
            index++;
        }
    }
    fclose(file);
    set->array=config_array;
    set->lines=lines_count;
    return;
}

