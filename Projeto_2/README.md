# Projeto 2 SOPE - Etapa 1

Trabalho realizado por:

* João Guilherme Guedes Perez da Silva Barra (up201808910)
* João Luís Azevedo Ferreira (201806716)
* Pedro Varandas da Costa Azevedo da Ponte (up201809694)

## Pormenores relevantes da implementação

Neste documento pretendemos demonstrar alguns pormenores que achamos relevantes na implementação da tarefa pedida.

Na primeira estapa do projeto, a operação do programa Q1 é mais restringida, pois a lotação e o número de threads do servidos é ilimitada. Deste modo, não introduzimos na struct responsável por guardar os dados necessários para o funcionamento deste processo variáveis que guardassem os valores para estes 2 atributos referidos.

Para correr o programa, basta escrever num terminal **./U1 -t nsecs fifoname** e noutro **Qn -t nsecs fifoname**.

### Criação de structs para guardar os dados fornecidos

A struct **[args_u1](./args_handler.h)** guarda os dados introduzidos no terminal para o funcionamento do processo multithread U1 (processo cliente). 

```C
typedef struct {
    int nsecs;
    char fifoname[200];
} args_u1;
``` 

A struct **[args_q1](./args_handler.h)** guarda os dados introduzidos no terminal para o funcionamento do processo multithread Q1 (processo servidor).

```C
typedef struct {
    int nsecs;
    char fifoname[200];
} args_q1;
```

Em ambas as structs anteriores o argumento **nsecs** representa o numero de segundos que o respetivo processo deve funcionar e o argumento **fifoname** representa o nome do canal público (FIFO) para comunicação entre cliente e servidor. Para que a aplicação funcione, é necessário que em ambos os processos o nome do FIFO seja o mesmo, senão não haverá comunicação.

A struct **[Pedido](./utils.h)** guarda os dados de um pedido do cliente/ resposta do servidor.

```C
typedef struct {
    int id;
    int pid;
    int tid;
    int dur;
    int pl;
} Pedido;
```

**id** - o número sequencial do pedido (gerado por Un);

**pid** - identificador de sistema do processo (cliente, no caso do pedido; servidor, no caso da resposta);

**tid** - identificador no sistema do thread cliente (cliente, no caso do pedido; servidor, no caso da resposta);

**dur** - duração, em milissegundos, de utilização (de um lugar) do Quarto de Banho (valor atribuído no pedido e repetido na resposta, se se der a ocupação; se não se der, por motivo de o Quarto de Banho estar em vias de encerrar, o servidor responde aqui com o valor -1;

**pl** – no de lugar que eventualmente lhe será atribuído no Quarto de Banho (no pedido, este campo é
preenchido com o valor -1 e na resposta terá o valor do lugar efetivamente ocupado ou também -1, na sequência de insucesso de ocupação, por motivo de encerramento).

### Funções que lêm o que foi introduzido no terminal e preenchem as structs args_u1 e args_q1 em [args_handler.c](./args_handler.c)

Através das funções **process_args_u** e **process_args_q** lê-se aquilo que foi introduzido no terminal, permitindo preencher as respetivas structs ou então retornando erro caso os argumentos introduzidos sejam inválidos.

### Geração aleatória do tempo de ocupação de um lugar da casa de banho por parte do cliente em [U1.c](./U1.c)

```C
request.dur = rand() % 3000001 + 1;
```

### Registo das operações na saída padrão (stdout) em [utils.c](./utils.c)

```C
int registLog(int id, pid_t pid, pthread_t tid, int dur, int pl, char* action){#
    char* log = malloc(500);
    time_t inst = time(NULL);
    sprintf(log, " %ld; %i; %.8u; %.8lu; %i; %i; %s\n", inst, id, pid, tid, dur, pl, action);
    write(STDOUT_FILENO, log, strlen(log));

    return 0;
}
```

em que **id, pid, tid, dur e pl** têm o mesmo significado que as variáveis utilizadas na **struct Pedido**. 

**inst** representa o valor retornado pela chamada ao sistema time(), na altura da produção da linha.

**action** representa siglas de 5 letras ajustadas às fases da operação que cada processo/ thread acabou de executar: IWANT, RECVD, ENTER, IAMIN, TIMUP, 2LATE, CLOSD, FAILD ou GAVUP.

## Algumas notas extra

* Mesmo depois da casa de banho fechar (servidor), ainda continua a receber pedidos de utilizadores que a querem frequentar enquanto o tempo de funcionamento do processo cliente não for atingido. No entanto, todos estes pedidos são regeitados a partir do momento em que a casa de banho fecha.

* Se o processo cliente for criado sem que exista nenhum servidor para receber os pedidos, o cliente vai ficar durante todo o tempo de execução a que se destinou a tentar comunicar com o servidor, aparecendo no terminal a mensagem "Connecting to server...".

* Enquanto o servidor não receber pedidos do cliente, aparecerá no terminal a mensagem "Waiting for requests...". 

* Quando a casa de banho é fechada, o servidor escreve no terminal a mensagem "Wc closed".