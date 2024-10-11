#ifdef _WIN32
#include <windows.h>
#include "fprocessor_w.h"
#else
#include "fprocessor.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

int main(int argc, char *argv[]) {
    ConfigBlock *config_blocks = NULL;
    int block_count = 0;
    Dictionary *dict = &spa;
    char *path = ".";
    char *config_file = NULL;
    int verbose = 0;
    unsigned int manual = FALSE;
    unsigned int autodeploy = FALSE;

#ifdef _WIN32
    if (GetVersion() < 0x80000000) {
        fprintf(stderr, "%s", dict->ERRORUSER);
        exit(EXIT_FAILURE);
    }
#else
    if (geteuid() == 0) {
        fprintf(stderr, "%s", dict->ERRORUSER);
        exit(EXIT_FAILURE);
    }
#endif

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
        else if (strncmp(argv[i], "-d", 2) == 0 || strncmp(argv[i], "--deploy", 8) == 0) {
            autodeploy = TRUE;
        } 
        else if (strncmp(argv[i], "-m", 2) == 0 || strncmp(argv[i], "--manual", 8) == 0) {
            char *manual_mode = strchr(argv[i], '=');
            if (!manual_mode && i + 1 < argc) {
                manual_mode = argv[++i];
            } else if (manual_mode) {
                manual_mode++;
            } else {
                print_usage();
                exit(EXIT_FAILURE);
            }
            if (strcmp(manual_mode, "yes") == 0) {
                manual = TRUE;
            } else if (strcmp(manual_mode, "no") == 0) {
                manual = FALSE;
            } else {
                fprintf(stderr, dict->UNSUPPORTEDMODE, manual_mode);
                exit(EXIT_FAILURE);
            }
        } 
        else {
            print_usage();
            exit(EXIT_FAILURE);
        }
    }

    if (config_file) {
        read_config(config_file, &config_blocks, &block_count, dict);
        for (int i = 0; i < block_count; i++) {
            display_config_block(&config_blocks[i]);
        }
    }
    else {
        if (!manual) {
            read_config(CONFIG_FILE, &config_blocks, &block_count, dict);
        }
        else {
            fprintf(stderr, "%s", dict->MANUALMODE);
        }
    }

    create_directory(TMP_DIR, dict);
    if (verbose) {
        printf("%s", dict->DIRNOTIFY);
    }

    if (verbose) {
        for (int i = 0; i < block_count; i++) {
            display_config_block(&config_blocks[i]);
        }
        printf("%s", dict->DIRSCANNING);
    }

    scan_directory(path, dict, config_blocks, &manual, &block_count);

    if (verbose) {
        printf("%s", dict->TEMPFILESCREATED);
    }

    char confirm[4];
    printf("%s", dict->DELTEMPQUEST);
    fgets(confirm, sizeof(confirm), stdin);
    remove_newlinesp(confirm);
    if (is_yes(confirm) == TRUE) {
        delete_temp_files(dict);
    } else {
        printf("%s", dict->DELTEMPNO);
    }

    free_config_blocks(config_blocks, block_count);
    return 0;
}
