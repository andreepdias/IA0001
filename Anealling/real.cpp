#include <bits/stdc++.h>
using namespace std;

class AlgoritmoGeneticoReal{

public:
    vector< vector<double> > populacao;
    int tamanhoPopulacao, tamanhoIndividuo, chanceMutacao, chanceCrossOver, geracao;
    double low, high;
    bool elitismo;

    vector<double> geraIndividuo(){
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(low, high);
        vector<double> individuo;
        for(int i = 0; i < tamanhoIndividuo; i++)
            individuo.push_back(dis(gen));
        return individuo;
    }

    void printIndividuo(vector<double>& individuo){
        for(double gene : individuo)
            cout << gene << " ";
        cout << endl;
    }

    void printPopulacao(){
        for(vector<double>& individuo : populacao)
            printIndividuo(individuo);
        cout << endl;
    }

    double func_objetivo(vector<double>& individuo){
        double firstSum = 0, secondSum, pi = acos(-1.0), n = individuo.size();
        for(double gene : individuo){
            firstSum += gene*gene;
            secondSum += cos(2*gene*pi);
        }
        return -20*exp(-0.2*sqrt(firstSum/n)) - exp(secondSum/n) + 20 + exp(1);
    }

    double func_penalidade(vector<double>& gene){
        return 0;
    }

    double fit(vector<double>& individuo){
        double val = func_objetivo(individuo);
        // printIndividuo(individuo);
        // cout << val << endl;
        return val;
    }

    void mutacao(vector<double>& individuo){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, 100);
        uniform_real_distribution<double> dis_random(low, high);
        // printIndividuo(individuo);
        for(double& gene : individuo)
            if(dis(gen) <= chanceMutacao){
                double delta = dis_random(gen);
                if(dis(gen) <= 50)
                    gene += delta;
                else
                    gene -= delta;
                gene = max(gene, low);
                gene = min(gene, high);
            }
        // printIndividuo(individuo);
    }


    double diversidade(){
        vector<double> centroide;
        for(int i = 0; i < tamanhoIndividuo; i++){
            double x = 0;
            for(int j = 0; j < tamanhoPopulacao; j++)
                x += populacao[j][i];
            x /= tamanhoPopulacao;
            centroide.push_back(x);
        }
        double dist = 0;

        for(int i = 0; i < tamanhoPopulacao; i++){
            for(int j = 0; j < tamanhoIndividuo; j++)
                dist += pow(centroide[j]-populacao[i][j], 2);
        }

        return dist;
    }


    int individuo_chance_roleta(int ignorar){
        double sumFit = 0;
        vector<double> valores_fitness;
        for(int i = 0; i < (int)populacao.size(); i++){
            if(i == ignorar){
                valores_fitness.push_back(0);
            }
            else{
                vector<double>& individuo = populacao[i];
                valores_fitness.push_back(fit(individuo));
                sumFit += valores_fitness.back();
            }
        }

        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dis(0, sumFit);

        double chance = dis(gen);
        int usado = 0;
        sumFit = 0;
        for(int i = 0; i < (int)valores_fitness.size(); i++){
            double val = valores_fitness[i];
            sumFit += val;
            if(chance <= sumFit)
                return i;
        }
    }


    pair< vector<double>,vector<double> > selecao_roleta(){
        int primeiroIndividuo = individuo_chance_roleta(-1);
        int segundoIndividuo  = individuo_chance_roleta(primeiroIndividuo);
        return make_pair(populacao[primeiroIndividuo], populacao[segundoIndividuo]);
    }

    int individuo_chance_torneio(int k, int ignorar){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, tamanhoPopulacao-1);
        map<int,int> usado;
        vector< pair<double,int> > vec;

        for(int i = 0; i < k; i++){
            int chance = dis(gen);
            while(usado[chance] || chance == ignorar) chance = dis(gen);
            double val_fit = fit(populacao[chance]);
            vec.push_back( make_pair( val_fit, chance) );
            usado[chance] = 1;
        }
        sort(vec.begin(), vec.end());
        return  vec.back().second;
    }

    pair< vector<double> , vector<double> > selecao_torneio(int k){
        int primeiroIndividuo = individuo_chance_torneio(k, -1);
        int segundoIndividuo = individuo_chance_torneio(k, primeiroIndividuo);
        return make_pair( populacao[primeiroIndividuo], populacao[segundoIndividuo] );
    }

    pair< vector<double>, vector<double> > crossover_blx(vector<double>& p1, vector<double>& p2){
        vector<double> primeiroFilho, segundoFilho;
        for(int i = 0; i < (int)p1.size(); i++){
            double d = fabs(p1[i]-p2[i]);
            double a = 0.5;
            double random_low = min(p1[i], p2[i])-a*d;
            random_low = max(low, random_low);
            double random_high = max(p1[i], p2[i])+a*d;
            random_high = min(high, random_high);
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(random_low, random_high);

            primeiroFilho.push_back(dis(gen));
            segundoFilho.push_back(dis(gen));
        }
        return {primeiroFilho, segundoFilho};
    }

    pair< vector<double>, vector<double> > crossover_uniform_average(vector<double>& p1, vector<double>& p2){
        vector<double> primeiroFilho = p1, segundoFilho = p2;
        for(int i = 0; i < (int)p1.size(); i++){
            double media = (p1[i]+p2[i])/2.0;
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0,1);
            if(dis(gen) <= 0.5) primeiroFilho[i] = media;
            else                segundoFilho[i] = media;
        }
        return {primeiroFilho, segundoFilho};
    }

    pair< vector<double>, vector<double> > crossover_arithmetic(vector<double>& p1, vector<double>& p2){
        vector<double> primeiroFilho = p1, segundoFilho = p2;
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0, 1);
        double a = dis(gen);
        for(int i = 0; i < (int)p1.size(); i++){
            primeiroFilho[i] = p1[i]*a + (1-a)*p2[i];
            segundoFilho[i]  = (1-a)*p1[i] + a*p2[i];
        }
        return {primeiroFilho, segundoFilho};
    }


    vector< pair<double, vector<double> > > findFitness(vector< vector<double> >& pop){
        vector< pair<double, vector<double> > > vec;
        for(vector<double>& individuo : pop)
            vec.push_back({fit(individuo), individuo});
        sort(vec.begin(), vec.end());
        return vec;
    }

    AlgoritmoGeneticoReal(int _tamanhoPopulacao, int _tamanhoIndividuo, bool _elitismo, int _chanceMutar, int _chanceCross, int _geracao, double _low, double _high){
        tamanhoPopulacao = _tamanhoPopulacao;
        tamanhoIndividuo = _tamanhoIndividuo;
        elitismo = _elitismo;
        chanceMutacao = _chanceMutar;
        chanceCrossOver = _chanceCross;
        geracao = _geracao;
        low = _low;
        high = _high;
        for(int i = 0; i < tamanhoPopulacao; i++)
            populacao.push_back(geraIndividuo());
        loopEvolutivo();
    }

    void loopEvolutivo(){

        vector<double> weiss;weiss.push_back(-10);
        weiss.push_back(-10);
        cout << fit(weiss) << endl;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, 100);
        double best = 0;
        vector<double> bestIndividuo;

        pair< vector<double> ,  vector<double> > pai = selecao_torneio(2);
        printIndividuo(pai.first);
        printIndividuo(pai.second);
        pair< vector<double> ,  vector<double> > filho = crossover_uniform_average(pai.first, pai.second);
        printIndividuo(filho.first);
        printIndividuo(filho.second);

        for(int i = 0; i < geracao; i++){
            // printPopulacao();
            vector< vector<double> > novaPopulacao;
            for(int i = 0; i*2 < populacao.size(); i++){
                pair< vector<double> ,  vector<double> > pai = selecao_torneio(2);

                pair< vector<double> ,  vector<double> > filho = pai;
                if(dis(gen) <= chanceCrossOver)
                    filho = crossover_blx(pai.first, pai.second);

                mutacao(filho.first);
                mutacao(filho.second);

                novaPopulacao.push_back(filho.first);
                novaPopulacao.push_back(filho.second);
            }

            vector< pair<double,  vector<double> > > fitness_old = findFitness(populacao);
            vector< pair<double,  vector<double> > > fitness_new = findFitness(novaPopulacao);

            if(elitismo){
                fitness_new.push_back(fitness_old.back());
                sort(fitness_new.begin(), fitness_new.end());
                for(int i = 1; i < (int)fitness_new.size(); i++)
                    populacao[i-1] = fitness_new[i].second;
            }
            else
                populacao = novaPopulacao;

            fitness_new = findFitness(populacao);

            if(fitness_new.back().first > best){
                best = fitness_new.back().first;
                bestIndividuo = fitness_new.back().second;
            }
            double bestPopulacao = fitness_new.back().first;
            double mediaPopulacao = 0;
            for(pair<double, vector<double> > par : fitness_new)
                mediaPopulacao += par.first;
            mediaPopulacao /= fitness_new.size();
            // printf("%d %.5f\n", i, diversidade());
            // printf("%d %.5f %.5f\n", i, bestPopulacao, mediaPopulacao);
        }
    }
};

int main(){
    int a, b, c, d, e, f, g, h; cin >> a >> b >> c >> d >> e >> f >> g >> h;
    AlgoritmoGeneticoReal algo(a, b, c, d, e, f, g, h);
    return 0;
}
