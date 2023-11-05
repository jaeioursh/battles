#ifndef BOTS_H
#define BOTS_H


#include "robot.h"


class large:public robot{
    public:
        using robot::robot;
        void init() override;

};
void large::init(){
    health=100;
    speed=0.05;
    dmg=10;
    rps=5;
    clip=2;
    reload_delay=0.5;
    
    rad=0.25;
    float Model[]={-1.  , -1.  ,  1.  , -1.  ,  1.  , -0.5 , -1.  , -0.5 , -0.75,
       -0.5 , -0.75,  0.5 , -0.25,  0.5 , -0.25, -0.5 , -1.  ,  0.5 ,
        1.  ,  0.5 ,  1.  ,  1.  , -1.  ,  1.   };
    model_len=24;
    setup(Model);
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
    rps=10;
    clip=400;
    reload_delay=.2;
    
    rad=0.5;
    float Model[]={0.0, -1.0, 0.0, -0.8, 0.8, -0.8, 0.8,
     -1.0, 0.4, -0.8, 0.4, -0.6, 0.6, -0.6, 0.6, -0.8, 0.2, 
     -0.6, 0.2, -0.4, 1.0, -0.4, 1.0, -0.6, 0.0, -0.4, 0.0, 
     0.4, 0.6, 0.4, 0.6, -0.4, 0.2, 0.4, 0.2, 0.6, 1.0, 0.6, 
     1.0, 0.4, 0.4, 0.6, 0.4, 0.8, 0.6, 0.8, 0.6, 0.6, 0.0, 0.8, 
     0.0, 1.0, 0.8, 1.0, 0.8, 0.8, -1.0, -0.6, -1.0, 0.6, 0.0, 
     0.6, 0.0, -0.6};
    model_len=64;
    setup(Model);
}

#endif