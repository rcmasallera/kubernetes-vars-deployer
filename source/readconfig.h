#include <string.h>
#include <regex.h>
#include "kubeconnector.h"

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
    int *time_w;
    char *time_w_arg;
    char *file;
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

int get_config_occurrences(const char *filename, Dictionary *lang) {
    int occurrences_count = 0;
    char line[MAX_LINE];
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf(lang->FOPENERROR, filename);
        return FALSE;
    }

    regex_t regex;
    if (regcomp(&regex, "\\[(.*?)\\]", REG_EXTENDED) != 0) {
        fprintf(stderr, "Error al compilar la expresión regular\n");
        fclose(file);
        return FALSE;
    }

    while (fgets(line, sizeof(line), file)) {

        if (line[0] == '\n' || line[0] == '#') {
            continue;
        }

        regmatch_t matches[2];
        char *search_start = line;

        while (regexec(&regex, search_start, 2, matches, 0) == 0) {
            occurrences_count++; 
            size_t match_length = matches[1].rm_eo - matches[1].rm_so;
            //char match[match_length + 1]; 
            //strncpy(match, search_start + matches[1].rm_so, match_length);
            //match[match_length] = '\0';

            //printf("Coincidencia encontrada: %s\n", match);

            search_start += matches[0].rm_eo;
        }
    }

    regfree(&regex);
    fclose(file);
    return occurrences_count;
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

