#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {
    printf("1");
    if(fork() > 0) {
        printf("2");
        printf("3");
    } 
    else {
        printf("4");
        printf("5");
    }
    printf("\n");
    return 0;
}

/*prints 123 \n 145. com o printf, o buffer pode não ser logo esvaziado, pelo que ao fazer-se fork, vai originar-se duas cópias quase iguai do código original,
incluindo do conteúdo que se encontrava no buffer. Assim, como o 1 estava no buffer, então vai ser imprimido tanto quando se corre o processo pai como o processo filho.*/