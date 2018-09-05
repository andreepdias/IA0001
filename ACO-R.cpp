//g++ test.cpp -o s -lsfml-graphics -lsfml-window -lsfml-system
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace std;

typedef vector<int> vi;
typedef vector<vector<int> > vvi;

#define WINDOW_X 700
#define WINDOW_Y 700

int dx[] = {-1, 0, 0, 1};
int dy[] = {0, -1, 1, 0};

struct Resource{
    int x, y, cor;
    Resource(int a, int b, int c){
        x = a, y = b, cor = c;
    }
};

typedef vector<vector<Resource*> > vvr;
typedef vector<Resource*> vr;

struct Ant{
    int x, y, raio;
    bool carregando;
    Resource *bag;

    Ant(int a, int b, int r){
        x = a, y = b, raio = r;
        carregando = false;
        bag = NULL;
    }

    double sumItems(vvr &grid, double alpha, int dropOrTake){
        int a, b, sg = grid.size();
        double sum = 0, d;

        for(int i = -raio; i <= raio; i++){
            for(int j = -raio; j <= raio; j++){
                a = x + i;
                b = y + j;

                if(a < 0)
                    a = sg - 1;
                else if(a >= sg)
                    a = 0;

                if(b < 0)
                    b = sg - 1;
                else if(b >= sg)
                    b = 0;

                if(grid[a][b] != NULL){

                    if(dropOrTake == 1){
                        d = sqrt( (pow(grid[a][b] -> x - bag -> x, 2)) + (pow(grid[a][b] -> y - bag -> y, 2) ) ) ;
                    }else if(dropOrTake == 2){
                        d = sqrt( (pow(grid[x][y] -> x - grid[a][b] -> x, 2)) + (pow(grid[x][y] -> y - grid[a][b] -> y, 2) ) );
                    }
                    d = d * 1.0 / alpha;
                    sum += (1.0 - d);
                }
            }
        }
        return sum;
    }

    void move(vvi &grid_ant){

        int sg = grid_ant.size();
        int r = rand() % 4;

        grid_ant[x][y]--;

        x += dx[r];
        y += dy[r];

        if(x < 0)
            x = sg -1;
        else if(x >= sg)
            x = 0;

        if(y < 0)
            y = sg - 1;
        else if(y >= sg)
            y = 0;

        grid_ant[x][y]++;
    }

    void drop(vvr &grid, double sigma, double alpha){

        if(grid[x][y] != NULL)
            return;

        double prob_drop, prob_rand;

        prob_drop = sumItems(grid, alpha, 1);
        prob_drop /= sigma * sigma;

        if(prob_drop >= 1.0)
            prob_drop = 1.0;
        else
            prob_drop = pow(prob_drop, 4);

        prob_rand = (double) rand() / RAND_MAX;

        if(prob_rand <= prob_drop){
            carregando = false;
            grid[x][y] = bag;
            bag = NULL;
        }
    }

    void pick(vvr &grid, double sigma, double alpha){

        if(grid[x][y] == NULL)
            return;

        double prob_pick, prob_rand;

        prob_pick = sumItems(grid, alpha, 2);
        prob_pick /= sigma * sigma;

        if(prob_pick <= 1.0)
            prob_pick = 1.0;
        else
            prob_pick = (1 / (prob_pick * prob_pick) );


        prob_rand = (double) rand() / RAND_MAX;

        if(prob_rand <= prob_pick){
            carregando = true;
            bag = grid[x][y];
            grid[x][y] = NULL;
        }
    }
};

void draw(vvr &grid, vvi &grid_ant, sf::RenderWindow &window){

    int sx = grid.size();
    int sy = grid[0].size();

    sf::RectangleShape item(sf::Vector2f(WINDOW_X / (double) sx, WINDOW_X / (double) sx));

    for(int i = 0; i < sx; i++){
        for(int j = 0; j < sy; j++){

            if(grid[i][j] != NULL){
                switch(grid[i][j]->cor){
                    case 1:
                        item.setFillColor(sf::Color(255,20,147));
                        break;
                    case 2:
                        item.setFillColor(sf::Color(0,206,209));
                        break;
                    case 3:
                        item.setFillColor(sf::Color(60,179,113));
                        break;
                    case 4:
                        item.setFillColor(sf::Color(255,215,0));
                        break;
                }
            }else{
                item.setFillColor(sf::Color(255,255,255));
            }

            item.setPosition(WINDOW_X * i / (double)sx, WINDOW_Y * j / (double) sy);
            window.draw(item);
        }
    }
    window.display();
}

void initGrid(vvr &grid, vvi &grid_ant, vector<Ant> &ants, int tam_grid, int num_formigas, int tam_raio){

    srand(time(NULL));

    grid = vvr(tam_grid, vr(tam_grid, NULL));
    grid_ant = vvi(tam_grid, vi(tam_grid, 0));

    ifstream dataset("dataset2.txt");
    int x, y, c;
    double a, b;
    char l;

    while(dataset >> a >> l >> b >> l >> c){
        do{
            x = rand() % tam_grid;
            y = rand() % tam_grid;
        }while(grid[x][y] != NULL);

        grid[x][y] = new Resource(x, y, c);
    }

    for(int i = 0; i < num_formigas; i++){
        int x = rand() % tam_grid;
        int y = rand() % tam_grid;

        ants.push_back(Ant(x, y, tam_raio));
        grid_ant[x][y]++;
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "ACO");

    int tam_grid, num_formigas, num_iteracoes, tam_raio;
    double sigma, alpha;
    ifstream in("input.txt");

    cout << "Lendo dados de entrada..." << endl;
    in >> tam_grid >> num_formigas >> tam_raio >> num_iteracoes >> sigma >> alpha;
    cout << "Dados de entrada lidos." << endl;
    cout << "Tamanho da Grid:\t" << tam_grid << " x " << tam_grid << endl;
    cout << "Numero de formigas:\t" << num_formigas << endl;
    cout << "Tamanho do raio:\t" << tam_raio << endl;
    cout << "Numero de iteracoes:\t" << num_iteracoes << endl << endl;
    cout << "Sigma:\t" << sigma << endl;
    cout << "Alpha:\t" << alpha << endl;

    vvr grid;
    vvi grid_ant;
    vector<Ant> ants;

    initGrid(grid, grid_ant, ants, tam_grid, num_formigas, tam_raio);

    int sa = ants.size();

    int k = 0;
    while(num_iteracoes--){

        for(int i = 0; i < sa; i++){
            ants[i].move(grid_ant);

            if(ants[i].carregando){
                ants[i].drop(grid, sigma, alpha);
            }else{
                ants[i].pick(grid, sigma, alpha);
            }
        }

        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        k++;
        if(k == 25000){
            draw(grid, grid_ant, window);
            k = 0;
        }
    }
    window.close();

    return 0;
}
