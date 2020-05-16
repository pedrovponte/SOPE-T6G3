# Projeto 2 SOPE - Etapa 1

Trabalho realizado por:

* João Guilherme Guedes Perez da Silva Barra (up201808910)
* João Luís Azevedo Ferreira (201806716)
* Pedro Varandas da Costa Azevedo da Ponte (up201809694)

## Pormenores relevantes da implementação

Neste documento pretendemos demonstrar alguns pormenores que achamos relevantes na implementação da tarefa pedida.

Na segunda etapa do projeto, terminamos a implementação do projeto pedido, continuando o trabalho já realizado na 1ª etapa (com as devidas correções aconselhadas). 

Assim, implementamos a parte correspondente à existência de um número máximo de threads a atender pedidos e também a lotação do quarto de banho, continuando o programa a funcionar também como funcionava na 1ª etapa.

Para correr o programa, basta escrever num terminal **./U2 <-t nsecs> fifoname** e noutro **./Q2 <-t nsecs> [-l nplaces] [-n threads] fifoname**.

### Criação de structs para guardar os dados fornecidos

A struct **[args_u2](./args_handler.h)** guarda os dados introduzidos no terminal para o funcionamento do processo multithread U1 (processo cliente). 

```C
typedef struct {
    int nsecs;
    char fifoname[200];
} args_u2;
``` 

A struct **[args_q2](./args_handler.h)** guarda os dados introduzidos no terminal para o funcionamento do processo multithread Q1 (processo servidor).

```C
typedef struct {
    int nsecs;
    int nplaces;
    int nthreads;
    char fifoname[200];
} args_q2;
```

Em ambas as structs anteriores o argumento **nsecs** representa o numero de segundos que o respetivo processo deve funcionar e o argumento **fifoname** representa o nome do canal público (FIFO) para comunicação entre cliente e servidor. Para que a aplicação funcione, é necessário que em ambos os processos o nome do FIFO seja o mesmo, senão não haverá comunicação.

Na struct **args_q2**, o argumento **nplaces** representa a lotação do quarto de banho e o argumento **nthreads** representa o número máximo de threads a atender pedidos.

A struct **[Pedido](./utils.h)** guarda os dados de um pedido do cliente/ resposta do servidor.

```C
typedef struct {
    int id;
    pid_t pid;
    pthread_t tid;
    int dur;
    int pl;
} Pedido;
```

**id** - o número sequencial do pedido (gerado por Un);

**pid** - identificador de sistema do processo (cliente, no caso do pedido; servidor, no caso da resposta);

**tid** - identificador no sistema do thread cliente (cliente, no caso do pedido; servidor, no caso da resposta);

**dur** - duração, em milissegundos, de utilização (de um lugar) do Quarto de Banho (valor atribuído no pedido e repetido na resposta, se se der a ocupação; se não se der, por motivo de o Quarto de Banho estar em vias de encerrar, o servidor responde aqui com o valor -1;

**pl** – número de lugar que eventualmente lhe será atribuído no Quarto de Banho (no pedido, este campo é
preenchido com o valor -1 e na resposta terá o valor do lugar efetivamente ocupado ou também -1, na sequência de insucesso de ocupação, por motivo de encerramento).

### Funções que lêm o que foi introduzido no terminal e preenchem as structs args_u2 e args_q2 em [args_handler.c](./args_handler.c)

Através das funções **process_args_u** e **process_args_q** lê-se aquilo que foi introduzido no terminal, permitindo preencher as respetivas structs ou então retornando erro caso os argumentos introduzidos sejam inválidos.

### Geração aleatória do tempo de ocupação de um lugar da casa de banho por parte do cliente em [U2.c](./U2.c)

```C
request.dur = rand() % 20 + 1;
```

### Registo das operações na saída padrão (stdout) em [utils.c](./utils.c)

```C
int registLog(int id, pid_t pid, pthread_t tid, int dur, int pl, char* action){
    char* log = malloc(500);
    time_t inst = time(NULL);
    sprintf(log, " %ld ; %i ; %.8u ; %.8lu ; %i ; %i ; %s\n", inst, id, pid, tid, dur, pl, action);
    write(STDOUT_FILENO, log, strlen(log));

    return 0;
}
```

em que **id, pid, tid, dur e pl** têm o mesmo significado que as variáveis utilizadas na **struct Pedido**. 

**inst** representa o valor retornado pela chamada ao sistema time(), na altura da produção da linha.

**action** representa siglas de 5 letras ajustadas às fases da operação que cada processo/ thread acabou de executar: IWANT, RECVD, ENTER, IAMIN, TIMUP, 2LATE, CLOSD, FAILD ou GAVUP.

### Implementação de uma queue

Para ser mais fácil controlar os lugares da casa de banho, implementamos uma queue no ficheiro [queue.c](./queue.c).

Esta implementação foi baseada em https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/.

### Implementação e funcionamento do arguento -l nplaces**

Para implementar esta funcionalidade, recorremos à utilização de um **mutex** e de um **semáforo**. Esta implementação está feita em [Q2.c](./Q2.c).

O semáforo serve para controlar o número de pedidos que entram na casa de banho, sendo inicializado com o valor do número de casas de banho que são introduzidas a seguir ao argumento **-l**. Deste modo, esse número de clientes entram na casa de banho e os restantes ficam a aguardar que alguma saia para entrarem.

O mutex é utilizado para controlar a secção crítica do servidor, de modo a impedir que dois clientes ocupem simultaneamente o mesmo lugar.

### Implementação e funcionamento do argumento -n threads**

Para implementar esta funcionalidade, recorremos à utilização de um **semáforo**. Esta implementação está feita em [Q2.c](./Q2.c).

O semáforo serve para controlar o número de threads que estão em funcionamento simultaneamente. É inicializada com o valor introduzido a seguir ao argumento -n. 

Assim, sempre que uma thread é criada, decrementa-se o semáforo, e volta-se a incrementar quando a thread termina a sua função, podendo criar-se uma nova.

### Algumas notas extra

* Mesmo depois da casa de banho fechar (servidor), ainda continua a receber pedidos de utilizadores que a querem frequentar enquanto o tempo de funcionamento do processo cliente não for atingido. No entanto, todos estes pedidos são rejeitados a partir do momento em que a casa de banho fecha.

* Se o processo cliente for criado sem que exista nenhum servidor para receber os pedidos, o cliente vai ficar durante todo o tempo de execução a que se destinou a tentar comunicar com o servidor.

* Ao longo de todo o programa encontra-se em alguns sítios a variável **tries** que serve para contar o número de chamadas às funções **open** ou **read** e terminar o programa com erro caso estas funções sejam chamadas consecutivamente mais do que um certo número de vezes.