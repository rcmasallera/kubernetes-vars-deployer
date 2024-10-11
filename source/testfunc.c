#include <stdio.h>
#include <strings.h>

#define TRUE 1
#define FALSE 0

unsigned int is_yes(char *str) {
    if (str == NULL) {
        return FALSE;
    }
    return (strcasecmp(str, "yes") == 0 || strcasecmp(str, "y") == 0);
}

int main() {
    char *test1 = "yes";
    char *test2 = "y";
    char *test3 = "no";
    char *test4 = "YES";  // Para probar mayúsculas

    printf("Test 1: %s -> %d\n", test1, is_yes(test1));
    printf("Test 2: %s -> %d\n", test2, is_yes(test2));
    printf("Test 3: %s -> %d\n", test3, is_yes(test3));
    printf("Test 4: %s -> %d\n", test4, is_yes(test4));

    return 0;
}