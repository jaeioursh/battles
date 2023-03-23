#include <cmath>
#include <random>
#include <cstdlib>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
//srand(std::time(0));
//int rand()
#define FULLSCREEN 0
#define WIDTH 1300			//width  of screen
#define HEIGHT 700		//height of screen
#define REAN 30*4			//reanimation time
#define SPD 300				//speed of ppl
#define BLOCK_SIZE 5		//chunk size (pixels)
#define BRANGE 3			//range of nearest chunks to see
#define PCHECK 20 			//number of characters to look through (closest)
#define SEE 15				//sight range
#define F_RANGE 10			//firing range
#define FPS 60				//frames per second

#define ACC 100				//percent accurate with shots
#define BOOST 10.0			//percent speed boost by zombies
#define HUM 20000			//humans
#define ZOM 100				//zombies
#define FIRE 1				//percent armed
#define SPAWN 'n'		//  (n)orth,(s)outh,(e)ast,(w)est,(c)enter 
							//  lower case-midle upper case-spead out
#define F_DELAY 0			//fire delay
#define BUILDING 100		//buildings
#define ARMORY 0			//armories

using namespace std;
using namespace sf;

float rrand(){
	return float(rand()%10000)/10000.0;
}

float clip(float L, float U, float V){
	return min(max(L,V),U);
}

int clip(int L, int U, int V){
	return min(max(L,V),U);
}
class player{
public:
	float x;
	float y;
	bool armed;
	bool fail;
	Vertex* point;
	
	int xx;
	int yy;
	int delay;
	
	float oldx;
	double oldy;
	
	float spd;
	float ang;
	
	player* bef;
	player* nxt;

	int reanimate;
	
	int type; //0-ded 1-zomb 2-human
	
	void arm();
	void move(float,float);
	void reset();
	bool fire(int);
	void init(int,int,int,bool,Vertex*);
	void change();
	void kill();
	player();
	~player();


};

player::player(){
	bef=NULL;
	nxt=NULL;
	delay=rand()%5;
}

player::~player(){

}

void player::init(int X,int Y,int t,bool a,Vertex* pt){
	armed=a;
	type=t;
	x=X;
	y=Y;
	point=pt;
	fail=false;

	if(t==2){
		reanimate=REAN;
	}
	else{
		reanimate=0;
	}
	spd = (rand() % SPD)/1000.0 + .20f;
	ang = (rand() % 360) * 3.14f / 180.f;
	point->position=Vector2f(int(x),int(y));
	if(t==1){
		point->color=Color::Red;
	}
	else if(t==2){
		if(armed){
			point->color=Color::Yellow;
		}
		else{
			point->color=Color::Cyan;
		}
	}
}
void player::kill(){
	type=1;
	change();
}


void player::change(){
	switch(type){
		case 0:
			if(reanimate==0){
				point->color=Color::Red;
				type=1;
				spd*=(1.0+BOOST/100.0);
			}
			break;
			
		case 1:
			point->color=Color(210,210,210,100);
			type = 0;
			reanimate=-1;
			break;
			
		case 2:
			point->color=Color(0,0,0,250);
			type = 0;
			break;
	}
}

void player::arm(){
	if(!armed){
		armed=true;
		point->color=Color::Yellow;
	
	}
}

void player::move(float a,float mult){

	oldy=y;
	oldx=x;
	if (type!=0){
		if (a==10){
			y+=mult*spd*sin(ang);
			x+=mult*spd*cos(ang);
		}
		else{
			y+=mult*spd*sin(a);
			x+=mult*spd*cos(a);
			ang=a;	
		}
		point->position=Vector2f(int(x+1),int(y+1));
	}
	else if(reanimate>0){
		reanimate-=1;
		float col=1.0-.8*(float)(reanimate)/(float)(REAN);
		point->color=Color((unsigned char)(255.0*col),0,0,250);
	}
}

void player::reset(){
	y=oldy;
	x=oldx;
	ang = (rand() % 360) * 3.14f / 180.f;
	point->position=Vector2f(int(x+1),int(y+1));
}

bool player::fire(int dly){
	delay--;
	if(delay<=0){
		delay=dly;
		
		return true;
	}
	return false;
}

class lil{
public:
	lil();
	~lil();
	int N;
	float density(int,int);
	void add(player&);
	void remove(player&);
	void update();
	void update(player&);

	player* closest(int,int,int,float&,float&);
	player*** players;
	int** count;
};

lil::lil(){
	N=0;
	players=new player**[WIDTH/BLOCK_SIZE+1];
	count=new int*[WIDTH/BLOCK_SIZE+1];
	for(int i=0;i<WIDTH/BLOCK_SIZE+1;i++){
		players[i]=new player*[HEIGHT/BLOCK_SIZE+1]();
		count[i]=new int[HEIGHT/BLOCK_SIZE+1];
		for(int j=0;j<HEIGHT/BLOCK_SIZE+1;j++)
			count[i][j]=0;
	}
}

lil::~lil(){
	for(int i=0;i<WIDTH/BLOCK_SIZE;i++){
		delete[] players[i];
		delete[] count[i];
	}
	delete[] players;
	delete[] count;
}

player* lil::closest(int x,int y,int r, float& dist,float& ang){
	dist=1000;
	float d;
	
	float dx=0;
	float dy=0;
	float scale=0;
	int N=0;
	player* temp;
	player* close=NULL;
	
	int X=x/BLOCK_SIZE;
	int Y=y/BLOCK_SIZE;
	int x1=max(0,X-r);
	int y1=max(0,Y-r);
	int x2=min(WIDTH/BLOCK_SIZE-1,X+r);
	int y2=min(HEIGHT/BLOCK_SIZE-1,Y+r);

	int i=0;
	int j=0;
	int di=0;
	int dj=-1;
	int t;
	for(int k=0;k<(2*r+1)*(2*r+1);k++) {
	//for (int i=x1;i<=x2;i++){
	//	for(int j=y1;j<=y2;j++){
		if(N>PCHECK)
			break;
		if (X+i >= x1 && X+i <x2 && Y+j >= y1 && Y+j <y2){
		
			temp=players[X+i][Y+j];
			while(temp != NULL){
				N++;
				if(N>PCHECK)
					break;
				d=sqrt(pow(temp->x-x,2)+pow(temp->y-y,2));
				if(d<dist){
					dist=d;
					close=temp;				
				}
				
				if (! temp->fail){
					dy+=sin(temp->ang);
					dx+=cos(temp->ang);
				}
				else{
					dy-=(temp->y-y)/d;
					dx-=(temp->x-x)/d;
				}
				temp=temp->nxt;			
			}		
		}
		
		//cout<<i<<" "<<j<<endl;
		if (i==j || (i<0 && i==-j) || (i>0 && i==1-j)){
			t=di;
			di=-dj;
			dj=t;
		}
		i+=di;
		j+=dj;

	}
	if (dx==0)
		ang=10;
	else
		ang=atan2(dy+0*rrand(),dx+0*rrand());
	return close;
}



float lil::density(int x,int y){
	
	float sum=0;
	int r=1;
	int X=x/BLOCK_SIZE;
	int Y=y/BLOCK_SIZE;
	int x1=max(0,X-r);
	int y1=max(0,Y-r);
	int x2=min(WIDTH/BLOCK_SIZE-1,X+r);
	int y2=min(HEIGHT/BLOCK_SIZE-1,Y+r);
	for (int i=x1;i<=x2;i++){
		for(int j=y1;j<=y2;j++){
			if(i==X && j==Y)
				sum+=float(count[i][j]);
			else
				sum+=.1*float(count[i][j]);
					
		}
	}
	return min(.8,sum/pow(BLOCK_SIZE,2)/2.0);
}

void lil::add(player& person){
	int x=int(person.x)/BLOCK_SIZE;
	int y=int(person.y)/BLOCK_SIZE;
	N++;
	count[x][y]++;
	person.nxt=players[x][y];
	
	if (person.nxt !=NULL)
		person.nxt->bef=&person;
	person.bef=NULL;
	players[x][y]=&person;
	person.xx=x;
	person.yy=y;
}

void lil::remove(player& person){
	int x=int(person.xx);
	int y=int(person.yy);
	N--;
	count[x][y]--;
	if(person.bef==NULL){
		players[x][y]=person.nxt;
	}
	else{
		person.bef->nxt=person.nxt;
	}
	
	if(person.nxt!=NULL){
		person.nxt->bef=person.bef;
	}
	person.nxt=NULL;
	person.bef=NULL;
}

void lil::update(){
	player* temp;
	player* next;
	int x,y;
	for(int i=0;i<WIDTH/BLOCK_SIZE;i++){
		for(int j=0;i<HEIGHT/BLOCK_SIZE;j++){
			temp=players[i][j];
			while(temp != NULL){
				x=int(temp->x)%BLOCK_SIZE;
				y=int(temp->y)%BLOCK_SIZE;
				next=temp->nxt;
				if(y!=temp->yy || x !=temp->xx){
					remove(*temp);
					add(*temp);
				}
				temp=next;
			}
		}
	}
}

void lil::update(player& temp){
	int x,y;

	x=int(temp.x)%BLOCK_SIZE;
	y=int(temp.y)%BLOCK_SIZE;

	if(y!=temp.yy || x !=temp.xx){
		remove(temp);
		add(temp);
	}

}

class game{
public:
	player* players;
	VertexArray points;
	VertexArray line;
	Sprite* background;
	Image bg;
	Texture texture;
	int N;

	int f_d;
	lil dead;
	lil humans;
	lil zombies;
	
	void set_bg(int,int);
    void run();
    void drawline(int,int,float,RenderWindow&);
    void step(RenderWindow*);
    void setz(int&,int&,char);
    
    game(int,int,int,char, int,int ,int);
    
    ~game();
};

void game::setz(int& x,int& y,char c){
	do{
	switch(c){
		case 'S':
			y=rand()%100-100+HEIGHT;
			x=rand()%WIDTH;
			break;
		case 'N':
			y=rand()%100;
			x=rand()%WIDTH;
			break;
		case 'E':
			y=rand()%HEIGHT;
			x=rand()%100-100+WIDTH;
			break;
		case 'W':
			y=rand()%100-50+(HEIGHT)/2;
			x=rand()%100;
			break;
		case 's':
			y=rand()%100-100+HEIGHT;
			x=rand()%100-50+(WIDTH)/2;
			break;
		case 'n':
			y=rand()%100;
			x=rand()%100-50+(WIDTH)/2;
			break;
		case 'e':
			y=rand()%100-50+(HEIGHT)/2;
			x=rand()%100-100+WIDTH;
			break;
		case 'w':
			y=rand()%HEIGHT;
			x=rand()%100;
			break;
		case 'c':
			y=rand()%100-50+(HEIGHT)/2;
			x=rand()%100-50+(WIDTH)/2;
			break;
		default:
			y=rand()%HEIGHT;
			x=rand()%WIDTH;

	}
	
	}while(bg.getPixel(x,y) != Color::Black);
}

game::game(int n,int z,int parmed,char spawn, int fire_delay,int buildings,int armories):points(Points,size_t(n)),line(Lines,2){
	int x,y;
	N=n;

	f_d=fire_delay;
	line[0].color=Color::Yellow;
	line[1].color=Color::Black;
	set_bg(buildings,armories);
	players=new player[N];
	for(int i=0;i<N;i++){
		if(i<z){
			setz(x,y,spawn);
			players[i].init(x,y,1,0,&(points[i]));
			zombies.add(players[i]);
		}
		else{
			do{
				y=rand()%HEIGHT;
				x=rand()%WIDTH;
			}while(bg.getPixel(x,y) != Color::Black);
			
			if(rand()%100<parmed){
				players[i].init(x,y,2,1,&(points[i]));
			}
			else{
				players[i].init(x,y,2,0,&(points[i]));
			}
			humans.add(players[i]);
		}
	}
	
};

game::~game(){
	
	
	delete[] players;
	delete background;

}

void game::set_bg(int num,int numg ){
	VertexArray quad(Quads, 4);
	quad[0].color=Color::White;
	quad[1].color=Color::White;
	quad[2].color=Color::White;
	quad[3].color=Color::White;
	
	VertexArray quadg(Quads, 4);
	quadg[0].color=Color::Yellow;
	quadg[1].color=Color::Yellow;
	quadg[2].color=Color::Yellow;
	quadg[3].color=Color::Yellow;
	
	
	int X,Y,dx,dy;
	RenderTexture renderTexture;
	
	if (!renderTexture.create(WIDTH,HEIGHT))
	{
			// error...
			return;
	}
	renderTexture.clear(Color::Black);
	
	for(int i=0;i<num+5;i++){
		X=rand()%WIDTH;
		Y=rand()%HEIGHT;
		dx=20+rand()%50;
		dy=20+rand()%50;
		switch(i){
		case 0:
			Y=HEIGHT;
			X=0;
			dy=-30;
			dx=350;
			break;
		case 1:
			Y=HEIGHT;
			X=0;
			dy=-5;
			dx=WIDTH;
			break;
		case 2:
			Y=0;
			X=0;
			dy=5;
			dx=WIDTH;
			break;
		case 3:
			Y=0;
			X=WIDTH;
			dy=HEIGHT;
			dx=-5;
			break;
		case 4:
			Y=0;
			X=0;
			dy=HEIGHT;
			dx=5;
			break;
		
		}	
		quad[0].position = Vector2f(X,Y);
		quad[1].position = Vector2f(X+dx, Y);
		quad[2].position = Vector2f(X+dx, Y+dy);
		quad[3].position = Vector2f(X, Y+dy);

		renderTexture.draw(quad);
	}
	for(int i=0;i<numg;i++){
		X=rand()%WIDTH;
		Y=rand()%HEIGHT;
		dx=20+rand()%50;
		dy=20+rand()%50;
		
		quadg[0].position = Vector2f(X,Y);
		quadg[1].position = Vector2f(X+dx, Y);
		quadg[2].position = Vector2f(X+dx, Y+dy);
		quadg[3].position = Vector2f(X, Y+dy);

		renderTexture.draw(quadg);
	}	

		texture = renderTexture.getTexture();
		bg=texture.copyToImage();

		background =new Sprite(texture);

}

void game::step(RenderWindow* window){
	player* close;
	int n_close;
	float speed_mult=1.0;
	float d;
	float a;
	float d2;
	for(int i=0;i<N;i++){
		switch (players[i].type){
			speed_mult=1.0;
			case 0:	//dead ai
				players[i].move(10,0);
				players[i].change();
				if(players[i].type==1){
					dead.remove(players[i]);
					zombies.add(players[i]);
				}
				break;
				
			case 1:		//zombie ai
				close=humans.closest(players[i].x,players[i].y,BRANGE,d,a);
				zombies.closest(players[i].x,players[i].y,BRANGE,d2,a);
				//speed_mult=1-zombies.density(players[i].x,players[i].y);	
				if (close==NULL or d>SEE*1.1){
					//players[i].move(10,speed_mult);
					players[i].move(a,speed_mult);
				}
				else if(d<1.5){
				
					humans.remove(*close);
					close->change();
					dead.add(*close);

					break;
				}
				else{
					players[i].move(atan2(-players[i].y+close->y,-players[i].x+close->x),speed_mult);
				}
				if(bg.getPixel(players[i].x,players[i].y) != Color::Black){
					players[i].reset();	
					players[i].fail=true;

					if(bg.getPixel(players[i].x,players[i].y) != Color::Black){
							
						zombies.remove(players[i]);	
						players[i].kill();
						dead.add(players[i]);
					}
				}
				else
					players[i].fail=false;

				if (players[i].type==1)
					zombies.update(players[i]);
				break;
				
			case 2:		//human ai
				close=zombies.closest(players[i].x,players[i].y,BRANGE,d,a);
				
				//speed_mult=1-humans.density(players[i].x,players[i].y);
				if (close==NULL or d>SEE){ // no line of sight
					players[i].move(10,speed_mult);
				}
				else if(d<F_RANGE && players[i].armed && players[i].fire(f_d)){ // fire
					
					if (window != NULL)
						drawline(int(players[i].x),int(players[i].y),atan2(close->y-players[i].y,close->x-players[i].x),*window);
					if (rand()%100 < ACC){
						zombies.remove(*close);
						close->change();
						dead.add(*close);
					}
			
									
				}
				else{ // move away from seen zombie
					players[i].move(atan2(players[i].y-close->y,players[i].x-close->x) ,speed_mult);
				}
				if(bg.getPixel(players[i].x,players[i].y) != Color::Black){
					if(bg.getPixel(players[i].x,players[i].y) == Color::Yellow)
						players[i].arm();
					
					
					else
						players[i].fail=true;

					players[i].reset();
					if(bg.getPixel(players[i].x,players[i].y) != Color::Black){
		
						humans.remove(players[i]);	
						players[i].kill();
						dead.add(players[i]);
					}
				}
				else
					players[i].fail=false;
				if (players[i].type==2)
					humans.update(players[i]);
				
				
				break;
		
		}
	}
}

void game::drawline(int x,int y,float ang,RenderWindow& window){
	x+=2*cos(ang);
	y+=2*sin(ang);
	line[0].position=Vector2f(x, y);
	x+=8*cos(ang);
	y+=8*sin(ang);
	line[1].position=Vector2f(x, y);
	window.draw(line);
}

void game::run(){
	cout<<"Press space to start"<<endl;
	bool paused=1;
	int x1,x2,y1,y2;
	bool sclicked=0;
	bool lclicked=0;
	bool rclicked=0;
	VertexArray brec(Quads, 4);
	brec[0].color=Color::Black;
	brec[1].color=Color::Black;
	brec[2].color=Color::Black;
	brec[3].color=Color::Black;
	
	VertexArray wrec(Quads, 4);
	wrec[0].color=Color::White;
	wrec[1].color=Color::White;
	wrec[2].color=Color::White;
	wrec[3].color=Color::White;
	RenderWindow* window;
	if (FULLSCREEN)
		window= new RenderWindow(VideoMode(WIDTH, HEIGHT), "My window", Style::Fullscreen);
	else
		window=new RenderWindow(VideoMode(WIDTH, HEIGHT), "My window");
	window->setFramerateLimit(FPS);
	Text text;
	Font font;

	font.loadFromFile("NanumGothicBold.ttf");
	text.setColor(Color::Magenta);
	text.setFont(font); 
	text.setCharacterSize(24); 
	
    // run the program as long as the window is open
    while (window->isOpen())
    {
    	
        // check all the window's events that were triggered since the last iteration of the loop
        Event event;
        
        window->draw(*background);
        
        while (window->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Q)){
                window->close();
                delete window;
                return;
            }
            if (Keyboard::isKeyPressed(Keyboard::Space))
			{
				if(!sclicked){
					sclicked=!sclicked;
					paused=!paused;
				}
			}
			else
				sclicked=0;
			
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				//cout<<"hi"<<endl;
				// left click...
				if(!lclicked){
				lclicked=!lclicked;
					x1=Mouse::getPosition(*window).x;
					y1=Mouse::getPosition(*window).y;
					
				}
				else{
					x2=Mouse::getPosition(*window).x;
					y2=Mouse::getPosition(*window).y;
					wrec[0].position=Vector2f(x1, y1);
					wrec[1].position=Vector2f(x2, y1);
					wrec[2].position=Vector2f(x2, y2);
					wrec[3].position=Vector2f(x1, y2);
					
				}
			}
			else
			{
				if(lclicked){
					lclicked=0;
					
					RenderTexture renderTexture;
					renderTexture.create(WIDTH,HEIGHT);
					//renderTexture.clear(Color::Black);
					renderTexture.draw(*background);
					renderTexture.draw(wrec);
					texture = renderTexture.getTexture();
					bg=texture.copyToImage();
					bg.flipVertically();
					texture.loadFromImage(bg);
					background->setTexture(texture);
				}
			}
			
			
			if (Mouse::isButtonPressed(Mouse::Right))
			{
				//cout<<"hi"<<endl;
				// left click...
				if(!rclicked){
					rclicked=!rclicked;
					x1=Mouse::getPosition(*window).x;
					y1=Mouse::getPosition(*window).y;
					x1=clip(5,WIDTH-5,x1);
					y1=clip(5,HEIGHT-5,y1);
				}
				else{
					x2=Mouse::getPosition(*window).x;
					y2=Mouse::getPosition(*window).y;
					x2=clip(5,WIDTH-5,x2);
					y2=clip(5,HEIGHT-5,y2);
					brec[0].position=Vector2f(x1, y1);
					brec[1].position=Vector2f(x2, y1);
					brec[2].position=Vector2f(x2, y2);
					brec[3].position=Vector2f(x1, y2);
					
				}
			}
			else
			{
				if(rclicked){
					rclicked=0;
					
					RenderTexture renderTexture;
					renderTexture.create(WIDTH,HEIGHT);
					//renderTexture.clear(Color::Black);
					renderTexture.draw(*background);
					renderTexture.draw(brec);
					texture = renderTexture.getTexture();
					bg=texture.copyToImage();
					bg.flipVertically();
					texture.loadFromImage(bg);
					background->setTexture(texture);
				}
			}
			
        
        }
        

        text.setString("H: "+to_string(humans.N)+"  Z: "+to_string(zombies.N)+"  D: "+to_string(dead.N));
        if (lclicked)
        	window->draw(wrec);
        if (rclicked)
        	window->draw(brec);
		if(!paused)
			step(window);
		window->draw(points);
		
		window->draw(text);
        // end the current frame
        window->display();
    }

}

int main(){
srand(std::time(0));

game hvz(HUM+ZOM,ZOM,FIRE,SPAWN,  F_DELAY,BUILDING,ARMORY);
hvz.run();


return 0;
}
