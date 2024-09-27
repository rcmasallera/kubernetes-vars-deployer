#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include "readconfig.h"

#define MAX_PATH 1024
#define TMP_DIR "TEMP"
#define CONFIG_FILE "vars.conf"


typedef struct {
    // Suponiendo que 'Dictionary' es una estructura definida en otra parte
} Dictionary;

typedef struct {
    int *time_w;
    char *time_w_arg;
    char *file;
    void *array; // Suponiendo que Config es una estructura definida en otra parte
    int *lines;
} ConfigSet;

// Prototipo de la función CreateConfigSet
void CreateConfigSet(ConfigSet *configSet, const char *path);

void process_yaml_files(const char *dir_path, Dictionary *lang, char **file_names, int *num_files, 
                        ConfigSet **config_sets, int *num_config_sets) {
    DIR *dir;
    struct dirent *entry;
    struct stat sb;
    char temp_dir[512];
    int found = 0;
    snprintf(temp_dir, sizeof(temp_dir), "%s/TEMP", dir_path);
    
    // Crear el directorio TEMP
    if (mkdir(temp_dir, 0755) && errno != EEXIST) {
        perror("Error al crear el directorio TEMP");
        return;
    }

    // Abrir el directorio
    dir = opendir(dir_path);
    if (!dir) {
        perror("Error al abrir el directorio");
        return;
    }

    // Escanear el directorio y subdirectorios
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Ignorar "." y ".."
        }

        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        // Comprobar si es un directorio
        if (stat(full_path, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            // Llamada recursiva para subdirectorios
            process_yaml_files(full_path, lang, file_names, num_files, config_sets, num_config_sets);
        } else if (strstr(entry->d_name, ".yaml")) {
            // Comprobar si el archivo tiene extensión .yaml
            char *file_name = strtok(entry->d_name, "."); // Obtener el nombre del archivo sin la extensión
            

            // Comparar con los nombres de archivo en el array
            for (int i = 0; i < *num_files; i++) {
                if (strcmp(file_names[i], file_name) == 0) {
                    found = 1;
                    break;
                }
            }

            // Preparar la ruta de destino en el directorio TEMP
            char dest_path[512];
            if (found) {
                snprintf(dest_path, sizeof(dest_path), "%s/tmp.%s", temp_dir, entry->d_name);
            } else {
                snprintf(dest_path, sizeof(dest_path), "%s/%s", temp_dir, entry->d_name);
            }

            // Copiar el archivo
            FILE *src_file = fopen(full_path, "rb");
            if (!src_file) {
                perror("Error al abrir el archivo de origen");
                continue;
            }
            FILE *dest_file = fopen(dest_path, "wb");
            if (!dest_file) {
                fclose(src_file);
                perror("Error al abrir el archivo de destino");
                continue;
            }

            char buffer[1024];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
                fwrite(buffer, 1, bytes, dest_file);
            }

            fclose(src_file);
            fclose(dest_file);

            // Llamar a CreateConfigSet si se encontró un archivo
            if (found) {
                ConfigSet *configSet = malloc(sizeof(ConfigSet)); // Asegúrate de manejar correctamente la memoria
                // Inicializa configSet según sea necesario
                CreateConfigSet(configSet, dest_path);
                // Aquí puedes agregar el configSet al array de config_sets si es necesario
            }
        }
    }

    closedir(dir);
}
