

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include <cstdlib>
#include <time.h>

#include <fstream>
#include <sstream>
#include <vector>

using namespace std;




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
