#ifndef BOTS_H
#define BOTS_H


#include "robot.h"
#include<iostream>

class medium:public robot{
    public:
        using robot::robot;
        void init() override;

};

void medium::init(){
    health=100;
    speed=0.05;
    dmg=10;
    rps=1;
    clip=1;
    reload_delay=0.1;
    
    rad=0.25;
    float Model[]={-0.6667, -1.0, 0.3333, -1.0,
     0.3333, -0.3333, -0.6667, -0.3333, -1.0, 
     -0.3333, 1.0, -0.3333, 1.0, 0.3333, -1.0,
      0.3333, -0.6667, 0.3333, 0.3333, 0.3333,
       0.3333, 1.0, -0.6667, 1.0};
    float angs[]={0.0};
    model_len=24;
    setup(Model,angs);
}



class large:public robot{
    public:
        using robot::robot;
        void init() override;

};
void large::init(){
    health=100;
    speed=0.05;
    dmg=10;
    rps=0.75;
    clip=2;
    reload_delay=2;
    
    rad=0.25;
    float Model[]={-1.  , -1.  ,  1.  , -1.  ,  1.  , -0.5 , -1.  , -0.5 , -0.75,
       -0.5 , -0.75,  0.5 , -0.25,  0.5 , -0.25, -0.5 , -1.  ,  0.5 ,
        1.  ,  0.5 ,  1.  ,  1.  , -1.  ,  1.   };
    float angs[]={M_PI/4.0,-M_PI/4.0};
    model_len=24;
    setup(Model,angs);
}


class turret:public robot{
    public:
        using robot::robot;
        void init() override;

};

void turret::init(){
    health=100;
    speed=0.00;
    dmg=10;
    rps=100;
    clip=1;
    reload_delay=2;
    
    rad=0.25;
    float Model[]={-1.0, -1.0, -1.0, 1.0, 
    -0.5, 1.0, -0.5, -1.0, -0.5, -0.5,
     -0.5, 0.5, 1.0, 0.5, 1.0, -0.5};
    float angs[]={0.0};
    model_len=16;
    setup(Model,angs);
}


class replicator:public robot{
    public:
        int rep_timer;
        int rep_time;
        int spawn_left;
        int version;

        using robot::robot;
        void init() override;
        robot* alt(float) override;

};

void replicator::init(){
    health=100;
    speed=0.05;
    dmg=10;
    rps=100;
    clip=1;
    reload_delay=1;

    rep_time=3;
    spawn_left=2;
    version=4;

    rep_timer=0;

    
    rad=0.25;
    float Model[]={-1.0, -1.0, -1.0, 1.0,
     -0.3333, 1.0, -0.3333, -1.0, -0.3333,
      -0.3333, 0.3333, -0.3333, 0.3333, 0.3333,
       -0.3333, 0.3333, 0.3333, -1.0, 0.3333,
        -0.3333, 1.0, -0.3333, 1.0, -1.0, 0.3333, 
        0.3333, 0.3333, 1.0, 1.0, 1.0, 1.0, 0.3333};
    float angs[]={0.0};
    model_len=32;
    setup(Model,angs);
}

robot* replicator::alt(float dt){
    cout<<version;
    replicator* rob=NULL;
    float theta=0;
    rep_timer+=dt;
    if (rep_timer>rep_time && spawn_left>0 && version>0){
        spawn_left--;
        theta=(unif()*2-1)*M_PI;
        rob=new replicator(x+cos(theta)*rad,y+sin(theta)*rad,theta,lim);
        rob->init();
        rob->version=version-1;
        rep_timer=0;
    }
    return rob;
}


class uber:public robot{
    public:
        using robot::robot;
        void init() override;

};
void uber::init(){
    health=1000;
    speed=0.05;
    dmg=100;
    rps=2;
    clip=4;
    reload_delay=3;
    
    rad=0.5;
    float Model[]={0.0, -1.0, 0.0, -0.8, 0.8, -0.8, 0.8,
     -1.0, 0.4, -0.8, 0.4, -0.6, 0.6, -0.6, 0.6, -0.8, 0.2, 
     -0.6, 0.2, -0.4, 1.0, -0.4, 1.0, -0.6, 0.0, -0.4, 0.0, 
     0.4, 0.6, 0.4, 0.6, -0.4, 0.2, 0.4, 0.2, 0.6, 1.0, 0.6, 
     1.0, 0.4, 0.4, 0.6, 0.4, 0.8, 0.6, 0.8, 0.6, 0.6, 0.0, 0.8, 
     0.0, 1.0, 0.8, 1.0, 0.8, 0.8, -1.0, -0.6, -1.0, 0.6, 0.0, 
     0.6, 0.0, -0.6};
    float angs[]={M_PI/4*1.1,-M_PI/8*1.1,M_PI/8*1.1,-M_PI/4*1.1};
    model_len=64;
    setup(Model,angs);
}

#endif