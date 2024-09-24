#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct {
    char key[256];
    char value[256];
} Config;

Config read_conf_file(const char *filename) {
    char line[MAX_LINE];
    int lines_count = 0;

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }
        lines_count++;
    }

    Config *config_array = malloc(lines_count * sizeof(Config));
    if (!config_array) {
        perror("Error al asignar memoria");
        fclose(file);
        return;
    }

    rewind(file);

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

    for (int i = 0; i < lines_count; i++) {
        printf("Variable: '%s', Valor: '%s'\n", config_array[i].key, config_array[i].value);
    }

    //free(config_array);
    return *config_array;
}

