#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_OPTION_LEN 10
#define MAX_PATH_LEN 256

extern struct ConfigBlock;

int silent = 0;
int verbose = 0;
/*
TODO
*/
int executeDeploy(ConfigBlock *config, int configs) {

    char path[MAX_PATH_LEN];
    snprintf(path, sizeof(path), "./*/-deploy.yaml");
    
    FILE *fp;
    char command[MAX_PATH_LEN + 20];
    
    snprintf(command, sizeof(command), "ls %s 2>/dev/null", path);
    fp = popen(command, "r");
    
    if (fp) {
        while (fgets(file, sizeof(file), fp) != NULL) {
            snprintf(command, sizeof(command), "kubectl apply -f %s", file);
            printf("Waiting %d secconds before %s deploy, reason %s", config->config_waittime, config->file_name, config->config_waitarg);
            sleep(config->config_waittime);
            system(command);
        }
        pclose(fp);
    }

    snprintf(command, sizeof(command), "ls %s 2>/dev/null", path);
    fp = popen(command, "r");

}

int deploy(ConfigBlock *config, int configs){
    
}
