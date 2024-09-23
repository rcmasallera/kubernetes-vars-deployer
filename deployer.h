#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "langdict.h"

#define MAX_PATH 1024
#define TMP_DIR "TEMP"

#include <stdio.h>
#include <string.h>

void create_directory(const char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        mkdir(dir, 0700);
    }
}

void get_tmp_file_name(const char *file_path, char *tmp_file) {
    const char *last_slash = strrchr(file_path, '/');
    const char *file_name = last_slash ? last_slash + 1 : file_path;  
    snprintf(tmp_file, MAX_PATH, "%s/tmp.%s", TMP_DIR, file_name);   
}


void delete_tmp_file(const char *file_path, Dictionary *lang) {
    char tmp_file[MAX_PATH];
    get_tmp_file_name(file_path, tmp_file); 
    if (remove(tmp_file) == 0) {
        printf(lang->DELTEMPYES, tmp_file);
    } else {
        perror(lang->DELTEMPERROR);
    }
}


void print_non_printable_ascii(const char *str, Dictionary *lang) {
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
        }
        else{
            *p = '\0';
        }
        p++;               
    }
    *output = '\0';        
}

void process_yaml_file(const char *file_path, Dictionary *lang) {
    printf(lang->YAMLPROCESSING);
    char tmp_file[MAX_PATH];
    get_tmp_file_name(file_path, tmp_file); 

    FILE *src = fopen(file_path, "r");
    if (!src) {
        perror(lang->FOPENERROR);
        return;
    }

    FILE *tmp = fopen(tmp_file, "w");
    if (!tmp) {
        perror(lang->TEMPFERROR);
        fclose(src);
        return;
    }

    printf(lang->FILEPROC, file_path);
    char line[1024];
    while (fgets(line, sizeof(line), src)) {
        char *pos = NULL;
        while ((pos = strstr(line, "%VAR-")) != NULL) { 
            char var_name[256] = {0};
            sscanf(pos, "%%VAR-%[^%%]%%", var_name); 
            
            var_name[strcspn(var_name, "\n")] = '\0'; 
            char replacement[256];
            printf(lang->VARVALUE, var_name);
            printf(lang->VARFOUNDED, var_name);
            
            fgets(replacement, sizeof(replacement), stdin);
            int length = strlen(replacement);
            while (length <= 1){
                fgets(replacement, sizeof(replacement), stdin);
                replacement[strcspn(replacement, "\n")] = '\0';
                replacement[strcspn(replacement, "\n")] = '\0';
                length = strlen(replacement);
            }
            remove_newlinesp(replacement); 
            char new_line[1024] = {0};
            strncpy(new_line, line, pos - line);  
            strcat(new_line, replacement);        
            strcat(new_line, pos + strlen(var_name) + 6);  
            strcpy(line, new_line); 
            memset(replacement, 0, sizeof(replacement));
            length = 0;
        }

        fputs(line, tmp); 
    }

    fclose(src);
    fclose(tmp);

    printf(lang->TFILECREATED, tmp_file);
}

void scan_directory(const char *dir_path, Dictionary *lang) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror(lang->DIROPENERROR);
        return;
    }

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
            scan_directory(path, lang);  
        }
        else if (strstr(entry->d_name, ".yaml")) {
          char confirm[4];
          printf(lang->PROCFILEQUEST, path);
          fgets(confirm, sizeof(confirm), stdin);
          confirm[strcspn(confirm, "\n")] = '\0';

          int strcmp_response = strcmp(confirm, "yes");
          if (strcmp_response == 0) {
              process_yaml_file(path, lang); 
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
        printf(lang->TEMPDIRDEL);
    } else {
        perror(lang->TEMPDIRDELERROR);
    }
}
