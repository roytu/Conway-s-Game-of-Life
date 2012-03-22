#include <SDL.h>
#include <vector>

const int WIDTH = 20;
const int HEIGHT = 20;

const int XSCALE = 8;
const int YSCALE = 8;

const int FREQ_SPAWN = 8; //frequency = 1 per FREQ_SPAWN squares

int getNeighbors(std::vector< std::vector<bool> > g, int x, int y);
void updateGrid();
void drawScreen();
void randomizeGrid();

void FillRect(int x, int y, int w, int h, int color); // patrick - http://pbeblog.wordpress.com/2009/06/24/drawing-rectangles-sdl/

SDL_Surface* screen=NULL;
std::vector< std::vector<bool> > grid (WIDTH, std::vector<bool>(HEIGHT));
std::vector< std::vector<bool> > gridnew (WIDTH, std::vector<bool>(HEIGHT));
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

int getNeighbors(std::vector< std::vector<bool> > g,int x,int y)
{
	int neighbors=0;
	if(x>0 && y>0)
	{
		if(g[x-1][y-1])
		{
			neighbors++;
		}
	}
	if(y>0)
	{
		if(g[x][y-1])
		{
			neighbors++;
		}
	}
	if(x<WIDTH-1 && y>0)
	{
		if(g[x+1][y-1])
		{
			neighbors++;
		}
	}

	if(x>0)
	{
		if(g[x-1][y])
		{
			neighbors++;
		}
	}
	if(x<WIDTH-1)
	{
		if(g[x+1][y])
		{
			neighbors++;
		}
	}

	if(x>0 && y<HEIGHT-1)
	{
		if(g[x-1][y+1])
		{
			neighbors++;
		}
	}
	if(y<HEIGHT-1)
	{
		if(g[x][y+1])
		{
			neighbors++;
		}
	}
	if(x<WIDTH-1 && y<HEIGHT-1)
	{
		if(g[x+1][y+1])
		{
			neighbors++;
		}
	}

	return neighbors;
}

void updateGrid()
{
	for(int x=0;x<WIDTH;x++)
	{
		for(int y=0;y<HEIGHT;y++)
		{
			gridnew[x][y]=grid[x][y];
			int n = getNeighbors(grid,x,y);
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