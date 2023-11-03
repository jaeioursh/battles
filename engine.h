
#include"robot.h"
#include"bullet.h"
#include"lil.h"


class engine{
    public:
        int map_size;
        int res;
        lil<robot> team1;
        lil<robot> dead1;
        lil<robot> team2;
        lil<robot> dead2;

        lil<bullet> bullets1;
        lil<bullet> bullets2;

        lil<bullet>* bmap1;
        lil<bullet>* bmap2;

        int idx;
        float fps;
        int rob_limit;
        float dt;

        void init();
        void collide();
        void reset();
        void step();
        void clean(lil<robot>*,lil<robot>*,lil<bullet>*);
        engine(int,float);

};

engine::engine(int sz,float FPS){
    fps=FPS;
    map_size=sz;
    bmap1 = new lil<bullet>[sz*sz];
    bmap2 = new lil<bullet>[sz*sz];

}

void engine::reset(){

}

void engine::step(){
    


}

void engine::clean(lil<robot>* team,lil<robot>* dead,lil<bullet>* bullets){

}

void engine::collide(){
    robot* rob;
    bullet* bul;
    int x,y,idx;

    lil<robot>* team;
    lil<robot>* dead;
    lil<bullet>* bullets;
    lil<bullet>* bmap;

    for(int q=0;q<2;q++){
        if(q==0){
            team = &team1;
            dead = &dead1;
            bmap = bmap2;
        }
        else{
            team = &team2;
            dead = &dead2;
            bmap = bmap1;
        }
        
        team->reset();
        for(int i=0;i<team->len;i++){
            rob=team->get();
            team->inc();
            x=(int)rob->x;
            y=(int)rob->y;
            for(int X=x-1;X<x+2;X++)
                for(int Y=y-1;Y<y+2;Y++)
                    if (Y>=0 && Y<map_size && X>=0 && X<map_size){
                        idx = X+Y*map_size;
                        bmap[idx].reset();
                        for(int j=0;j<bmap[idx].len;j++){
                            bul=bmap[idx].get();
                            bmap[idx].inc();
                            if(!bul->hit && rob->health>0)
                                if(abs(bul->x-rob->x)<1.0 && abs(bul->x - rob->x)<1.0){
                                    rob->health-=bul->dmg;
                                    bul->hit=1;
                                }
                        }
                    }
                
        }
        clean(team,dead,bullets);
    }

}

void engine::init(){
    


}