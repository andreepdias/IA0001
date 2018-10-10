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

https://stackoverflow.com/questions/18176591/importerror-no-module-named-matplotlib-pyplot
https://github.com/google/python-subprocess32/issues/38

 sudo apt-get install -y python-subprocess32

pip uninstall matplotlib
python3 -m pip install matplotlib

sudo apt-get install python3-tk
**/

#include <bits/stdc++.h>
using namespace std;

typedef vector<int> vi;
typedef vector<bool> vb;

#define PI 3.14159265

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

    ifstream arquivo_parametros("parametros");
    arquivo >> var >> clau;
    arquivo_parametros >> n >> t0 >> tn;

    for(int i = 0; i < var; i++){
        variaveis.push_back(false);
    }

    while(arquivo >> a >> b >> c >> x){
        clausulas.push_back(Clausula(a, b, c));
    }
    // printClausulas(clausulas, var, clau);

}

void randomize(vb &variaveis){
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

double calculateTemperature0(int i, int n, double t0, double tn){
    double t = t0 - (double)i * ((t0 - tn) / n);
    // cout << t << endl;
    return t;
}

double calculateTemperature1(int i, int n, double t0, double tn){
    double t = t0 * pow((tn / t0), ((double)i / n));
    // cout << t << endl;
    return t;
}

double calculateTemperature2(int i, int n, double t0, double tn){
    double a = (double)((double)(t0 - tn) * (double)(n + 1)) / (double)n;
    double b = (double)t0 - a;
    double t = ((double)a / (double)(i + 1)) + (double)b;
    // cout << t << endl;
    return t;
}

double calculateTemperature3(int i, int n, double t0, double tn){
    double a = (double)log(t0 - tn) / log(n);
    double t = t0 - pow((double)i, a);
    // cout << t << endl;
    return t;
}

double calculateTemperature4(int i, int n, double t0, double tn){
    double t = ((t0 - tn) / (1 + exp(3 * (i - ((double)n / 2))))) + tn;
    // cout << t << endl;
    return t;
}

double calculateTemperature5(int i, int n, double t0, double tn){
    double t =  ((double)1 / 2) * (t0 - tn) * (1 + cos((i * PI) / n)) + tn;
    // cout << t << endl;
    return t;
}

double calculateTemperature6(int i, int n, double t0, double tn){
    double t = ((double)1 / 2) * (t0 - tn) * (1 - tanh( ((double)(10 * i) / n) - 5  )) + tn;
    // cout << t << endl;
    return t;
}

double calculateTemperature7(int i, int n, double t0, double tn){
    double t = ((t0 - tn) / cosh((double)(10 * i) / n)) + tn;
    // cout << t << endl;
    return t;
}

double calculateTemperature8(int i, int n, double t0, double tn){
    double a = ((double)1 / n) * log((double) t0 / tn);
    double t = t0 * exp(-a * i);
    // cout << t << endl;
    return t;
}

double calculateTemperature9(int i, int n, double t0, double tn){
    double a = ((double)1 / pow(n, 2)) * log((double) t0 / tn);
    double t = t0 * exp(-a * pow(i, 2));
    // cout << t << endl;
    return t;
}

double calculateTemperature(int i, int n, double t0, double tn, int cs){
    switch(cs){
        case 0:
            return calculateTemperature0(i, n, t0, tn);
        case 1:
            return calculateTemperature1(i, n, t0, tn);
        case 2:
            return calculateTemperature2(i, n, t0, tn);
        case 3:
            return calculateTemperature3(i, n, t0, tn);
        case 4:
            return calculateTemperature4(i, n, t0, tn);
        case 5:
            return calculateTemperature5(i, n, t0, tn);
        case 6:
            return calculateTemperature6(i, n, t0, tn);
        case 7:
            return calculateTemperature7(i, n, t0, tn);
        case 8:
            return calculateTemperature8(i, n, t0, tn);
        case 9:
            return calculateTemperature9(i, n, t0, tn);
        default:
            return calculateTemperature0(i, n, t0, tn);
    }
}

int annealing(vb &variaveis, vc &clausulas, int atual, int n, double t0, double tn, int cs){

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
        t = calculateTemperature(k, n, t0, tn, cs);
        if(a > atual){
            conf_atual = conf_candidata;
            atual = a;
        }else if(a < atual){
            p = (double) rand() / RAND_MAX;
            c = exp((double)(a - atual) / t);
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
    int numero_iteracoes, atual, tipo_resfriamento = stoi(argv[2]);
    double temperatura_inicial, temperatura_final;

    readFile(clausulas, variaveis, arquivo, numero_iteracoes, temperatura_inicial, temperatura_final);

    double media = 0;
    int x;
    for(int i = 0; i < 10; i++){
        randomize(variaveis);
        atual = avaliate(variaveis, clausulas);
        x = annealing(variaveis, clausulas, atual, numero_iteracoes, temperatura_inicial, temperatura_final, tipo_resfriamento);
        media += x;
        cout << "Execucao " << i+1 << ": " << x << endl;
    }
    media /= 10;
    cout << "Media: " << media << endl;

}
