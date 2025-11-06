#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXV 5005
#define LINEBUF (1<<16)

typedef struct NoAdjacencia {
    int v, w;
    struct NoAdjacencia *proximo;
} NoAdjacencia;

NoAdjacencia *inicio[MAXV];

void ajuda() {
    printf("Uso:\n");
    printf("  -h              : mostra o help\n");
    printf("  -o <arquivo>    : redireciona a saida para o arquivo\n");
    printf("  -f <arquivo>    : indica o arquivo que contem o grafo de entrada\n");
    printf("  -i <vertice>    : vertice inicial (para Prim)\n");
    printf("  -s              : mostra a solucao (arestas da AGM)\n");
}

void adiciona_aresta(int u, int v, int w) {
    NoAdjacencia *no = (NoAdjacencia*)malloc(sizeof(NoAdjacencia));
    no->v = v; no->w = w; no->proximo = inicio[u]; inicio[u] = no;
}

int buscarChaveMinima(int chave[], int naAGM[], int n) {
    int min = INT_MAX, indice_min = -1;
    for (int v = 1; v <= n; v++) {
        if (!naAGM[v] && chave[v] < min) {
            min = chave[v];
            indice_min = v;
        }
    }
    return indice_min;
}

void prim_completo(int n, int verticeInicial, int mostrarSolucao) {
    int *pai = malloc((n+1)*sizeof(int));
    int *chave = malloc((n+1)*sizeof(int));
    int *naAGM = malloc((n+1)*sizeof(int));

    for (int i = 1; i <= n; i++) {
        pai[i] = -1;
        chave[i] = INT_MAX;
        naAGM[i] = 0;
    }

    if (verticeInicial >= 1 && verticeInicial <= n) chave[verticeInicial] = 0;
    else chave[1] = 0;

    long long custoTotal = 0;

    for (;;) {
        int u = buscarChaveMinima(chave, naAGM, n);
        if (u == -1) break;
        naAGM[u] = 1;
        if (pai[u] != -1)
            custoTotal += chave[u];

        for (NoAdjacencia *p = inicio[u]; p; p = p->proximo) {
            int v = p->v, w = p->w;
            if (!naAGM[v] && w < chave[v]) {
                chave[v] = w;
                pai[v] = u;
            }
        }

        int concluido = 1;
        for (int i = 1; i <= n; i++)
            if (!naAGM[i]) { concluido = 0; break; }
        if (concluido) break;
    }

    if (mostrarSolucao) {
        int primeiro = 1;
        for (int i = 1; i <= n; i++) {
            if (pai[i] != -1) {
                if (!primeiro) printf(" ");
                printf("(%d,%d)", pai[i], i);
                primeiro = 0;
            }
        }
        printf("\n");
    } else {
        printf("%lld\n", custoTotal);
    }

    free(pai);
    free(chave);
    free(naAGM);
}

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    char *nomeArquivoEntrada = NULL, *nomeArquivoSaida = NULL;
    int mostrarSolucao = 0, verticeInicial = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) { ajuda(); return 0; }
        else if (strcmp(argv[i], "-f") == 0 && i+1 < argc) nomeArquivoEntrada = argv[++i];
        else if (strcmp(argv[i], "-o") == 0 && i+1 < argc) nomeArquivoSaida = argv[++i];
        else if (strcmp(argv[i], "-s") == 0) mostrarSolucao = 1;
        else if (strcmp(argv[i], "-i") == 0 && i+1 < argc) verticeInicial = atoi(argv[++i]);
    }

    if (!nomeArquivoEntrada) {
        fprintf(stderr, "Erro: arquivo de entrada nao especificado. Use -f <arquivo>\n");
        return 1;
    }

    if (nomeArquivoSaida && freopen(nomeArquivoSaida, "w", stdout) == NULL) {
        fprintf(stderr, "Erro ao redirecionar saida para %s\n", nomeArquivoSaida);
        return 1;
    }

    FILE *f = fopen(nomeArquivoEntrada, "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivoEntrada);
        return 1;
    }

    for (int i = 0; i < MAXV; i++) inicio[i] = NULL;

    char linha[LINEBUF];
    int n = -1, m = -1;

    while (fgets(linha, sizeof(linha), f)) {
        char *s = linha;
        while (*s == ' ' || *s == '\t') s++;
        if (*s == '\0' || *s == '\n' || *s == '\r' || *s == '#' || *s == '%')
            continue;
        int n2;
        if (sscanf(s, "%d %d %d", &n, &n2, &m) == 3)
            break;
        else if (sscanf(s, "%d %d", &n, &m) == 2)
            break;
    }

    if (n <= 0 || n >= MAXV) {
        fprintf(stderr, "Erro: numero de vertices invalido\n");
        fclose(f);
        return 1;
    }

    int u, v;
    double pesoTemp;
    while (fgets(linha, sizeof(linha), f)) {
        char *s = linha;
        while (*s == ' ' || *s == '\t') s++;
        if (*s == '\0' || *s == '\n' || *s == '\r' || *s == '#' || *s == '%') continue;

        int contagem = sscanf(s, "%d %d %lf", &u, &v, &pesoTemp);
        int w = 1;
        if (contagem >= 2) {
            if (contagem == 3) w = (int)pesoTemp;
            if (u >= 1 && u <= n && v >= 1 && v <= n) {
                adiciona_aresta(u, v, w);
                adiciona_aresta(v, u, w);
            }
        }
    }
    fclose(f);

    if (verticeInicial < 1 || verticeInicial > n) verticeInicial = 1;
    prim_completo(n, verticeInicial, mostrarSolucao);

    for (int i = 1; i <= n; i++) {
        NoAdjacencia *p = inicio[i];
        while (p) {
            NoAdjacencia *temp = p;
            p = p->proximo;
            free(temp);
        }
    }

    return 0;
}