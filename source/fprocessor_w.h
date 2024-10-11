#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readconfig_w.h"

#define MAX_PATH 1024
#define TMP_DIR "TEMP"
#define CONFIG_FILE "vars.conf"

void create_directory(const char *dir, Dictionary *lang) {
    char tmp[MAX_PATH];
    strcpy(tmp, dir);
    size_t len = strlen(tmp);
    if (tmp[len - 1] == '\\') {
        tmp[len - 1] = 0;
    }
    char *p = tmp;
    while (*p) {
        if (*p == '\\') {
            *p = 0;
            if (CreateDirectory(tmp, NULL) == 0 && GetLastError() != ERROR_ALREADY_EXISTS) {
                printf(lang->DIRCREATFAIL, tmp);
                return;
            }
            *p = '\\';
        }
        p++;
    }
    
    if (CreateDirectory(tmp, NULL) == 0 && GetLastError() != ERROR_ALREADY_EXISTS) {
        printf(lang->DIRCREATFAIL, tmp);
    }
}

void _create_directory(const char *dir) {

    DWORD dwAttrib = GetFileAttributes(dir);
    if (dwAttrib == INVALID_FILE_ATTRIBUTES || !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
        CreateDirectory(dir, NULL);
    }
}

static void get_tmp_file_name(const char *file_path, char *tmp_file) {
    const char *last_backslash = strrchr(file_path, '\\');
    const char *file_name = last_backslash ? last_backslash + 1 : file_path;
    snprintf(tmp_file, MAX_PATH, "%s\\tmp.%s", TMP_DIR, file_name);
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
    char *output = str;

    while (*p) {
        if (*p != 10) {
            *output++ = *p;
        } else {
            *p = '\0';
        }
        p++;
    }
    *output = '\0';
}

static FILE * open_file(const char *file_path, char *error, char *mode, Dictionary *lang) {
    FILE *src = fopen(file_path, mode);
    if (!src) {
        perror(lang->FOPENERROR);
        return NULL;
    }
    return src;
}

static void process_yaml_file(const char *file_path, Dictionary *lang, ConfigBlock *cfgblock, unsigned int *manual) {
    int config_lines;
    ConfigVar **vars = NULL;

    if (cfgblock) {
        config_lines = *cfgblock->var_count;
        vars = &cfgblock->vars;
    } else {
        if (*manual == FALSE) {
            exit(EXIT_FAILURE);
        }
    }

    printf("%s", lang->YAMLPROCESSING);
    char tmp_file[MAX_PATH];
    get_tmp_file_name(file_path, tmp_file);

    FILE *src = open_file(file_path, lang->FOPENERROR, "r", lang);
    FILE *tmp = open_file(tmp_file, lang->FOPENERROR, "w", lang);

    if ((!src) || (!tmp)) {
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
            var_name[strcspn(var_name, "\n")] = 0;
            char new_line[1024] = {0};
            unsigned short var_assigned = FALSE;
            int compare_result;
            if (*manual == 0 && cfgblock) {
                for (int i = 0; i < *cfgblock->var_count; i++) {
                    compare_result = strcmp(var_name, cfgblock->vars[i].key);
                    if (compare_result == 0) {
                        strncpy(replacement, cfgblock->vars[i].value, sizeof(replacement) - 1);
                        remove_newlinesp(replacement);
                        var_assigned = TRUE;
                        break;
                    }
                }
            }

            if (!var_assigned) {
                printf("%s%s\n", lang->VARVALUE, var_name);
                printf("%s%s\n", lang->VARFOUNDED, var_name);
                fgets(replacement, sizeof(replacement), stdin);
                remove_newlinesp(replacement);
            }
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

    printf(lang->TFILECREATED, tmp_file);
}

unsigned int is_yes(char str[]) {
    if ((strcasecmp(str, "yes") == 0) || (strcasecmp(str, "y") == 0)) {
        return TRUE;
    }
    return FALSE;
}

void scan_directory(const char *dir_path, Dictionary *lang, ConfigBlock *cfgblocks, unsigned int *manual, unsigned int *blocks) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char path[MAX_PATH];

    snprintf(path, MAX_PATH, "%s\\*", dir_path);
    hFind = FindFirstFile(path, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror(lang->DIROPENERROR);
        return;
    }

    ConfigBlock *current_config = cfgblocks;

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }

        snprintf(path, MAX_PATH, "%s\\%s", dir_path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            scan_directory(path, lang, cfgblocks, manual, blocks);
        } else if (strstr(findFileData.cFileName, ".yaml")) {
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
                    printf("%d\n", x);
                    if (strcmp(current_config->file_name, filename) == 0) {
                        current_config = cfgblocks;
                        process_yaml_file(path, lang, current_config, manual);
                    }
                    else{
                        current_config ++;
                    }
                    if(x == (*blocks - 1)){
                        printf(lang->FILENOTINCFG, filename);
                        fgets(confirm, sizeof(confirm), stdin);
                        remove_newlinesp(confirm);
                        unsigned int confirmation = TRUE;
                        confirmation = is_yes(confirm);
                        if (confirmation == TRUE){
                            process_yaml_file(path, lang, current_config, &confirmation);
                        }
                        else{continue;}
                    }
                }
                current_config = cfgblocks;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void delete_temp_files(Dictionary *lang) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char file_path[MAX_PATH];

    snprintf(file_path, MAX_PATH, "%s\\*", TMP_DIR);
    hFind = FindFirstFile(file_path, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror(lang->TDIROPERROR);
        return;
    }

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }
        snprintf(file_path, MAX_PATH, "%s\\%s", TMP_DIR, findFileData.cFileName);
        if (remove(file_path) == 0) {
            printf(lang->FILEDELETED, file_path);
        } else {
            perror(lang->DELFILEERROR);
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);

    if (RemoveDirectory(TMP_DIR) == 0) {
        printf("%s", lang->TEMPDIRDEL);
    } else {
        perror(lang->TEMPDIRDELERROR);
    }
}
