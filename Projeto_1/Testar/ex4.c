#include <stdio.h>
#include <stdlib.h>

/*#define MAX_NAME_LENGTH 50

int ex4a(void){
    char name[MAX_NAME_LENGTH];
    printf("Name ? "); 
    fgets(name, sizeof(name), stdin);
    printf("Hello %s! \n", name);
    return 0;
}

int ex4b(void){
    char name[MAX_NAME_LENGTH];
    int num = 0;
    printf("Name ? "); 
    fgets(name, sizeof(name), stdin);
    printf("Number: ");
    scanf("%i", &num);
    for(int j = 0; j < num; j++){
        printf("Hello %s! \n", name);
    }
    
    return 0;
}

int main(void){
    //ex4a();
    ex4b();
}*/

int ex4a(int argc, char* argv[]){
    printf("Hello ");
    for(int i = 1; i < argc; i++){
        printf("%s ", argv[i]);
    }
    printf("!\n");
    return 0;
}

int ex4b(int argc, char* argv[]){
    for(int i = 0; i < atoi(argv[argc - 1]); i++){
        printf("Hello ");
        for(int i = 1; i < argc - 1; i++){
            printf("%s ", argv[i]);
        }
        printf("!\n");
    }
    return 0;
}

int main(int argc, char* argv[]){
    //ex4a(argc, argv);
    ex4b(argc, argv);
}