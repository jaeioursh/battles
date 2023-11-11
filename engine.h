#ifndef ENGINE_H
#define ENGINE_H

#include"robot.h"
#include"bots.h"
#include"bullet.h"
#include"lil.h"


class engine{
    public:
        int map_size;
        lil<robot> team1;
        lil<robot> dead1;
        lil<robot> team2;
        lil<robot> dead2;

        lil<bullet> bullets1;
        lil<bullet> bullets2;

        lil<bullet>* bmap1;
        lil<bullet>* bmap2;

        int idx;
        int rob_limit;
        float dt;

        void init();
        void collide();
        void move(float);
        void reset();
        void step(float);
        void clean();
        void add_bot(float,float,float,int,int);
        engine(int);

};

engine::engine(int sz){
   
    map_size=sz;
    bmap1 = new lil<bullet>[sz*sz];
    bmap2 = new lil<bullet>[sz*sz];
    idx=0;
}

void engine::reset(){

}

void engine::step(float dt){
    move(dt);
    if (idx%3==0){
        clean();
        collide();
    }
    idx++;


}

void engine::clean(){
    robot* rob;
    bullet* bul;
    int x,y,idx,len;

    lil<robot>* team;
    lil<robot>* dead;
    lil<bullet>* bullets;
    lil<bullet>* bmap;
    

    for(int q=0;q<2;q++){
        if(q==0){
            team = &team1; dead=&dead1; bullets=&bullets1; bmap=bmap1;
        }
        else{
            team = &team2; dead=&dead2; bullets=&bullets2; bmap=bmap2;
        }
        for(int i=0;i<map_size*map_size;i++)
            bmap[i].clear();
            
        team->reset();
        len=team->len;
        for(int i=0;i<len;i++){
            rob=team->get();
            if(rob->health<=0 ){
                team->remove();
                dead->add(rob);
            }
            else
                team->inc();
        }
        bullets->reset();
        len=bullets->len;
        for(int i=0;i<len;i++){
            bul=bullets->get();
            if(bul->hit == 1 || bul->x < 0 || bul->x >= (float)map_size || bul->y < 0 || bul->y >= (float)map_size){
                bullets->remove();
                delete bul;
            }
            else{
                idx = (int)bul->x +(int)bul->y * map_size;
                bmap[idx].add(bul);
                bullets->inc();
            }

            
        
        }
    }
}
void engine::move(float dt){
    robot* rob;
    robot* spawn;
    bullet* bul;
    int x,y,idx;

    lil<robot>* team;
    lil<robot>* enemy;
    lil<bullet>* bullets;
    

    for(int q=0;q<2;q++){
        if(q==0){
            team = &team1; enemy = &team2; bullets=&bullets1;
        }
        else{
            team = &team2; enemy = &team1; bullets=&bullets2;
        }
        team->reset();
        for(int i=0;i<team->len;i++){
            rob=team->get();
            team->inc();
            rob->ai(*enemy);
            rob->move(dt);
            spawn=rob->alt(dt);
            if(spawn != NULL)
                team->add(spawn);
            if(rob->can_shoot(dt)){
                bul=rob->shoot();
                bullets->add(bul);
            }
        }
        bullets->reset();
        for(int i=0;i<bullets->len;i++){
            bul=bullets->get();
            bullets->inc();
            bul->step(dt);
        }

    }



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
            team = &team1; dead = &dead1; bmap = bmap2;
        }
        else{
            team = &team2; dead = &dead2; bmap = bmap1;
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
                                if(abs(bul->x - rob->x)<rob->rad && abs(bul->y - rob->y)<rob->rad){
                                    rob->health-=bul->dmg;
                                    bul->hit=1;
                                }
                        }
                    }
                
        }

    }

}




void engine::init(){
    


}


void engine::add_bot(float x,float y,float t,int typ,int tteam){
    lil<robot>* team;
    robot* rob=NULL;

    if(tteam==1){
        team = &team1;
    }
    else{
        team = &team2;
    }
    if(typ==0)
        rob=new medium(x,y,t,map_size);
    if(typ==1)
        rob=new large(x,y,t,map_size);
    if(typ==2)
        rob=new turret(x,y,t,map_size);
    if(typ==3)
        rob=new replicator(x,y,t,map_size);
    if(typ==4)
        rob=new uber(x,y,t,map_size);
    
    if(rob!=NULL)
        rob->init();
        team->add(rob);

}
#endif