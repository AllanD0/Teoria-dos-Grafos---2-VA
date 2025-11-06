#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXV 10005
#define LINEBUF (1<<16)
#define MAX_LINES 200000

typedef struct Adjacencia {
    int v, w;
    struct Adjacencia *next;
} Adjacencia;

Adjacencia *inicio[MAXV];

typedef struct No {
    int v;
    long long d;
} No;

No *heapVetor;
int heapTamanho = 0;

void heapInsere(int v, long long d){ heapVetor[++heapTamanho].v=v; heapVetor[heapTamanho].d=d; int i=heapTamanho; while(i>1){int p=i>>1; if(heapVetor[p].d<=heapVetor[i].d) break; No t=heapVetor[p]; heapVetor[p]=heapVetor[i]; heapVetor[i]=t; i=p;} }
No heapRemove(){ No ret=heapVetor[1]; heapVetor[1]=heapVetor[heapTamanho--]; int i=1; while(1){int l=i<<1,r=l+1, s=i; if(l<=heapTamanho && heapVetor[l].d<heapVetor[s].d) s=l; if(r<=heapTamanho && heapVetor[r].d<heapVetor[s].d) s=r; if(s==i) break; No t=heapVetor[i]; heapVetor[i]=heapVetor[s]; heapVetor[s]=t; i=s;} return ret; }
int heapVazio(){ return heapTamanho==0; }

void adiciona_aresta(int u,int v,int w){
    Adjacencia *no=(Adjacencia*)malloc(sizeof(Adjacencia));
    no->v=v;
    no->w=w;
    no->next=inicio[u];
    inicio[u]=no;
}

void ajuda(){
    printf("Uso:\n");
    printf("  -h              : mostra o help\n");
    printf("  -o <arquivo>    : redireciona a saida para o arquivo\n");
    printf("  -f <arquivo>    : indica o arquivo que contem o grafo de entrada\n");
    printf("  -i <vertice>    : vertice inicial (para Dijkstra)\n");
}

void ler_grafo_flexivel(const char *nomeArquivoEntrada, int *ponteiro_n){
    FILE *arquivo = fopen(nomeArquivoEntrada,"r");
    if(!arquivo){ fprintf(stderr,"Erro ao abrir arquivo %s\n", nomeArquivoEntrada); exit(1); }

    char bufferLinha[LINEBUF];
    char **linhas = (char**)malloc(sizeof(char*)*MAX_LINES);
    int numLinhas = 0;

    while(fgets(bufferLinha,sizeof(bufferLinha),arquivo)){
        char *pLinha = bufferLinha;
        while(*pLinha==' '||*pLinha=='\t') pLinha++;
        if(*pLinha=='\0' || *pLinha=='\n' || *pLinha=='\r') continue;
        if(*pLinha=='%' || *pLinha=='#') continue;
        linhas[numLinhas++] = strdup(pLinha);
        if(numLinhas >= MAX_LINES) break;
    }
    fclose(arquivo);
    if(numLinhas==0){ fprintf(stderr,"Erro: arquivo %s vazio ou sem linhas validas\n", nomeArquivoEntrada); free(linhas); exit(1); }

    int n=-1, n2=-1, m=-1;
    int temCabecalho = 0;
    int lidos = sscanf(linhas[0], "%d %d %d", &n, &n2, &m);
    if(lidos >= 2){
        int possivel_m = (lidos==3? m : n2);
        if(possivel_m <= numLinhas-1 && possivel_m >= 0) temCabecalho = 1;
        else {
            if(lidos==3) temCabecalho = 1;
            else temCabecalho = 0;
        }
    } else temCabecalho = 0;

    int linhaInicial = temCabecalho ? 1 : 0;
    if(!temCabecalho) n = 0;

    for(int i=linhaInicial; i<numLinhas; i++){
        int u=0,v=0; double pesoTemp=0;
        int contagem = sscanf(linhas[i], "%d %d %lf", &u, &v, &pesoTemp);
        if(contagem >= 2){
            if(!temCabecalho){
                if(u > n) n = u;
                if(v > n) n = v;
            }
        }
    }

    if(n <= 0){ for(int i=0;i<numLinhas;i++) free(linhas[i]); free(linhas); fprintf(stderr,"Erro: numero de vertices invalido no arquivo\n"); exit(1); }
    if(n >= MAXV){ for(int i=0;i<numLinhas;i++) free(linhas[i]); free(linhas); fprintf(stderr,"Erro: numero de vertices excede MAXV\n", nomeArquivoEntrada); exit(1); }

    for(int i=0;i<=n;i++) inicio[i]=NULL;

    for(int i=linhaInicial; i<numLinhas; i++){
        int u=0,v=0; double pesoTemp=0;
        int contagem = sscanf(linhas[i], "%d %d %lf", &u, &v, &pesoTemp);
        if(contagem >= 2){
            int w = (contagem==3)? (int)pesoTemp : 1;
            if(u>=1 && u<=n && v>=1 && v<=n){
                adiciona_aresta(u,v,w);
            }
        }
    }

    for(int i=0;i<numLinhas;i++){ free(linhas[i]); }
    free(linhas);
    *ponteiro_n = n;
}

int main(int argc, char *argv[]){
    setbuf(stdout,NULL);
    char *nomeArquivoEntrada=NULL, *nomeArquivoSaida=NULL;
    int verticeInicial=1;

    if(argc==1){ ajuda(); return 0; }
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"-h")==0){ ajuda(); return 0; }
        else if(strcmp(argv[i],"-f")==0 && i+1<argc) nomeArquivoEntrada = argv[++i];
        else if(strcmp(argv[i],"-o")==0 && i+1<argc) nomeArquivoSaida = argv[++i];
        else if(strcmp(argv[i],"-i")==0 && i+1<argc) verticeInicial = atoi(argv[++i]);
    }
    if(!nomeArquivoEntrada){ fprintf(stderr,"Erro: arquivo de entrada nao especificado. Use -f <arquivo>\n"); return 1; }
    if(nomeArquivoSaida){ if(!freopen(nomeArquivoSaida,"w",stdout)){ fprintf(stderr,"Erro ao redirecionar saida\n"); return 1; } }

    int n;
    ler_grafo_flexivel(nomeArquivoEntrada, &n);

    if(verticeInicial < 1 || verticeInicial > n) verticeInicial = 1;

    long long *distancias = (long long*)malloc((n+1)*sizeof(long long));
    for(int i=1;i<=n;i++) distancias[i]=LLONG_MAX;

    heapVetor = (No*)malloc(sizeof(No)*(n*10+10));
    heapTamanho = 0;

    distancias[verticeInicial]=0;
    heapInsere(verticeInicial,0);

    while(!heapVazio()){
        No atual = heapRemove();
        int u = atual.v;
        long long distAtual = atual.d;

        if(distAtual != distancias[u]) continue;

        for(Adjacencia *adj = inicio[u]; adj; adj = adj->next){
            int v = adj->v;
            int w = adj->w;
            if(distancias[u] != LLONG_MAX && distancias[v] > distancias[u] + w){
                distancias[v] = distancias[u] + w;
                heapInsere(v, distancias[v]);
            }
        }
    }

    for(int i=1;i<=n;i++){
        if(i>1) printf(" ");
        if(distancias[i]==LLONG_MAX) printf("%d:-1", i);
        else printf("%d:%lld", i, distancias[i]);
    }
    printf("\n");

    free(distancias);
    free(heapVetor);
    for(int i=1;i<=n;i++){
        Adjacencia *adj=inicio[i];
        while(adj){
            Adjacencia *temp=adj;
            adj=adj->next;
            free(temp);
        }
    }
    return 0;
}