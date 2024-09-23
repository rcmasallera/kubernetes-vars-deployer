#include "deployer.h"

// compile(){ gcc -o deployer deployer.c; }
// clean(){ rm -rdf TEMP* deplo*; echo > deployer.c;}


int main() {

    Dictionary *dict = &spa;

    if (geteuid() == 0) {
        fprintf(stderr, spa.ERRORUSER);
        exit(EXIT_FAILURE);
    }

    create_directory(TMP_DIR);
    printf(spa.DIRNOTIFY);

    printf(spa.DIRSCANNING);
    scan_directory(".", dict);

    printf(spa.TEMPFILESCREATED);

    char confirm[4];
    printf(spa.DELTEMPQUEST);
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0'; 

    if (strcmp(confirm, "yes") == 0) {
        delete_temp_files(dict);  
    } else {
        printf(spa.DELTEMPNO);
    }

    return 0;
}

