#ifndef RENDER_H
#define RENDER_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include<iostream>
#include "robot.h"
#include "bullet.h"
#include "engine.h"
#include "lil.h"
#include "bots.h"

using namespace sf;

class render{

    public:
    float dx;
    float dy;
    float scale;
    float map;
    int WIDTH,HEIGHT;
    float fps;

    Sprite* background;
    RenderWindow* window;
    Image bg;
    Texture texture;
    Vector2f center;
    

    void draw_robot(robot&,Color);
    void draw_bullet(bullet&,Color);
    void draw_bg();
    bool draw(engine&);
    void show();
    void line_draw(RenderWindow*,const Vector2f&,const Vector2f& ,Color);
    render(float);
    ~render();
};

void render::line_draw(RenderWindow*window, const Vector2f& point1, const Vector2f& point2, Color col){
    float thickness=0.02;
    VertexArray vertices(Quads,4);

    
    Vector2f delta = Vector2f(dx,dy);

    Vector2f direction = point2 - point1;
    Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
    Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

    Vector2f offset = (thickness/2.f)*unitPerpendicular;

    vertices[0].position = point1 + offset;
    vertices[1].position = point2 + offset;
    vertices[2].position = point2 - offset;
    vertices[3].position = point1 - offset;
    for (int i = 0; i < 4; ++i){
        vertices[i].position-=delta;
        vertices[i].position*=scale;
        //vertices[i].position+=scale*delta;
        vertices[i].position+=center;
        vertices[i].color = col;
    }
        
    
    window->draw(vertices);
    
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

    float x1,x2,y1,y2,xt,yt;
    //cout<<rob.model_len<<endl;
    for(int i=0;i<rob.model_len/8;i++){
        for(int j=0;j<4;j++){
            
            x1=rob.model[i*8+j*2]*rob.rad;
            y1=rob.model[i*8+j*2+1]*rob.rad;
            if (j==3){
                x2=rob.model[i*8]*rob.rad;
                y2=rob.model[i*8+1]*rob.rad;
            }
            else{
                x2=rob.model[i*8+j*2+2]*rob.rad;
                y2=rob.model[i*8+j*2+3]*rob.rad;
            }
            xt=cos(rob.t)*x1-sin(rob.t)*y1 + rob.x;
            yt=sin(rob.t)*x1+cos(rob.t)*y1 + rob.y;
            x1=xt;
            y1=yt;

            xt=cos(rob.t)*x2-sin(rob.t)*y2 + rob.x;
            yt=sin(rob.t)*x2+cos(rob.t)*y2 + rob.y;
            x2=xt;
            y2=yt;
            
            line_draw(window, Vector2f(x1,y1), Vector2f(x2,y2),col);

        }
    }

}

void render::draw_bullet(bullet& bul,Color col){

    line_draw(window, Vector2f(bul.x-cos(bul.t)*.25,bul.y-sin(bul.t)*0.25), Vector2f(bul.x,bul.y),col);

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
        team->reset();
        for(int i=0;i<team->len;i++){
            
            rob=team->get();
            team->inc();
            //cout<<team->len<<" "<<rob->model_len<<endl;
            draw_robot(*rob,col);
        }
        dead->reset();
        for(int i=0;i<dead->len;i++){
            rob=dead->get();
            dead->inc();
            draw_robot(*rob,ded);
        }
        bullets->reset();
        for(int i=0;i<bullets->len;i++){
            bul=bullets->get();
            draw_bullet(*bul,col);
            bullets->inc();
        }
    }
    show();
    return 0;
}

render::~render(){
    delete background;
    delete window;
}

#endif