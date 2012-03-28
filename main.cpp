#include <SDL.h>
#include <vector>

const int WIDTH = 128;
const int HEIGHT = 96;

const int XSCALE = 8;
const int YSCALE = 8;

const int FREQ_SPAWN = 4; //frequency = 1 per FREQ_SPAWN squares

const int DIRECTIONS[8][2]={{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};

int getNeighbors(int x, int y);
void updateGrid();
void updateGridNeighbors();
void drawScreen();
void randomizeGrid();
template<typename T> T getCell(std::vector< std::vector<T> > g, int x, int y);
template<typename T> void setCell(std::vector< std::vector<T> >* g, int x, int y, T value);

void FillRect(int x, int y, int w, int h, int color); // patrick - http://pbeblog.wordpress.com/2009/06/24/drawing-rectangles-sdl/

SDL_Surface* screen=NULL;
std::vector< std::vector<bool> > grid (WIDTH, std::vector<bool>(HEIGHT));
std::vector< std::vector<bool> > gridnew (WIDTH, std::vector<bool>(HEIGHT));
std::vector< std::vector<int> > gridneighbors (WIDTH, std::vector<int>(HEIGHT, 0));
Uint32* pixels;

int main( int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	bool quit=false;
	SDL_Event event;

	screen=SDL_SetVideoMode(WIDTH*XSCALE,HEIGHT*YSCALE,32,SDL_SWSURFACE);
	pixels = (Uint32*)screen->pixels;

	randomizeGrid();

	while(!quit)
	{
		updateGrid();
		if(SDL_PollEvent(&event))
		{
			if(event.type==SDL_KEYDOWN)
			{
				switch(event.key.keysym.sym)
				{
					case SDLK_SPACE:
						randomizeGrid();
					break;
				}
			}
			else if(event.type==SDL_QUIT)
			{
				quit=true;
			}
		}
		drawScreen();
	}

	SDL_Quit();
	return 0;
}

void updateGridNeighbors()
{
	for(int x=0;x<WIDTH;x++)
	{
		for(int y=0;y<HEIGHT;y++)
		{
			gridneighbors[x][y]=0;
		}
	}
	for(int x=0;x<WIDTH;x++)
	{
		for(int y=0;y<HEIGHT;y++)
		{
			if(grid[x][y])
			{
				for(int d=0;d<8;d++)
				{
					int dx = DIRECTIONS[d][0];
					int dy = DIRECTIONS[d][1];
					int ov = gridneighbors[((x+dx)+WIDTH)%WIDTH][((y+dy)+HEIGHT)%HEIGHT];
					gridneighbors[((x+dx)+WIDTH)%WIDTH][((y+dy)+HEIGHT)%HEIGHT]=ov+1;
				}
			}
		}
	}
}

template<typename T> T inline getCell(std::vector< std::vector<T> > g, int x, int y)
{
	return g[(x+WIDTH)%WIDTH][(y+HEIGHT)%HEIGHT];
}

template<typename T> void inline setCell(std::vector< std::vector<T> >* g, int x, int y, T value)
{
	int xx=(x+WIDTH)%WIDTH;
	int yy=(y+HEIGHT)%HEIGHT;
	std::vector< std::vector<T> > gp = *g;
	gp[xx][yy]=value;
}

int getNeighbors(int x,int y)
{
	return gridneighbors[x][y];
}

void updateGrid()
{
	updateGridNeighbors();
	for(int x=0;x<WIDTH;x++)
	{
		for(int y=0;y<HEIGHT;y++)
		{
			gridnew[x][y]=grid[x][y];
			int n = getNeighbors(x,y);
			if(grid[x][y]) //live cell
			{
				if(n<2 || n>3)
				{
					gridnew[x][y]=false;
				}
			}
			else //dead cell
			{
				if(n==3)
				{
					gridnew[x][y]=true;
				}
			}
		}
	}
	grid.swap(gridnew);
}

void drawScreen()
{
	for(int x=0;x<WIDTH;x++)
	{
		for(int y=0;y<HEIGHT;y++)
		{
			if(grid[x][y])
			{
				FillRect(x*XSCALE,y*YSCALE,XSCALE,YSCALE,0x000000);
			}
			else
			{
				FillRect(x*XSCALE,y*YSCALE,XSCALE,YSCALE,0xFFFFFF);
			}
		}
	}

	SDL_Flip(screen);
}

void randomizeGrid()
{
	for(int x=0;x<WIDTH;x++)
	{
		for(int y=0;y<HEIGHT;y++)
		{
			grid[x][y]=!(rand()%FREQ_SPAWN>=1);
			gridnew[x][y]=false;
		}
	}
}

void FillRect(int x, int y, int w, int h, int color)
{
	SDL_Rect rect = {x,y,w,h};
	SDL_FillRect(screen, &rect, color);
}