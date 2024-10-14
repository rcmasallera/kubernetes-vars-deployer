#ifndef READ_CONFIG_H
#define READ_CONFIG_H

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
    char *key;
    char *value;
    int value_len;
} ConfigVar;

typedef struct {
    char *file_name;
    char *tmp_filename;
    int config_waittime; 
    char *config_waitarg;
    ConfigVar *vars;
    int *var_count;
    char *config_command; 
} ConfigBlock;

void read_config(const char *filename, ConfigBlock **config_blocks, int *block_count, Dictionary *lang) {
    FILE *file = fopen(filename, "r");
    unsigned int wait = FALSE;
    unsigned int arg = FALSE;
    unsigned int vars = FALSE;
    unsigned int command = FALSE;
    
    if (!file) {
        perror(lang->FOPENERROR);
        return;
    }

    char *line = NULL;
    size_t len = 0;
    ConfigBlock current_block = {NULL, NULL, 0, NULL, NULL, NULL, NULL};
    int in_vars_section = 0;
    int in_config_section = 0;

    *block_count = 0;
    *config_blocks = NULL;

    while (getline(&line, &len, file) != -1) {
        line[strcspn(line, "\n")] = 0;
        if (line[0] == 35){
            continue;
        }
        if (strlen(line) == 0) {
            continue;
        }

        if (line[0] == '[' && strstr(line, "]")) {
            if (in_vars_section || in_config_section){
                printf("FILE CONFIG ERROR");
                exit(SO_ERROR);
            }
            if (current_block.file_name != NULL) {
                (*block_count)++;
                *config_blocks = realloc(*config_blocks, sizeof(ConfigBlock) * (*block_count));
                (*config_blocks)[*block_count - 1] = current_block;
            }
            current_block.file_name = malloc(strlen(line) + 1);
            sscanf(line, "[%[^]]", current_block.file_name);
            current_block.config_waittime = 0;
            current_block.config_waitarg = NULL;
            current_block.vars = NULL;
            current_block.var_count = malloc(sizeof(int));
            *(current_block.var_count) = 0;
            in_vars_section = 0;
            wait = FALSE;
            arg = FALSE;
            vars = FALSE;
            command = FALSE;
        }
        else if (strstr(line, "CONFIG{")) {
            in_config_section = 1;
        } 
        else if (in_config_section && strstr(line, "CFG-WAITTIME=")) {
            sscanf(line, "CFG-WAITTIME=%d", &current_block.config_waittime);
            wait=TRUE;
        }
        else if (in_config_section && strstr(line, "CFG-WAITARG=")) {
            char *waitarg_value = strchr(line, '=') + 1;
            current_block.config_waitarg = malloc(strlen(waitarg_value) + 1);
            strcpy(current_block.config_waitarg, waitarg_value);
            arg=TRUE;
        }
        else if (in_config_section && strstr(line, "CFG-COMMAND=")) {
            char *config_command = strchr(line, '=') + 1;
            current_block.config_command = malloc(strlen(config_command) + 1);
            strcpy(current_block.config_command, config_command);
            command=TRUE;
        }
        else if (strstr(line, "VARS{")) {
            in_vars_section = 1;
        }
        else if (strstr(line, "}")) {
            if (in_config_section){
                if(!command){
                    char *no_command="#";
                    current_block.config_command = malloc(strlen(no_command) + 1);
                    strcpy(current_block.config_command, no_command);
                }
                if(!wait){
                    current_block.config_waittime=1;
                }
                if(!arg){
                    char *no_arg=" ";
                    current_block.config_waitarg = malloc(strlen(no_arg) + 1);
                    strcpy(current_block.config_waitarg, no_arg);
                }
                in_config_section = 0;
                continue;
            }
            else if(in_vars_section){
                in_vars_section = 0;
                vars = TRUE;
                continue;
            }
        }
        else if (in_vars_section) {
            ConfigVar var;
            char *key = malloc(strlen(line) + 1);
            char *value = malloc(strlen(line) + 1);
            sscanf(line, "%[^=]=%s", key, value);
            var.key = key;
            var.value = value;
            var.value_len = strlen(value);

            (*(current_block.var_count))++;
            current_block.vars = realloc(current_block.vars, sizeof(ConfigVar) * (*(current_block.var_count)));
            current_block.vars[*(current_block.var_count) - 1] = var;
        }

    }

    if (current_block.file_name != NULL) {
        (*block_count)++;
        *config_blocks = realloc(*config_blocks, sizeof(ConfigBlock) * (*block_count));
        (*config_blocks)[*block_count - 1] = current_block;
    }

    free(line);
    fclose(file);
}

void display_config_block(ConfigBlock *block) {
    printf("[%s]\n", block->file_name);
    printf("CONFIG:\n");
    printf("CFG-WAITTIME %d\n", block->config_waittime);
    printf("CFG-WAITARG %s\n\n", block->config_waitarg);
    printf("VARS:\n");
    for (int i = 0; i < *(block->var_count); i++) {
        printf("%s %s (Length: %d)\n", block->vars[i].key, block->vars[i].value, block->vars[i].value_len);
    }
    printf("\n");
}

void free_config_blocks(ConfigBlock *config_blocks, int block_count) {
    for (int i = 0; i < block_count; i++) {
        free(config_blocks[i].file_name);
        free(config_blocks[i].config_waitarg);
        for (int j = 0; j < *(config_blocks[i].var_count); j++) {
            free(config_blocks[i].vars[j].key);
            free(config_blocks[i].vars[j].value);
        }
        free(config_blocks[i].vars);
        free(config_blocks[i].var_count);
    }
    free(config_blocks);
}

#endif
