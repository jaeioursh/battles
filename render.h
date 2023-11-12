#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include<iostream>
#include <chrono>
#include "robot.h"
#include "bullet.h"
#include "engine.h"
#include "lil.h"
#include "bots.h"

using namespace sf;
using namespace std;

class render{

    public:
    float dx;
    float dy;
    float scale;
    float map;
    int WIDTH,HEIGHT;
    float fps;
    int index;

    double thy;
    chrono::high_resolution_clock::time_point thyme;
    

    Sprite* background;
    RenderWindow* window;
    Image bg;
    Texture texture;
    Vector2f center;
    
    Sprite sprites[16];
    Texture tex;
    Image tiles;

    Text text;
	Font font;
    

    void draw_robot(robot&,Color);
    void draw_bullet(bullet&,Color);
    void draw_bg();
    bool draw(engine&);
    void show();
    void load_sprite();
    void size_sprite();
    render(float);
    ~render();
};

void render::load_sprite(){
    tiles.loadFromFile("robots.png");
    tex.loadFromImage(tiles);
    int x,y;
    for(int i=0;i<16;i++){
        sprites[i].setTexture(tex);
        x=i%4;
        y=i/4;
        sprites[i].setTextureRect(IntRect(x*16,y*16,16,16));
        sprites[i].setPosition(0, 0);
        //sprites[i].setOrigin(Vector2f(4,4));
        sprites[i].setOrigin(sf::Vector2f(sprites[i].getLocalBounds().width, sprites[i].getLocalBounds().height) / 2.f);

    }
    
}



render::render(float sz){
        map=sz;
        WIDTH=1000;
        HEIGHT=1000;
        center = Vector2f((float)WIDTH/2.0,(float)HEIGHT/2.0);
        dx=sz/2.0;
        dy=sz/2.0;
        scale=WIDTH/sz;
        fps=60;


        load_sprite();

        index=0;
        thy=0.1;
        thyme=chrono::high_resolution_clock::now();
        
        font.loadFromFile("tech.TTF");
        text.setFillColor(Color::Magenta);
        text.setFont(font); 
        text.setCharacterSize(80); 


		window= new RenderWindow(VideoMode(WIDTH, HEIGHT), "My window" ,Style::Default, ContextSettings(0, 0, 8));
        window->setFramerateLimit(fps);

        RenderTexture renderTexture;
	
	    renderTexture.create(WIDTH,HEIGHT);

	    renderTexture.clear(Color::Black);

	
		texture = renderTexture.getTexture();
		bg=texture.copyToImage();

		background =new Sprite(texture);

}


void render::draw_bg(){
    //window->clear();
    window->draw(*background);

}

void render::show(){
    window->display();
}

void render::draw_robot(robot& rob,Color col){
    float x,y,r;
    x=(rob.x-dx)*scale+center.x;
    y=(rob.y-dy)*scale+center.y;
    r=rob.rad*scale;
    if(x<-r || x>WIDTH+r || y<-r || y>HEIGHT+r)
        return;
    sprites[rob.index].setPosition(x,y);
    sprites[rob.index].setColor(col);
    sprites[rob.index].setScale(rob.rad*scale/8.,rob.rad*scale/8.);
    window->draw(sprites[rob.index]);
    

   
}

void render::draw_bullet(bullet& bul,Color col){

    float x,y,r;
    x=(bul.x-dx)*scale+center.x;
    y=(bul.y-dy)*scale+center.y;
    r=0.1*scale;
    if(x<-r || x>WIDTH+r || y<-r || y>HEIGHT+r)
        return;
    sprites[15].setPosition(x, y);
    sprites[15].setColor(col);
    sprites[15].setScale(scale/50.,scale/50.);
    sprites[15].setRotation(bul.t*180/M_PI-90);
    window->draw(sprites[15]);

}

bool render::draw(engine& eng){

    robot* rob;
    bullet* bul;
    int x,y,idx;

    lil<robot>* team;
    lil<robot>* dead;
    lil<bullet>* bullets;
    lil<bullet>* bmap;
    Color col;
    Color ded;
    
    Event event;
    float dd=200/scale;
    float SCALE=0.95;
    index+=1;
    while (window->pollEvent(event))
    {
        
        if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Q)){
            window->close();
            return 1;
        }
        
    }
    if(Keyboard::isKeyPressed(Keyboard::W))
        dy+=dd/fps;
    if(Keyboard::isKeyPressed(Keyboard::S))
        dy-=dd/fps;
    if(Keyboard::isKeyPressed(Keyboard::A))
        dx+=dd/fps;
    if(Keyboard::isKeyPressed(Keyboard::D))
        dx-=dd/fps;
    if(Keyboard::isKeyPressed(Keyboard::R)){
        scale*=SCALE;
    }
    if(Keyboard::isKeyPressed(Keyboard::F)){
        scale/=SCALE;
    }

    

    
    
    draw_bg();
    for(int q=0;q<2;q++){
        if(q==0){
            team = &(eng.team1); dead=&(eng.dead1); bullets=&(eng.bullets1); 
            col=Color(255,0,0);
            ded=Color(100,0,0);
        }
        else{
            team = &(eng.team2); dead=&(eng.dead2); bullets=&(eng.bullets2);
            col=Color(0,0,255);
            ded=Color(0,0,100);
        }
        dead->reset();
        for(int i=0;i<dead->len;i++){
            rob=dead->get();
            dead->inc();
            draw_robot(*rob,ded);
        }   
        team->reset();
        for(int i=0;i<team->len;i++){
            
            rob=team->get();
            team->inc();
            //cout<<team->len<<" "<<rob->model_len<<endl;
            draw_robot(*rob,col);
        }
        
        bullets->reset();
        for(int i=0;i<bullets->len;i++){
            bul=bullets->get();
            draw_bullet(*bul,col);
            bullets->inc();
        }
    }

    if(index%10==0){
        chrono::duration<double> diff = 
        chrono::high_resolution_clock::now()-thyme;
        
        thyme=chrono::high_resolution_clock::now();
        thy=10.0/diff.count();
    }
    text.setString("H: "+to_string(thy)+" "+to_string(bullets->len));
    window->draw(text);


    show();
    return 0;
}

render::~render(){
    delete background;
    delete window;
}

#endif