#include <termios.h>
#include <unistd.h>
#include <string.h>

#define MAX_LEN 30

int main(void){
    struct termios newterm, oldterm;
    int i = 0;
    char pass[MAX_LEN + 1], ch, echo = '*';

    write(STDOUT_FILENO, "\nPASSWORD? ", 11);

    tcgetattr(STDIN_FILENO, &oldterm);
    newterm = oldterm;
    newterm.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &newterm);

    while(i < MAX_LEN && read(STDIN_FILENO, &ch, 1) && ch != '\n'){
        pass[i++] = ch;
        write(STDOUT_FILENO, &echo, 1);
    }
    pass[i] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);

    write(STDOUT_FILENO, "\n\nPassword: ", 12);
    write(STDOUT_FILENO, pass, strlen(pass));
    write(STDOUT_FILENO, "\n", 1);

    return 0;
}