/*Desenvolvido por Renato Craveiro (2018011392) - LEI-PL - Ano Letivo 2022/2023 - Sistemas Operativos*/

//Biblioteca ao qual este .c se refere com includes, estruturas e definições necessários
#include "soBay.h"

/*Divide uma string em várias palavras (divisão por " " ou "\n") denominadas por comandos e 
conta em quantas partes fica, armazenando essa informação toda na estrutura command que retorna*/
struct command splitString(char *buf){
    struct command returnValue;
    int i=0,j = 0;
    char *p = strtok (buf, " ");
    char *array[125];

    while (p != NULL)
    {
        array[i++] = p;
        p = strtok (NULL, " \n");
    }

    for (j = 0; j < i; ++j) 
        returnValue.comando[j]=array[j];

    returnValue.param=j;

    return returnValue;
}


//Devolve 0 se um caracter for número e 1 se não for. NÃO FUNCIONA A 100%
int isNumber(char s[]){
    for (int i = 0; s[i]!= '\n' || s[i]!= '\0' ; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

//Imprime a introdução do programa, apresentando o autor, links de publicação do autor e um pequeno "banner" do software
void printIntro(){
    printf("\n\n\
\
\
\
⠀⠈⠛⠻⠶⣶⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n \
⠀⠀⠀⠀⠀ ⠈⢻⣆⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⠀⠀⠀\n \
⠀⠀⠀⠀⠀⠀⠀ ⢻⡏⠉⠉⠉⠉⢹⡏⠉⠉⠉⠉⣿⠉⠉⠉⠉⠉⣹⠇⠀⠀⠀\n \
⠀⠀⠀⠀⠀⠀⠀ ⠈⣿⣀⣀⣀⣀⣸⣧⣀⣀⣀⣀⣿⣄⣀⣀⣀⣠⡿⠀⠀⠀⠀   _____ ____  ____             \n \
⠀⠀⠀⠀⠀⠀⠀⠀ ⠸⣧⠀⠀⠀⢸⡇⠀⠀⠀⠀⣿⠁⠀⠀⠀⣿⠃⠀⠀⠀⠀  / ___// __ \\/ __ )____ ___  __\n \
⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢹⣧⣤⣤⣼⣧⣤⣤⣤⣤⣿⣤⣤⣤⣼⡏⠀⠀⠀⠀⠀  \\__ \\/ / / / __  / __ `/ / / /\n \
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢿⠀⠀⢸⡇⠀⠀⠀⠀⣿⠀⠀⢠⡿⠀⠀⠀⠀⠀⠀ ___/ / /_/ / /_/ / /_/ / /_/ / \n \
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢸⣷⠤⠼⠷⠤⠤⠤⠤⠿⠦⠤⠾⠃⠀⠀⠀⠀⠀⠀/____/\\____/_____/\\__,_/\\__, /  \n \
⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⢀⣾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀                       /____/   \n \
        ⢾⣷⢶⣶⠶⠶⠶⠶⠶⠶⣶⠶⣶⡶⠀⠀⠀⠀⠀⠀⠀					    \n \
         ⣧⣠⡿⠀⠀⠀⠀⠀⠀⢷⣄⣼⠇⠀⠀⠀⠀⠀⠀⠀\n \
\n©Renato Craveiro | 2018011392 | LEI-PL | ISEC 2022/23 | Sistemas Operativos\n\
https://github.com/renato-craveiro/\n\
https://gamejolt.com/@renatocraveiro\n\
--------------------------------------------------------------------------------\n\
");
    return;
}
