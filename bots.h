

#include "robot.h"


class large:public robot{
    public:
        void init() override;

};
void large::init(){
    health=100;
    speed=0.1;
    dmg=10;
    rps=5;
    clip=4;
    reload_delay=2;
    
    float Model[]={-1.  , -1.  , -1.  ,  1.  , -0.5 ,  1.  , -0.5 , -1.  , -0.5 ,
       -0.75,  0.5 , -0.75,  0.5 , -0.25, -0.5 , -0.25,  0.5 , -1.  ,
        0.5 ,  1.  ,  1.  ,  1.  ,  1.  , -1.  };
    model_len=24;
    setup(Model);
}