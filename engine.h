
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

        lil<bullet> bbin;

        lil<bullet> bullets1;
        lil<bullet> bullets2;

        lil<bullet>** bmap1;
        lil<bullet>** bmap2;

        int idx;
        int fps;
        int rob_limit;

        void init();
};

void engine::init(){



}