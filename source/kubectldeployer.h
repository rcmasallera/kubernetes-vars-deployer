#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

void executeDeploy(char *configpath, char *yaml_prefix, char *yaml_sufix, int *secconds_wait, char *wait_arg) {
    setenv("KUBECONFIG", *configpath, 1);

    //system("kubectl apply -f ./*/ingress-nlb.yaml");

    // Mensaje de espera
    if(wait_arg){
        printf("Waiting %d secconds for %s... \n", *secconds_wait, *wait_arg);
    }
    else{
        printf("Waiting %d secconds", *secconds_wait)
    }

    sleep(*secconds_wait);

    // Abrir el directorio actual
    struct dirent *de;
    DIR *wirkdir = opendir("./TEMP");

    if (wirkdir == NULL) {
        printf("Could not open current directory\n");
        return;
    }

    // Recorrer los archivos en los subdirectorios
    while ((de = readdir(wirkdir)) != NULL) {
        if (de->d_type == DT_DIR && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            char path[1024];
            snprintf(path, sizeof(path), "%s/*-deploy.yaml", de->d_name);

            // Formar el comando kubectl
            char command[2048];
            snprintf(command, sizeof(command), "kubectl apply -f %s", path);
            printf("Applying: %s\n", command);
            system(command);
        }
    }

    // Cerrar el directorio
    closedir(wirkdir);
}

int main() {
    executeDeploy();
    return 0;
}