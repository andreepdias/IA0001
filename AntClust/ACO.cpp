//g++ test.cpp -o s -lsfml-graphics -lsfml-window -lsfml-system
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace std;

typedef vector<int> vi;
typedef vector<vector<int> > vvi;

#define WINDOW_SIZE 700

int dx[] = {-1, 0, 0, 1};
int dy[] = {0, -1, 1, 0};

struct Ant{
    int x, y, raio;
    bool carregando;
    Ant(int a, int b, int r){
        x = a, y = b, raio = r;
        carregando = false;
    }

    int countItens(vvi &grid){
        int a, b, sg = grid.size();
        int cont = 0;

        for(int i = -raio; i <= raio; i++){
            for(int j = -raio; j <= raio; j++){
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

                if(grid[a][b] == 1){
                    cont++;
                }
            }
        }
        return cont;
    }

    void move(int sg){
        int r = rand() % 4;

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
    }

    void drop(vvi &grid){

        if(grid[x][y] == 1)
            return;

        int num_casas, num_itens;
        double prob_drop, prob_rand;

        num_casas = (2 * raio + 1) * (2 * raio + 1) - 1;
        num_itens = countItens(grid);

        prob_drop = (num_itens * 1.0 / num_casas);
        prob_drop *= prob_drop;
        prob_drop -= 0.0025;

        prob_rand = (double) rand() / RAND_MAX;

        if(prob_rand <= prob_drop){
            grid[x][y] = 1;
            carregando = false;
        }
    }

    void pick(vvi &grid){

        if(grid[x][y] == 0)
            return;

        int num_casas, num_itens;
        double prob_pick, prob_rand;

        num_casas = (2 * raio + 1) * (2 * raio + 1) - 1;
        num_itens = countItens(grid);

        prob_pick =  1 - (num_itens * 1.0 / num_casas);
        prob_pick *= prob_pick;
        prob_pick += 0.0025;

        prob_rand = (double) rand() / RAND_MAX;

        if(prob_rand <= prob_pick){
            grid[x][y] = 0;
            carregando = true;
        }
    }

    void forceDrop(vvi &grid){
        int k = 0, sg = grid.size();

        while(carregando and k <= 1000000){
            move(sg);
            drop(grid);
            k++;
        }
        if(carregando){
            carregando = false;
            grid[x][y] = 1;
        }
    }
};

void draw(vvi &grid, sf::RenderWindow &window){

    int sx = grid.size();
    int sy = grid[0].size();

    sf::RectangleShape item(sf::Vector2f(WINDOW_SIZE / (double) sx, WINDOW_SIZE / (double) sx));

    for(int i = 0; i < sx; i++){
        for(int j = 0; j < sy; j++){

            if(grid[i][j] == 1){
                item.setFillColor(sf::Color(30,30,30));
            }else{
                item.setFillColor(sf::Color(255,255,255));
            }

            item.setPosition(WINDOW_SIZE * i / (double)sx, WINDOW_SIZE * j / (double) sy);
            window.draw(item);
        }
    }
    window.display();
}

void initGrid(vvi &grid, vector<Ant> &ants, int tam_grid, int num_formigas, int num_itens, int tam_raio){

    srand(time(NULL));

    grid = vvi(tam_grid, vi(tam_grid, 0));

    int x, y;

    for(int i = 0; i < num_itens; i++){
        do{
            x = rand() % tam_grid;
            y = rand() % tam_grid;
        }while(grid[x][y] != 0);

        grid[x][y] = 1;
    }

    for(int i = 0; i < num_formigas; i++){
        int x = rand() % tam_grid;
        int y = rand() % tam_grid;

        ants.push_back(Ant(x, y, tam_raio));
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "ACO");

    int tam_grid, num_formigas, num_itens, num_iteracoes, tam_raio;
    ifstream in("input1.txt");

    cout << "Lendo dados de entrada..." << endl;
    in >> tam_grid >> num_formigas >> num_itens >> tam_raio >> num_iteracoes;
    cout << "Dados de entrada lidos." << endl;
    cout << "Tamanho da Grid:\t" << tam_grid << " x " << tam_grid << endl;
    cout << "Numero de formigas:\t" << num_formigas << endl;
    cout << "Numero de itens:\t" << num_itens << endl;
    cout << "Tamanho do raio:\t" << tam_raio << endl;
    cout << "Numero de iteracoes:\t" << num_iteracoes << endl;

    vvi grid;
    vector<Ant> ants;

    initGrid(grid, ants, tam_grid, num_formigas, num_itens, tam_raio);

    int sa = ants.size();

    int k = 0, kk = 0;
    while(num_iteracoes--){

        for(int i = 0; i < sa; i++){
            ants[i].move(tam_grid);

            if(ants[i].carregando){
                ants[i].drop(grid);
            }else{
                ants[i].pick(grid);
            }
        }


        k++;
        if(k == 10000){
            sf::Event event;
            while(window.pollEvent(event)){
                if(event.type == sf::Event::Closed)
                window.close();
            }
            kk += k;
            k = 0;
            cout << "Numero de iteracoes: " << kk << endl;
            draw(grid, window);
        }
    }

    for(int i = 0; i < num_formigas; i++){
        ants[i].forceDrop(grid);
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
