//g++ test.cpp -o s -lsfml-graphics -lsfml-window -lsfml-system
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
using namespace std;

typedef vector<int> vi;
typedef vector<vector<int> > vvi;

#define WINDOW_X 700
#define WINDOW_Y 700

struct Ant{
    int x, y;
    bool carregando;
    Ant(int a, int b){
        x = a, y = b;
        carregando = false;
    }
};

void draw(vvi &grid, vvi &grid_ant, sf::RenderWindow &window){

    int sx = grid.size();
    int sy = grid[0].size();

    sf::RectangleShape item(sf::Vector2f(WINDOW_X / (float) sx, WINDOW_X / (float) sx));

    for(int i = 0; i < sx; i++){
        for(int j = 0; j < sy; j++){

            if(grid[i][j] == 1){
                item.setFillColor(sf::Color(30,30,30));
            }else{
                item.setFillColor(sf::Color(255,255,255));
            }

            item.setPosition(WINDOW_X * i / (float)sx, WINDOW_Y * j / (float) sy);
            window.draw(item);
        }
    }
    window.display();
}

void initGrid(vvi &grid, vvi &grid_ant, vector<Ant> &ants, int tam_grid, int num_formigas, int num_itens){

    srand(time(NULL));

    grid = vvi(tam_grid, vi(tam_grid, 0));
    grid_ant = vvi(tam_grid, vi(tam_grid, 0));

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

        ants.push_back(Ant(x, y));
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "ACO");

    int tam_grid, num_formigas, num_itens, num_iteracoes, tam_raio;
    ifstream in("input.txt");

    cout << "Lendo dados de entrada..." << endl;
    in >> tam_grid >> num_formigas >> num_itens >> tam_raio >> num_iteracoes;
    cout << "Dados de entrada lidos." << endl;
    cout << "Tamanho da Grid:\t" << tam_grid << " x " << tam_grid << endl;
    cout << "Numero de formigas:\t" << num_formigas << endl;
    cout << "Numero de itens:\t" << num_itens << endl;
    cout << "Tamanho do raio:\t" << tam_raio << endl;
    cout << "Numero de iteracoes:\t" << num_iteracoes << endl;

    vvi grid;
    vvi grid_ant;
    vector<Ant> ants;

    initGrid(grid, grid_ant, ants, tam_grid, num_formigas, num_itens);

    while(num_iteracoes--){

        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        draw(grid, grid_ant, window);

    }
    window.close();

    return 0;
}
