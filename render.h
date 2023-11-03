#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "robot.h"
#include "bullet.h"
#include "bots.h"

using namespace sf;

class render{

    public:
    float dx;
    float dy;
    float scale;
    float map;
    int WIDTH,HEIGHT;

    RenderWindow* window;

    void draw_robot(robot);
    void draw_bullet(bullet);
    render(float);
    
}

render::render(float sz){
        map=sz;
        WIDTH=300;
        HEIGHT=300;
        dx=WIDTH/2;
        dy=HEIGHT/2;
        scale=

        if (FULLSCREEN)
		window= new RenderWindow(VideoMode(WIDTH, HEIGHT), "My window", Style::Fullscreen);
}


void render::draw_robot(robot rob){

    float x1,x2,y1,y2,xt,yt;

    for(int i=0;i<rob.model_len/8;i++){
        for(int j=0;j<4;j++){
            x1=rob.model[i*8+j*2];
            y1=rob.model[i*8+j*2+1];
            if (j==3){
                x2=rob.model[i*8];
                y2=rob.model[i*8+1];
            }
            else{
                x2=rob.model[i*8+j*2+2];
                y2=rob.model[i*8+j*2+3];
            }
            xt=cos(t)*x1-sin(t)*y1 + rob.x;
            yt=sin(t)*x1+cos(t)*y1 + rob.y;
            x1=(xt+dx)*scale;
            y1=(yt+dy)*scale;

            xt=cos(t)*x2-sin(t)*y2 + rob.x;
            yt=sin(t)*x2+cos(t)*y2 + rob.y;
            x2=(xt+dx)*scale;
            y2=(yt+dy)*scale;

            line_draw(window, Vector2f(x1,y1), Vector2f(x2,y2), 4.0 * scale);

        }
    }

}

void render::draw_bullet(bullet bul){

    line_draw(window, Vector2f((bul.x_+dx)*scale,(bul.y_+dy)*scale), Vector2f((bul.x+dx)*scale,(bul.y+dy)*scale), 4.0 * scale);

}

void line_draw(sf::RenderWindow& window, const sf::Vector2f& point1, const sf::Vector2f& point2, float thickness){
    VertexArray vertices(Quads,4);
    Vector2f direction = point2 - point1;
    Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
    Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

    Vector2f offset = (thickness/2.f)*unitPerpendicular;

    vertices[0].position = point1 + offset;
    vertices[1].position = point2 + offset;
    vertices[2].position = point2 - offset;
    vertices[3].position = point1 - offset;
    window.draw(vertices);
}