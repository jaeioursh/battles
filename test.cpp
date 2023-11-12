#include <SFML/Graphics.hpp>

#include "lil.h"
#include "utils.h"
#include "bullet.h"
#include "bots.h"
#include "render.h"
#include <cstdlib>
#include <time.h>
#include<iostream>

using namespace std;

void test_csv(){
    vector<vector<int>> a;
    a=read_csv("teams/e1.csv");
    
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout<<a[i][j]<<' ';
        }
        cout<<endl;
    }

}

void test_lil(){
    cout<<"beggining lil test"<<endl;
    lil<int> lst;


    int A=5;
    int B=6;
    int C=7;


    
    lst.add(&A);
    lst.add(&B);
    lst.add(&C);
    

    lst.clear();

    lst.add(&A);
    lst.add(&B);
    lst.add(&C);
    


    for(int i=0;i<3;i++){
        cout<<*lst.get()<<" "<< lst.len << endl;
        lst.inc();
    }
    lst.reset();
    lst.add(&A);
    int len=lst.len;
    for(int i=0;i<len;i++){
        cout<<*lst.get()<<" bef "<< lst.len << endl;
        lst.remove();
        //cout<<*lst.get()<<" aft "<< lst.len << endl;
        //lst.inc();
    }
    return;
}

void test_sfml(){
    


    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

}

void test_rend(){
    srand(time(NULL));
    int sz=50;
    int rows=1;
    render rend(sz);
    engine eng(sz);
    for(int x=0;x<sz;x++){
        for(int y=0;y<rows;y++){
            eng.add_bot((float) x, (float) y,1.55,3,1);
            eng.add_bot((float) x, (float) (sz-y-1),-1.55,3,0);
        }
    }
    
    cout<<eng.team1.len<<endl;
    cout<<eng.team2.len<<endl;
    while(1){
        eng.step(1/rend.fps);
        //if(eng.idx%50==0)
        //    cout<<eng.idx<<" "<<eng.team2.len<<" "<<eng.team1.len<<endl;
        if(rend.draw(eng))
            return;
    }

}

int main(){
    //test_lil();
    //test_csv();
    test_rend();
    return 0; 
}

