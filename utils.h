

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include <cstdlib>
#include <time.h>

#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

void line_draw(sf::RenderWindow& window,sf::VertexArray vertices, const sf::Vector2f& point1, const sf::Vector2f& point2, float thickness){
    sf::Vector2f direction = point2 - point1;
    sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
    sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

    sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

    vertices[0].position = point1 + offset;
    vertices[1].position = point2 + offset;
    vertices[2].position = point2 - offset;
    vertices[3].position = point1 - offset;
    window.draw(vertices);
}



vector<vector<int>> read_csv(string filename) {
    vector<vector<int>> data;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<int> row;
        string value;

        while (getline(ss, value, ',')) {
            row.push_back(stoi(value));
        }

        data.push_back(row);
    }

    file.close();

    return data;
}


float unif(){
    return float(rand()%1000000000)/1000000000;
}

int randint(int val){
    return rand()%val;
}
