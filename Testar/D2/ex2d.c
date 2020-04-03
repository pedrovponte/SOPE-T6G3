#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(void) {
    printf("1\n");
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

/*prints 1 \n 23 \n 45. AO por-se um \n depois do 1, pode-se forçar (pode não acontecer) a que o buffer se esvazie antes do fork, pelo que o 1 só vai ser imprimido 
no primeiro processo que correr
Tal também pode ser conseguido de forma mais eficaz se se desligar os buffers, através da chamada setbuf(stdout, NULL). Deste modo, vai ser sempre escrito logo 
que se faz a chamada printf*/