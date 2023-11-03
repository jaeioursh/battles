#include <cmath>

#include "bullet.h"
#include "utils.h"
#include "lil.h"
using namespace sf;

class robot{
    public:

    int team,rps;
    float health,hp_max,speed,dmg;
    float x,y,t;
    float lim;
    int clip,reload_delay,mag;  //stats
    int offset;
    bool dead;
    robot* lock_on;         //target
    float *model;           //values for model shape
    int model_len;

    
    robot(float,float, float, int, float);
    virtual void init();
    void setup(float[]);
    ~robot();
    void alt(){
        return;
    }
    bool can_shoot();
    void shoot(bullet*);
    void ai(float&, float&, lil<robot>);
    void move(float&, float&);

};




robot::robot(float X, float Y, float T, int Team, float Lim){
    
        team=Team;
        x=X;
        y=Y;
        t=T;
        lim=Lim;

        dead=0;
    
        lock_on=NULL;
        offset=randint(60);
        init();
        

}



void robot::init(){
    health=100;
    speed=0.1;
    dmg=10;
    rps=5;
    clip=4;
    reload_delay=2;
    
    float Model[]={-1.  , -1.  , -1.  ,  1. };
    model_len=4;
    setup(Model);
}

void robot::setup(float Model[]){
    hp_max=health;
    mag=clip;
    model=new float[model_len];
    for(int i=0;i<model_len;i++)
        model[i]=Model[i];
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
    
    if (lock_on==NULL || lock_on->dead==1){
        idx = randint(enemies.len);

        enemies.reset();
        for(int i=0;i<idx;i++)
            enemies.inc();
        lock_on=enemies.get();
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
}