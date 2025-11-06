# 🧮 Teoria dos Grafos — 2ª VA

Implementação em **C** dos principais algoritmos estudados na disciplina de **Teoria dos Grafos**, desenvolvidos para a **2ª Avaliação (2ª VA)**.

O projeto segue as especificações fornecidas pelo professor e é compatível com os testes automáticos (`Bat1`).  
Cada algoritmo possui sua própria pasta e **Makefile independente**.

---

## 📘 Algoritmos Implementados

| Algoritmo  | Objetivo Principal |
|-------------|--------------------|
| **Prim** | Gera uma Árvore Geradora Mínima (AGM) usando vértice inicial |
| **Kruskal** | Gera uma Árvore Geradora Mínima (AGM) usando união de conjuntos |
| **Dijkstra** | Calcula o menor caminho a partir de um vértice origem |
| **Kosaraju** | Encontra os Componentes Fortemente Conexos (CFCs) de um grafo |

---

## ⚙️ Estrutura do Projeto

TeoriadosGrafos/
├── Prim/
│ ├── Prim.c
│ ├── Makefile
│
├── Kruskal/
│ ├── Kruskal.c
│ ├── Makefile
│
├── Dijkstra/
│ ├── Dijkstra.c
│ ├── Makefile
│
├── Kosaraju/
│ ├── Kosaraju.c
│ ├── Makefile
│
└── Bat1/
├── Bat1.sh
├── instances/
├── gabarito_agm.txt
└── ...

yaml
Copiar código

---

## 💻 Compilação

Cada algoritmo possui seu próprio **Makefile**.

### Exemplo (compilar o Prim):
```bash
cd Prim
make
Ou, se preferir compilar manualmente:

bash
Copiar código
gcc -Wall -g -std=c99 Prim.c -o prim
▶️ Execução
Todos os programas seguem o mesmo formato de execução:

diff
Copiar código
-h              : mostra o help
-o <arquivo>    : redireciona a saída para o arquivo
-f <arquivo>    : indica o arquivo de entrada
-i <vértice>    : vértice inicial (quando aplicável)
-s              : mostra a solução (ex: arestas da AGM)
Exemplo (Prim):
bash
Copiar código
./prim -f ../Bat1/instances/exemp.mtx -i 1
Saída:

Copiar código
14
Com a flag -s:

bash
Copiar código
./prim -f ../Bat1/instances/exemp.mtx -i 1 -s
Saída:

scss
Copiar código
(1,4) (4,2) (4,3) (3,5) (1,6)
🧩 Formato dos Arquivos de Entrada
Os grafos seguem o formato .mtx:

python-repl
Copiar código
n m
u1 v1 w1
u2 v2 w2
...
um vm wm
n: número de vértices

m: número de arestas

u v w: define uma aresta entre u e v com peso w

🧠 Descrição dos Algoritmos
🔹 Prim
Constrói uma Árvore Geradora Mínima (AGM) adicionando iterativamente a menor aresta que conecta um novo vértice à árvore.

🔹 Kruskal
Seleciona arestas em ordem crescente de peso e as adiciona se não formarem ciclo, usando a estrutura de conjuntos disjuntos (Union-Find).

🔹 Dijkstra
Calcula o menor caminho de um vértice origem até todos os outros vértices em um grafo ponderado (sem pesos negativos).

🔹 Kosaraju
Determina os Componentes Fortemente Conexos (CFCs) de um grafo dirigido, utilizando duas passagens de busca em profundidade (DFS).

🧰 Requisitos
Compilador GCC (C99 ou superior)

Sistema com suporte a Makefile

(Opcional) MinGW ou WSL no Windows

🧪 Testes Automáticos (Bat1)
A pasta Bat1 contém os arquivos de teste e gabaritos utilizados pelo professor.
Os programas são compatíveis com esses testes.

Rodar manualmente um teste:
bash
Copiar código
./prim -f ../Bat1/instances/exemp.mtx -i 1 -s

👨‍💻 Autor
Allan Douglas
Disciplina: Teoria dos Grafos
Avaliação: 2ª VA — 2025
