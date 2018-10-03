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

void readFile(vc &clausulas, vb &variaveis, ifstream &arquivo){
    int var, clau, a, b, c, x;

    arquivo >> var >> clau;

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

int avaliate(vc &clausulas, vb &variaveis){
    int cont = 0;

    for(int i = 0; i < clausulas.size(); i++){
        if(clausulas[i].isTrue(variaveis)){
            cont++;
        }
    }
    return cont;
}

void pertubate(vb &variaveis, int n){
    srand(time(NULL));

    int s = variaveis.size(), x;

    while(n--){
        x = rand() % s;
        if(variaveis[x]){
            variaveis[x] = false;
        }else{
            variaveis[x] = true;
        }
    }

}

int main(int argc, char const *argv[]) {

    ifstream arquivo(argv[1]);

    vb variaveis;
    vc clausulas;

    readFile(clausulas, variaveis, arquivo);
    initVariables(variaveis);

    cout << avaliate(clausulas, variaveis) << endl;

    string ch;

    while(true){
        cin >> ch;
        pertubate(variaveis, 3);
        cout << avaliate(clausulas, variaveis) << endl;
    }



}
