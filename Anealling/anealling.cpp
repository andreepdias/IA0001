/**

representar solução do problema:
    vetor de cláusulas

avaliar solução do problema:
    número de cláusulas satisfeitas

pertubar solução do problema:
    alterar uma variável

..
Clever Algorithms: Nature-Inspired Programming Recipes
www.cleveralgorithms.com/nature-inspired/stochastic/random_search.html

dicas execuoes:
1.
varias vezes
independente

2.
grafico de convergencia
resultado da solução a cada iteração

gnu-plot
**/

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef vector<bool> vb;

struct Clausula{
    int *v;
    Clausula(int a, int b, int c){
        v = new int[3];
        if(a > 0) a--; else a++;
        if(b > 0) b--; else b++;
        if(c > 0) c--; else c++;
        v[0] = a, v[1] = b, v[2] = c;
    }
    bool isTrue(vb &variaveis){
        return (v[0] > 0 ? (variaveis[v[0]]) : (not variaveis[-v[0]]))
            or (v[1] > 0 ? (variaveis[v[1]]) : (not variaveis[-v[1]]))
            or (v[2] > 0 ? (variaveis[v[2]]) : (not variaveis[-v[2]]));
    }
};

typedef vector<Clausula> vc;

void printClausulas(vc &clausulas, int var, int clau){
    cout << var << " " << clau << endl;
    for(int i = 0; i < clau; i++){
        cout << i+1 << ".\t" << clausulas[i].v[0] << "\t" << clausulas[i].v[1] << "\t" << clausulas[i].v[2] << endl;
    }
}

void printVariables(vb &variaveis){
    for(int i = 0; i < variaveis.size(); i++){
        cout << i+1 << ".\t";
        if(variaveis[i]){
            cout << "true";
        }else{
            cout << "false";
        }
        cout << endl;
    }
}

void readFile(vc &clausulas, vb &variaveis, ifstream &arquivo, int &n, double &t0, double &tn){
    int var, clau, a, b, c, x;

    arquivo >> var >> clau >> n >> t0 >> tn;

    for(int i = 0; i < var; i++){
        variaveis.push_back(false);
    }

    while(arquivo >> a >> b >> c >> x){
        clausulas.push_back(Clausula(a, b, c));
    }
    // printClausulas(clausulas, var, clau);

}

void initVariables(vb &variaveis){
    srand(time(NULL));
    int x;
    for(int i = 0; i < variaveis.size(); i++){
        x = rand() % 2;
        x == 0 ? (variaveis[i] = false) : (variaveis[i] = true);
    }
    // printVariables(variaveis);
}

int avaliate(vb &variaveis, vc &clausulas){
    int cont = 0;

    for(int i = 0; i < clausulas.size(); i++){
        if(clausulas[i].isTrue(variaveis)){
            cont++;
        }
    }
    return cont;
}

void flip(vb &variaveis, int i){
    if(variaveis[i]){
        variaveis[i] = false;
    }else{
        variaveis[i] = true;
    }
}

double calculateTemperature(int i, int n, double t0, double tn){
    double a = log(t0 - tn) / log(n);
    return t0 - pow((double)i, a);
}

double calculateTemperature2(int i, int n, double t0, double tn){
    return t0 - (double)i * ((t0 - tn) / n);
}

int annealing(vb &variaveis, vc &clausulas, int atual, int n, double t0, double tn){

    int s = variaveis.size(), a;
    double p, t, c;
    vb conf_candidata;
    vb conf_atual;

    conf_atual = variaveis;

    for(int k = 0; k < n; k++){
        conf_candidata = conf_atual;

        for(int i = 0; i < s; i++){         //FLIP 5% DAS VARIÁVEIS
            p = (double) rand() / RAND_MAX;
            if(p <= 0.05){
                flip(conf_candidata, i);
            }
        }

        a = avaliate(conf_candidata, clausulas);         //VERIFICA SE CONFIGURAÇÃO GERADA É MELHOR QUE ATUAL
        if(a >= atual){
            conf_atual = conf_candidata;
            atual = a;
        }else{
            t = calculateTemperature(k, n, t0, tn);
            p = (double) rand() / RAND_MAX;
            c = exp((double)(a - atual)/t);
            if(p <= c){
                conf_atual = conf_candidata;
                atual = a;
            }
        }
    }
    return atual;
}


int main(int argc, char const *argv[]) {
    srand(time(NULL));

    ifstream arquivo(argv[1]);

    vb variaveis;
    vc clausulas;
    int numero_iteracoes, atual;
    double temperatura_inicial, temperatura_final;

    readFile(clausulas, variaveis, arquivo, numero_iteracoes, temperatura_inicial, temperatura_final);

    int x;
    double media = 0;
    for(int i = 0; i < 10; i++){
        initVariables(variaveis);
        atual = avaliate(variaveis, clausulas);
        x = annealing(variaveis, clausulas, atual, numero_iteracoes, temperatura_inicial, temperatura_final);
        media += x;
        cout << "Execucao " << i+1 << ": " << x << endl;
    }
    media /= 10;
    cout << "Media: " << media << endl;




}
