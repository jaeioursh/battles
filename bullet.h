#include <cmath>

#include "utils.h"
using namespace sf;

class bullet{
    public:
        float x,y,t,x_,y_,dmg,size,speed,hit;
        int team;

        bullet();
        void set(float,float,float,float,int,float,float);
        float dist(float,float);
        void step();
        void draw(RenderWindow&,float,float,float);

};



bullet::bullet(){

}




void bullet::set(float X,float Y,float T,float Dmg,int Team,float Size,float Speed){
    x=X;
    y=Y;
    x_=X;
    y_=Y;
    t=T;
    dmg=Dmg;
    team=Team;
    size=Size;
    speed=Speed;
    hit=0;


}

float bullet::dist(float X, float Y){
    float x1,x2,x3,y1,y2,y3,norm,px,py,u,xx,yy,dx,dy,d;

    x3=X;
    y3=Y;
    x1=x;
    x2=x_;
    y1=y;
    y2=y_;

 
    px = x2-x1;
    py = y2-y1;

    norm = px*px + py*py;
    u =  ((x3 - x1) * px + (y3 - y1) * py) / norm;

    if (u > 1)
        u = 1;
    else if (u < 0)
        u = 0;

    xx = x1 + u * px;
    yy = y1 + u * py;

    dx = xx - x3;
    dy = yy - y3;

    d = sqrt(dx*dx + dy*dy);

    return d;

}

void bullet::step(){
    x_=x;
    y_=y;
    x=x_+cos(t)*speed;
    y=y_+sin(t)*speed;
}


