#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "bullet.h"
#include "utils.h"
#include "lil.h"
using namespace sf;

class robot{
    public:
    int team,rps;
    float health,hp_max,speed,dmg;
    float x,y,t;
    float size,lim;
    int clip,reload_delay,mag;
    int offset;
    bool dead;
    robot* lock_on;
    float *model;
    float *model_draw;
    int model_len;

    VertexArray pts;
    robot(int,float,float*,int, float, float, float, float, float, float, int,int,float,int);
    ~robot();
    void alt(){
        return;
    }
    bool can_shoot();
    void shoot(bullet*);
    void ai(float&, float&, lil<robot>);
    void move(float&, float&);

};



robot::robot(int Team,float Health,float* Model,int Model_len, float Dmg, float X, float Y, float T, float Size, float Speed, int Clip,int Rps,float Lim,int Rload):pts(Quads,4){
    
        team=Team;
        health=Health;
        hp_max=Health;
        speed=Speed;
        

        model_len=model_len;
        model=new float[model_len];
        model_draw=new float[model_len];
        for(int i=0;i<model_len;i++)
            model[i]=Model[i];
        
        
        dmg=Dmg;
        rps=rps;
        x=X;
        y=Y;
        t=T;
        size=Size;
        clip=Clip;
        mag=Clip;
        reload_delay=Rload;
        dead=0;
        lim=Lim;
        lock_on=NULL;
        offset=randint(60);

}

void robot::move(float& ang, float& vel){
  
    t+=ang;
    x+=vel*cos(t)*speed;
    y+=vel*sin(t)*speed;
    if (t>M_PI)
        t-=M_PI*2;
    if (t<-M_PI)
        t+=M_PI*2;
    if (y>lim-1)
        y=lim-1;
    if (y<0)
        y=0;
    if (x>lim-1)
        x=lim-1;
    if (x<0)
        x=0;
}
        
void robot::ai(float& ang, float& vel, lil<robot> enemies){
    float ang2,X,Y,diff;
    int idx;
    node* tmp;
    if (lock_on==NULL || lock_on.dead==1){
        idx = randint(enemies.len);
        tmp=enemies.head;
        for(int i=0;i<idx;i++)
            tmp=tmp->next;
        lock_on=tmp->elem;
    }
        

    X=lock_on->x;
    Y=lock_on->y;

    vel=1.0;
    

    ang2=atan2(Y-y,X-x);
    diff=-(t-ang2);
    
    if (diff>M_PI)
        diff-=M_PI*2;
    if (diff<-M_PI)
        diff+=M_PI*2;

    if (diff>0.1)
        diff=0.1;
    if (diff<-0.1)
        diff=-0.1;
}






bool robot::can_shoot(){
    clip--;
    if (clip<0){
        if (-clip==reload_delay)
            clip=mag;
        return 0;
    }
    else
        return 1; 


}

void robot::shoot(bullet* blt){

                            //size,speed
    blt->set(x,y,t,dmg,team,10.0,10.0);
}

robot::~robot(){
    delete[] model;
    delete[] model_draw;
}