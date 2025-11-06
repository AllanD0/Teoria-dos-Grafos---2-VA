#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXE 2000005
#define LINEBUF (1<<16)

typedef struct Aresta {
    int u, v;
    int peso;
} Aresta;

Aresta arestas[MAXE];
int num_arestas = 0;

void ajuda() {
    printf("Uso:\n");
    printf("  -h              : mostra o help\n");
    printf("  -o <arquivo>    : redireciona a saida para o arquivo\n");
    printf("  -f <arquivo>    : indica o arquivo que contem o grafo de entrada\n");
    printf("  -s              : mostra a solucao (arestas da AGM)\n");
    printf("  -i <vertice>    : vertice inicial (ignorado em Kruskal)\n");
}

int compararAresta(const void *a, const void *b) {
    Aresta *ea = (Aresta*)a;
    Aresta *eb = (Aresta*)b;
    if (ea->peso != eb->peso) return (ea->peso < eb->peso) ? -1 : 1;
    if (ea->u != eb->u) return ea->u - eb->u;
    return ea->v - eb->v;
}

int *uf_pai, *uf_rank;
int uf_buscar(int x) {
    if (uf_pai[x] != x) uf_pai[x] = uf_buscar(uf_pai[x]);
    return uf_pai[x];
}
int uf_unir(int a, int b) {
    a = uf_buscar(a); b = uf_buscar(b);
    if (a == b) return 0;
    if (uf_rank[a] < uf_rank[b]) uf_pai[a] = b;
    else if (uf_rank[b] < uf_rank[a]) uf_pai[b] = a;
    else { uf_pai[b] = a; uf_rank[a]++; }
    return 1;
}

void ler_grafo(const char *nomeArquivoEntrada, int *pn, int *pm) {
    FILE *f = fopen(nomeArquivoEntrada, "r");
    if (!f) { fprintf(stderr, "Erro ao abrir arquivo %s\n", nomeArquivoEntrada); exit(1); }
    char linha[LINEBUF];
    int n=-1,m=-1;
    while (fgets(linha, sizeof(linha), f)) {
        char *s = linha;
        while (*s==' '||*s=='\t') s++;
        if (*s=='\0'||*s=='\n'||*s=='\r'||*s=='%'||*s=='#') continue;
        int a,b,c;
        if (sscanf(s,"%d %d %d",&a,&b,&c)==3) {
            n=a; m=c; break;
        } else if (sscanf(s,"%d %d",&a,&b)==2) {
            n=a; m=b; break;
        }
    }
    if (n<=0) { fprintf(stderr,"Erro: numero de vertices invalido\n"); fclose(f); exit(1); }

    while (fgets(linha, sizeof(linha), f)) {
        char *s=linha; while (*s==' '||*s=='\t') s++;
        if (*s=='\0'||*s=='\n'||*s=='\r'||*s=='%'||*s=='#') continue;
        int u,v,peso;
        if (sscanf(s,"%d %d %d",&u,&v,&peso)>=2) {
            if (sscanf(s,"%d %d %d",&u,&v,&peso)==2) peso = 1;
            if (u>=1 && u<=n && v>=1 && v<=n) {
                if (u==v) continue;
                if (u < v) { arestas[num_arestas].u = u; arestas[num_arestas].v = v; arestas[num_arestas].peso = peso; num_arestas++; }
                else { arestas[num_arestas].u = v; arestas[num_arestas].v = u; arestas[num_arestas].peso = peso; num_arestas++; }
            }
        }
    }
    fclose(f);
    *pn = n; *pm = num_arestas;
}

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    char *nomeArquivoEntrada = NULL, *nomeArquivoSaida = NULL;
    int mostrarSolucao = 0;

    for (int i=1;i<argc;i++){
        if (strcmp(argv[i],"-h")==0) { ajuda(); return 0; }
        else if (strcmp(argv[i],"-f")==0 && i+1<argc) nomeArquivoEntrada = argv[++i];
        else if (strcmp(argv[i],"-o")==0 && i+1<argc) nomeArquivoSaida = argv[++i];
        else if (strcmp(argv[i],"-s")==0) mostrarSolucao = 1;
    }
    if (!nomeArquivoEntrada) { fprintf(stderr,"Erro: arquivo de entrada nao especificado. Use -f <arquivo>\n"); return 1; }
    if (nomeArquivoSaida) if (!freopen(nomeArquivoSaida,"w",stdout)) { fprintf(stderr,"Erro ao redirecionar saida\n"); return 1; }

    int n,m;
    ler_grafo(nomeArquivoEntrada, &n, &m);
    qsort(arestas, num_arestas, sizeof(Aresta), compararAresta);

    uf_pai = (int*)malloc((n+1)*sizeof(int));
    uf_rank = (int*)malloc((n+1)*sizeof(int));
    for (int i=1;i<=n;i++){ uf_pai[i]=i; uf_rank[i]=0; }

    long long custoTotal = 0;
    int arestasPegas = 0;
    Aresta *solucao = (Aresta*)malloc(sizeof(Aresta)*(n+5));
    for (int i=0;i<num_arestas && arestasPegas < n-1;i++){
        Aresta *e = &arestas[i];
        if (uf_buscar(e->u) != uf_buscar(e->v)) {
            uf_unir(e->u, e->v);
            custoTotal += e->peso;
            solucao[arestasPegas++] = *e;
        }
    }

    if (mostrarSolucao) {
        int primeiro = 1;
        for (int i=0;i<arestasPegas;i++){
            if (!primeiro) printf(" ");
            printf("(%d,%d)", solucao[i].u, solucao[i].v);
            primeiro = 0;
        }
        printf("\n");
    } else {
        printf("%lld\n", custoTotal);
    }

    free(uf_pai); free(uf_rank); free(solucao);
    return 0;
}