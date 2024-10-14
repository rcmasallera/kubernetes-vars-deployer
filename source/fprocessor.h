#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include "readconfig.h"

#define MAX_PATH 1024
#define TMP_DIR "TEMP"
#define CONFIG_FILE "vars.conf"

void create_directory(const char *dir, Dictionary *lang) {
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    
    if (tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            if (mkdir(tmp, 0700) != 0 && errno != EEXIST) {
                printf(lang->DIRCREATFAIL, tmp);
                return;
            }
            *p = '/';
        }
    }
    if (mkdir(tmp, 0700) != 0 && errno != EEXIST) {
        printf(lang->DIRCREATFAIL, tmp);
    }
}

void _create_directory(const char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        mkdir(dir, 0700);
    }
}

static void get_tmp_file_name(const char *file_path, char *tmp_file) {
    const char *last_slash = strrchr(file_path, '/');
    const char *file_name = last_slash ? last_slash + 1 : file_path;  
    snprintf(tmp_file, MAX_PATH, "%s/tmp.%s", TMP_DIR, file_name);   
}

static void delete_tmp_file(const char *file_path, Dictionary *lang) {
    char tmp_file[MAX_PATH];
    get_tmp_file_name(file_path, tmp_file); 
    if (remove(tmp_file) == 0) {
        printf(lang->DELTEMPYES, tmp_file);
    } else {
        perror(lang->DELTEMPERROR);
    }
}

static void print_non_printable_ascii(const char *str, Dictionary *lang) {
    while (*str) {
        if (*str < 32 || *str > 126) {
            printf("\n");
            printf(lang->NONPRINTABLE, (unsigned char)*str);
            printf("\n");
        } else {
            printf("%c", *str); 
        }
        str++;
    }
}

void remove_newlinesp(char *str) {
    char *p = str;        

    while (*p) {          
        if (*p == 10) {   
            *p = '\0';
        }
        p++;               
    }       
}

static FILE * open_file(const char *file_path, char *error, char *mode, Dictionary *lang){
    FILE *src = fopen(file_path, mode);
    if (!src) {
        perror(lang->FOPENERROR);
        return NULL;
    }
    return src;
}

static void process_yaml_file(const char *file_path, Dictionary *lang, ConfigBlock *cfgblock, unsigned int *manual) {
    int config_lines;
    ConfigVar **vars=NULL;

    if (cfgblock){
       config_lines = *cfgblock->var_count;
       vars = &cfgblock->vars;
    }
    else{
        if(*manual == FALSE){
            exit(EXIT_FAILURE);
        }
    }

    printf("%s", lang->YAMLPROCESSING);
    char tmp_file[MAX_PATH];
    get_tmp_file_name(file_path, tmp_file); 
    
    FILE *src = open_file(file_path, lang->FOPENERROR, "r", lang);
    FILE *tmp = open_file(tmp_file, lang->FOPENERROR, "w", lang);
    
    if((!src)||(!tmp)){
        exit(EXIT_FAILURE);
    }
    
    printf("%s %s\n", lang->FILEPROC, file_path);
    char line[1024];

    while (fgets(line, sizeof(line), src)) {
        char *pos = NULL;
        while ((pos = strstr(line, "%VAR-")) != NULL) { 
            char var_name[128] = {0};
            char replacement[256];
            sscanf(pos, "%%VAR-%[^%%]%%", var_name); 
            var_name[strcspn(var_name, "\n")] = '\0'; 
            char new_line[1024] = {0};
            unsigned short var_assigned = FALSE;
            int compare_result;
            if(*manual == 0 && cfgblock){
                for (int i = 0; i < *cfgblock->var_count; i++) {
                    compare_result = strcmp(var_name, cfgblock->vars[i].key);
                    if (compare_result == 0){
                        strncpy(replacement, cfgblock->vars[i].value, sizeof(replacement) - 1);
                        remove_newlinesp(replacement);
                        var_assigned = TRUE;
                        break;
                    }
                    else{
                        continue;
                    }
                };
            }

            if(!var_assigned){
                printf("%s%s\n", lang->VARVALUE, var_name);
                printf("%s%s\n", lang->VARFOUNDED, var_name);
                fgets(replacement, sizeof(replacement), stdin);
                remove_newlinesp(replacement);
            };
            remove_newlinesp(replacement);
            strncpy(new_line, line, pos - line);  
            strcat(new_line, replacement);        
            strcat(new_line, pos + strlen(var_name) + 6);  
            strcpy(line, new_line); 
            var_assigned = FALSE;
            memset(replacement, 0, sizeof(replacement));
        }

        fputs(line, tmp); 
    }

    fclose(src);
    fclose(tmp);
    char *ptmpfile = tmp_file;
    cfgblock->tmp_filename = ptmpfile;    
    printf(lang->TFILECREATED, tmp_file);
}

unsigned int _is_yes(char str[]) {
    if ((strcasecmp(str, "yes") == 0) || (strcasecmp(str, "y") == 0)) {
        if(strcasecmp(str, "yes") == 0){
            printf("WAS yes");
        }
        else if (strcasecmp(str, "y") == 0){
            printf("WAS y");
        }
        return TRUE;
    }
    return FALSE;
}


unsigned int __is_yes(char str[]) {
    unsigned int result = FALSE;
    if (strcasecmp(str, "yes") == 0) {
        result = TRUE;
    }
    else{
        result = FALSE;
    }
    if(result == 0){
        if (strcasecmp(str, "y") == 0){
            result = TRUE;
        }
        else{
            result = FALSE;
        }
    }
    return result;
}

unsigned int is_yes(char *str) {
    return (strcasecmp(str, "yes") == 0 || strcasecmp(str, "y") == 0) ? TRUE : FALSE;
}

const char* get_filename(const char* path) {
    const char* filename = strrchr(path, '/');
    return (filename) ? filename + 1 : path;
}

void scan_directory(const char *dir_path, Dictionary *lang, ConfigBlock *cfgblocks, unsigned int *manual, unsigned int *blocks) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror(lang->DIROPENERROR);
        return;
    }
    ConfigBlock *current_config = cfgblocks;

    struct dirent *entry;
    char path[MAX_PATH];
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, MAX_PATH, "%s/%s", dir_path, entry->d_name);
        struct stat st;
        stat(path, &st);

        if(strncmp(entry->d_name, "tmp.", 4) == 0){
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            scan_directory(path, lang, cfgblocks, manual, blocks); 
        }
        else if (strstr(entry->d_name, ".yaml")) {
            char confirm[MAX_LINE] = {0};
            printf(lang->PROCFILEQUEST, path);
            fgets(confirm, sizeof(confirm), stdin);
            remove_newlinesp(confirm);
            unsigned int confirmation = TRUE;
            confirmation = is_yes(confirm);
            if (confirmation == TRUE) {
                for (int x = 0; x < *blocks; x++) {
                    const char *filename = get_filename(path);
                    if(!current_config){
                        printf("NULL_POINTER_EXCEPTION %s", filename);
                    }
                    if (strcmp(current_config->file_name, filename) == 0) {
                        current_config = cfgblocks;
                        process_yaml_file(path, lang, current_config, manual);
                        break;
                    }
                    else{
                        current_config ++;
                    }
                    if(x == (*blocks - 1) && (confirmation == FALSE)){
                        printf(lang->FILENOTINCFG, filename);
                        fgets(confirm, sizeof(confirm), stdin);
                        remove_newlinesp(confirm);
                        unsigned int confirmation = TRUE;
                        confirmation = is_yes(confirm);
                        if (confirmation == TRUE){
                            process_yaml_file(path, lang, current_config, &confirmation);
                        }
                        else{
                            continue;
                        }
                    }
                }
                current_config = cfgblocks;
            } else {
                delete_tmp_file(path, lang);  
                printf(lang->PROCFILENO, path);
            }
        }
    }

    closedir(dir);
}


void delete_temp_files(Dictionary *lang) {
    DIR *dir = opendir(TMP_DIR);
    if (!dir) {
        perror(lang->TDIROPERROR);
        return;
    }

    struct dirent *entry;
    char file_path[MAX_PATH];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        snprintf(file_path, MAX_PATH, "%s/%s", TMP_DIR, entry->d_name);
        if (remove(file_path) == 0) {
            printf(lang->FILEDELETED, file_path);
        } else {
            perror(lang->DELFILEERROR);
        }
    }

    closedir(dir);

    if (rmdir(TMP_DIR) == 0) {
        printf("%s", lang->TEMPDIRDEL);
    } else {
        perror(lang->TEMPDIRDELERROR);
    }
}


int executeDeploy(ConfigBlock *config, int *configs, Dictionary *lang) {

    char command[MAX_LINE] = {0};
    unsigned int command_defined = FALSE;

    ConfigBlock *current_config = config;
    for(int i = 0; i < *configs; i++){
        if (*(config->config_command) == 35){
            printf(lang->NOCOMMAND, config->file_name);
            char confirm[4];
            fgets(confirm, sizeof(confirm), stdin);
            remove_newlinesp(confirm);
            if (is_yes(confirm) == TRUE) {
                printf("%s", lang->SYNTAX);
                fgets(command, sizeof(command), stdin);
                while(strlen(command) < 3){
                    printf("%s", lang->SYNTAX);
                    memset(command, 0, sizeof(command));
                    fgets(command, sizeof(command), stdin);
                }
                printf(command, config->tmp_filename, "\n");
            }
        }
        else{
            strncpy(command, current_config->config_command, sizeof(command) - 1);
        }
        system(command);
        printf(lang->CFGWAITARG, (current_config->config_waittime), (current_config->file_name), (current_config->config_waitarg));
        sleep(current_config->config_waittime);
        memset(command, 0, sizeof(command));
        current_config ++;
    }

}

int deploy(ConfigBlock *config, int configs){
    return 0;
}
