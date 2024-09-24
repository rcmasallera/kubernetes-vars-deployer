#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "deployer.h"

void print_usage() {
    printf("Uso: deployer [opciones]\n");
    printf("Opciones:\n");
    printf("  -v, --verbose       Modo detallado\n");
    printf("  -l, --language=LANG Seleccionar idioma (esp o eng)\n");
    printf("  -p, --path=PATH     Directorio de trabajo\n");
    printf("  -c, --config=FILE   Archivo de configuración\n");
}

int main(int argc, char *argv[]) {
    Dictionary *dict = &spa;
    char *path = ".";
    char *config_file = NULL;
    int verbose = 0;
    Config *config_array;
    // Args processing
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1; 
        } 
        else if (strncmp(argv[i], "-l", 2) == 0 || strncmp(argv[i], "--language", 10) == 0) {
            char *language = strchr(argv[i], '=');
            if (!language && i + 1 < argc) {
                language = argv[++i];
            } else if (language) {
                language++;
            } else {
                print_usage();
                exit(EXIT_FAILURE);
            }
            if (strcmp(language, "esp") == 0) {
                dict = &spa;
            } else if (strcmp(language, "eng") == 0) {
                dict = &eng;
            } else {
                fprintf(stderr, dict->UNSUPPORTEDLANG, language);
                exit(EXIT_FAILURE);
            }
        } 
        else if (strncmp(argv[i], "-p", 2) == 0 || strncmp(argv[i], "--path", 6) == 0) {
            char *param_path = strchr(argv[i], '=');
            if (!param_path && i + 1 < argc) {
                path = argv[++i];
            } else if (param_path) {
                path = param_path + 1;
            } else {
                print_usage();
                exit(EXIT_FAILURE);
            }
        } 
        else if (strncmp(argv[i], "-c", 2) == 0 || strncmp(argv[i], "--config", 8) == 0) {
            char *config = strchr(argv[i], '=');
            if (!config && i + 1 < argc) {
                config_file = argv[++i];
            } else if (config) {
                config_file = config + 1;
            } else {
                print_usage();
                exit(EXIT_FAILURE);
            }
        } 
        else {
            print_usage();
            exit(EXIT_FAILURE);
        }
    }

    if (geteuid() == 0) {
        fprintf(stderr, dict->ERRORUSER);
        exit(EXIT_FAILURE);
    }

    create_directory(TMP_DIR);
    if (verbose) {
        printf(dict->DIRNOTIFY);
    }

    if (verbose) {
        printf(dict->DIRSCANNING);
    }
    scan_directory(path, dict);

    if (verbose) {
        printf(dict->TEMPFILESCREATED);
    }

    char confirm[4];
    printf(dict->DELTEMPQUEST);
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0';

    if (strcmp(confirm, "yes") == 0) {
        delete_temp_files(dict);
    } else {
        printf(dict->DELTEMPNO);
    }

    return 0;
}
