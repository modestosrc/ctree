#include <dirent.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ANSI_RESET      "\x1B[0m"
#define ANSI_BLUE       "\x1B[34m"
#define ANSI_GREEN      "\x1B[32m"
#define ANSI_RED        "\x1B[31m"
void caminho(char nomeDiretorio[], char caminhoCompleto[PATH_MAX]); 
int offsetGalhos(char nomeDiretorio[]);
void galhos(char caminhoCompleto[], int offset);
int tree(char nomeDiretorio[], int offset);

int main(void) {
    char nomeDiretorio[PATH_MAX];

    if (getcwd(nomeDiretorio, sizeof(nomeDiretorio)) == NULL) {
        printf("Erro obtendo diretório atual!");
        return 1;
    }
    
    printf("%s\n", nomeDiretorio);
    int offset = offsetGalhos(".");
    if (tree(".", offset) == 1) return 1;
    return 0;
}

void caminho(char nomeDiretorio[], char caminhoCompleto[PATH_MAX]) {

    if (realpath(nomeDiretorio, caminhoCompleto) == NULL) {
        printf("Erro obtendo caminho do diretório!");
    }
}

int offsetGalhos(char nomeDiretorio[]) {
    char caminhoCompleto[PATH_MAX];
    int offset = -1;

    caminho(nomeDiretorio, caminhoCompleto);

    for (int i = 0; i <= strlen(caminhoCompleto); i++) {
        if (caminhoCompleto[i] == '/') offset++;
    }

    return offset;
}

void galhos(char caminhoCompleto[], int offset) {
    for (int i = 0; i < strlen(caminhoCompleto); i++)
        if (caminhoCompleto[i] == '/' && offset-- <= 0) printf(ANSI_BLUE " |" ANSI_RESET);
}

int tree(char nomeDiretorio[], int offset) {
    struct dirent *entry;
    DIR *diretorio;
    char caminhoCompleto[PATH_MAX];

    caminho(nomeDiretorio, caminhoCompleto);

    diretorio = opendir(caminhoCompleto);

    if (diretorio == NULL) {
        printf("Erro abrindo diretório: %s", caminhoCompleto);
        return 1;
    }    

    while ((entry = readdir(diretorio))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (entry->d_name[0] == '.') continue;

        char subcaminho[PATH_MAX];
        snprintf(subcaminho, PATH_MAX, "%s/%s", caminhoCompleto, entry->d_name);


        if (entry->d_type == DT_DIR) {
            galhos(caminhoCompleto, offset);
            printf(ANSI_GREEN "%s\n" ANSI_RESET, entry->d_name);
            tree(subcaminho, offset);
        }

        if (entry->d_type == DT_REG) {
            galhos(caminhoCompleto, offset);
            printf("%s\n" , entry->d_name);
        }
    }

    closedir(diretorio);
    return 0;
}
