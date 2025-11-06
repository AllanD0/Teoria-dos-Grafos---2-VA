#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXV 10005
#define LINEBUF (1<<15)

typedef struct No {
    int v;
    struct No *proximo;
} No;

No *grafo[MAXV], *grafoReverso[MAXV];
int n;
int visitado[MAXV], ordem[MAXV], componente[MAXV], contadorOrdem = 0;

void ajuda() {
    printf("Uso:\n");
    printf("  -h              : mostra o help\n");
    printf("  -o <arquivo>    : redireciona a saida para o arquivo\n");
    printf("  -f <arquivo>    : indica o arquivo que contem o grafo de entrada\n");
}

void adiciona_aresta(No **adj, int u, int v) {
    No *novoNo = (No*)malloc(sizeof(No));
    novoNo->v = v;
    novoNo->proximo = adj[u];
    adj[u] = novoNo;
}

void busca1(int u) {
    visitado[u] = 1;
    for (No *p = grafo[u]; p; p = p->proximo)
        if (!visitado[p->v]) busca1(p->v);
    ordem[contadorOrdem++] = u;
}

void busca2(int u, int c) {
    componente[u] = c;
    for (No *p = grafoReverso[u]; p; p = p->proximo)
        if (componente[p->v] == 0) busca2(p->v, c);
}

void ler_grafo(const char *nomeArquivoEntrada) {
    FILE *f = fopen(nomeArquivoEntrada, "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivoEntrada);
        exit(1);
    }

    char linha[LINEBUF];
    int m = -1;
    n = -1;

    while (fgets(linha, sizeof(linha), f)) {
        char *s = linha;
        while (*s == ' ' || *s == '\t') s++;
        if (*s == '\0' || *s == '\n' || *s == '\r' || *s == '%' || *s == '#')
            continue;
        if (sscanf(s, "%d %d", &n, &m) == 2) break;
    }

    if (n <= 0) {
        fprintf(stderr, "Erro: numero de vertices invalido\n");
        fclose(f);
        exit(1);
    }

    for (int i = 0; i <= n; i++) {
        grafo[i] = NULL;
        grafoReverso[i] = NULL;
    }

    int u, v;
    while (fscanf(f, "%d %d", &u, &v) == 2) {
        if (u >= 1 && u <= n && v >= 1 && v <= n) {
            adiciona_aresta(grafo, u, v);
            adiciona_aresta(grafoReverso, v, u);
        }
    }

    fclose(f);
}

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    char *nomeArquivoEntrada = NULL, *nomeArquivoSaida = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) { ajuda(); return 0; }
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) nomeArquivoEntrada = argv[++i];
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) nomeArquivoSaida = argv[++i];
    }

    if (!nomeArquivoEntrada) {
        fprintf(stderr, "Erro: arquivo de entrada nao especificado. Use -f <arquivo>\n");
        return 1;
    }

    if (nomeArquivoSaida) if (!freopen(nomeArquivoSaida, "w", stdout)) {
        fprintf(stderr, "Erro ao redirecionar saida\n");
        return 1;
    }

    ler_grafo(nomeArquivoEntrada);

    for (int i = 1; i <= n; i++) visitado[i] = 0;
    contadorOrdem = 0;

    for (int i = 1; i <= n; i++)
        if (!visitado[i]) busca1(i);

    for (int i = 1; i <= n; i++) componente[i] = 0;
    int c = 0;

    for (int i = contadorOrdem - 1; i >= 0; i--) {
        int v = ordem[i];
        if (componente[v] == 0) busca2(v, ++c);
    }

    for (int i = 1; i <= c; i++) {
        for (int v = 1; v <= n; v++)
            if (componente[v] == i) printf("%d ", v);
        printf("\n");
    }

    return 0;
}