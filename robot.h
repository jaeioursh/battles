#ifndef ROBOT_H
#define ROBOT_H

#include <cmath>

#include "bullet.h"
#include "utils.h"
#include "lil.h"
using namespace sf;

class robot{
    public:

    int team;
    float health,hp_max,speed,dmg;
    float x,y,t;
    float lim;
    float rad;
    int clip,mag;
    float rps,reload_delay;
    float ftime ,rtime ; //stats
    int offset;
    bool dead;
    robot* lock_on;         //target
    float *model;           //values for model shape
    int model_len;

    
    explicit robot(float,float, float, int);
    virtual void init();
    void setup(float[]);
    ~robot();
    void alt(){
        return;
    }
    bool can_shoot(float);
    bullet* shoot();
    void ai(lil<robot> & );
    void move(float);

};




robot::robot(float X, float Y, float T, int Lim){
    
        team=0;
        x=X+0.5;
        y=Y+0.5;
        t=T;
        lim=(float)Lim;

        dead=0;
    
        lock_on=NULL;
        offset=randint(60);

}



void robot::init(){
    health=100;
    speed=0.1;
    dmg=10;
    rps=5;
    clip=4;
    reload_delay=2;
    
    float Model[]={-1.  , -1.  , -1.  ,  1. };
    setup(Model);
}

void robot::setup(float Model[]){
    hp_max=health;
    mag=clip;
    ftime=unif()/rps;
    rtime=unif()*reload_delay;
    model=new float[model_len];
    for(int i=0;i<model_len;i++)
        model[i]=Model[i];
}

void robot::move(float dt){
    float X,Y,ang,diff;
    if (lock_on != NULL){
        X=lock_on->x;
        Y=lock_on->y;

        

        ang=atan2(Y-y,X-x);
        diff=-(t-ang);
        
        if (diff>M_PI)
            diff-=M_PI*2;
        if (diff<-M_PI)
            diff+=M_PI*2;

        if (diff>0.1)
            diff=0.1;
        if (diff<-0.1)
            diff=-0.1;


        t+=diff*speed*dt*50;
        x+=cos(t)*speed*dt*5;
        y+=sin(t)*speed*dt*5;
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
}
        
void robot::ai(lil<robot> & enemies){
    float ang2,X,Y,diff;
    int idx;
    if(lock_on != NULL && lock_on->health<=0)
        lock_on=NULL;


    if (lock_on==NULL && enemies.len>0){
        idx = randint(enemies.len);

        enemies.reset();
        for(int i=0;i<idx;i++)
            enemies.inc();
        lock_on=enemies.get();
    }
        

    
}

bool robot::can_shoot(float dt){
    if(rtime>reload_delay)
        if (mag==0)
            mag=clip;
    ftime+=dt;
    rtime+=dt;
    if(ftime>1/rps && mag>0 && lock_on != NULL)
        return 1;
    return 0;


}

bullet* robot::shoot(){
    mag--;
    ftime=0;
    if (mag==0)
        rtime=0;
    bullet* blt=new bullet();                            //size,speed
    blt->set(x,y,t,dmg,10.0,10.0);
    return blt;
}

robot::~robot(){
    delete[] model;
}


#endif