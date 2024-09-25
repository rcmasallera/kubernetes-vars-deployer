#include "deployer.h"

int main(int argc, char *argv[]) {
    Dictionary *dict = &spa;
    char *path = ".";
    char *config_file = NULL;
    int verbose = 0;
    int manual = FALSE;
    int config_lines = 0;
    ConfigSet *config_set;
    Config *config_array;
    int length = 0;
    // Args processing
    if (geteuid() == 0) {
        fprintf(stderr, "%s", dict->ERRORUSER);
        exit(EXIT_FAILURE);
    }
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
        else if (strncmp(argv[i], "-m", 2) == 0 || strncmp(argv[i], "--manual", 10) == 0) {
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
    
    if (config_file){
        config_lines = get_config_lines(config_file, dict);
        config_array = malloc(config_lines * sizeof(Config));
        config_set = malloc(sizeof(ConfigSet));
        read_conf_file(config_file, dict, &config_lines, config_set, config_array);
    }
    else{
        if (!manual){
            config_file=CONFIG_FILE;
            config_lines = get_config_lines(config_file, dict);
            config_array = malloc(config_lines * sizeof(Config));
            config_set = malloc(sizeof(ConfigSet));
            read_conf_file(config_file, dict, &config_lines, config_set, config_array);
        }
        else{
            fprintf(stderr, "%s", dict->MANUALMODE);
        }
    }

    create_directory(TMP_DIR, dict);
    if (verbose) {
        printf("%s", dict->DIRNOTIFY);
    }

    if (verbose) {
        printf("%s", dict->DIRSCANNING);
    }
    scan_directory(path, dict, config_set, &manual);

    if (verbose) {
        printf("%s", dict->TEMPFILESCREATED);
    }

    char confirm[4];
    printf("%s", dict->DELTEMPQUEST);
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0';
    remove_newlinesp(confirm);
    length = strlen(confirm);
    while (length <= 1){
      fgets(confirm, sizeof(confirm), stdin);
      length = strlen(confirm);
      remove_newlinesp(confirm);
    };
    if (strcmp(confirm, "yes") == 0) {
        delete_temp_files(dict);
    } else {
        printf("%s", dict->DELTEMPNO);
    }

    free(config_array);
    return 0;
}
