//g++ test.cpp -o s -lsfml-graphics -lsfml-window -lsfml-system
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace std;

typedef vector<int> vi;
typedef vector<double> vd;
typedef vector<vector<int> > vvi;

#define WINDOW_SIZE 500

struct Resource{
    int color;
    vd features;
    Resource(vd &f, int c){
        features = f, color = c;
    }
};

typedef vector<vector<Resource*> > vvr;
typedef vector<Resource*> vr;

int dx[] = {-1, 0, 0, 1};
int dy[] = {0, -1, 1, 0};

struct Ant{
    int x, y, raio;
    bool carregando;
    Resource *bag;

    Ant(int a, int b, int r){
        x = a, y = b, raio = r;
        carregando = false;
        bag = NULL;
    }

    double sumItems(vvr &grid, double alpha, int dropOrTake, int fs){
        int a, b, sg = grid.size(), i, j, k;
        double sum = 0, d, s = 0;
        bool negative = false;

        for(i = -raio; i <= raio; i++){
            for(j = -raio; j <= raio; j++){
                if(i == 0 and j == 0)
                    continue;

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

                    s = 0;
                    for(k = 0; k < fs; k++){
                        if(dropOrTake == 1){
                            s += pow(bag -> features[k] - grid[a][b] -> features[k], 2);
                        }else if(dropOrTake == 2){
                            s += pow(grid[x][y] -> features[k] - grid[a][b] -> features[k], 2);
                        }
                    }
                    d = sqrt(s);
                    d = (1.0 - (d / alpha));

                    if(d > 0.0){
                        sum += d;
                    }else{
                        negative = true;
                        break;
                    }
                }
            }
            if(negative){
                break;
            }
        }
        if(!negative){
            return sum;
        }
        return 0.0;
    }

    void move(int n){
        int r = rand() % 4;

        x += dx[r];
        y += dy[r];

        if(x < 0)
            x = n - 1;
        else if(x >= n)
            x = 0;

        if(y < 0)
            y = n - 1;
        else if(y >= n)
            y = 0;
    }

    void drop(vvr &grid, double sigma, double alpha){

        if(grid[x][y] != NULL)
            return;

        double prob_drop, prob_rand;

        prob_drop = sumItems(grid, alpha, 1, bag -> features.size());
        prob_drop /= (sigma * sigma);

        if(prob_drop < 0.0)
            prob_drop = 0.0;

        if(prob_drop >= 1.0)
            prob_drop = 1.0;
        else
            prob_drop = pow(prob_drop, 4);

        prob_rand = (double) rand() / RAND_MAX;

        if(prob_rand <= prob_drop){
            grid[x][y] = bag;
            bag = NULL;
            carregando = false;
        }
    }

    void pick(vvr &grid, double sigma, double alpha){

        if(grid[x][y] == NULL)
            return;

        double prob_pick, prob_rand;

        prob_pick = sumItems(grid, alpha, 2, grid[x][y] -> features.size());
        prob_pick /= (sigma * sigma);

        if(prob_pick < 0.0)
            prob_pick = 0.0;

        if(prob_pick <= 1.0)
            prob_pick = 1.0;
        else
            prob_pick = (1.0 / (prob_pick * prob_pick) );


        prob_rand = (double) rand() / RAND_MAX;

        if(prob_rand <= prob_pick){
            bag = grid[x][y];
            grid[x][y] = NULL;
            carregando = true;
        }
    }

    void forceDrop(vvr &grid, double sigma, double alpha){
        int k = 0, gs = grid.size();

        while(carregando and k <= 1000000){
            move(gs);
            drop(grid, sigma, alpha);
            k++;
        }
        if(carregando){
            grid[x][y] = bag;
            bag = NULL;
            carregando = false;
        }

    }
};

void draw(vvr &grid, sf::RenderWindow &window){

    int sx = grid.size();
    int sy = grid[0].size();

    sf::RectangleShape item(sf::Vector2f(WINDOW_SIZE / (double) sx, WINDOW_SIZE / (double) sx));

    for(int i = 0; i < sx; i++){
        for(int j = 0; j < sy; j++){

            if(grid[i][j] != NULL){
                switch(grid[i][j]->color){
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

            item.setPosition(WINDOW_SIZE * i / (double)sx, WINDOW_SIZE * j / (double) sy);
            window.draw(item);
        }
    }
    window.display();
}

void initGrid(vvr &grid, vector<Ant> &ants, int tam_grid, int num_formigas, int tam_raio){

    srand(time(NULL));

    grid = vvr(tam_grid, vr(tam_grid, NULL));

    ifstream dataset("dataset4.txt");
    int x, y, c;
    double a, b;
    char l;

    while(dataset >> a >> l >> b >> l >> c){
        do{
            x = rand() % tam_grid;
            y = rand() % tam_grid;
        }while(grid[x][y] != NULL);

        vd f;
        f.push_back(a);
        f.push_back(b);

        grid[x][y] = new Resource(f, c);
    }

    for(int i = 0; i < num_formigas; i++){
        int x = rand() % tam_grid;
        int y = rand() % tam_grid;

        ants.push_back(Ant(x, y, tam_raio));
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "ACO");

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
    vector<Ant> ants;

    initGrid(grid, ants, tam_grid, num_formigas, tam_raio);

    int k = 0, kk = 0;
    while(num_iteracoes--){

        for(int i = 0; i < num_formigas; i++){
            ants[i].move(tam_grid);

            if(ants[i].carregando){
                ants[i].drop(grid, sigma, alpha);
            }else{
                ants[i].pick(grid, sigma, alpha);
            }
        }

        k++;
        if(k == 250000){
            sf::Event event;
            while(window.pollEvent(event)){
                if(event.type == sf::Event::Closed)
                window.close();
            }
            draw(grid, window);

            kk += k;
            k = 0;
            cout << "Numero de iteracoes: " << kk << endl;
        }
    }

    for(int i = 0; i < num_formigas; i++){
        ants[i].forceDrop(grid, sigma, alpha);
    }
    cout << "Formigas paradas." << endl;
    draw(grid, window);

    bool pressed = false;
    while(pressed == false){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        	pressed = true;
        }
    }
    window.close();

    return 0;
}
