#include <iostream>

#define SDL_MAIN_HANDLED
#include "SDL2/include/SDL.h"
#include "windowManagement.hpp"
#include "Raindrop.hpp"
#include "Textures.hpp"

constexpr int RAIN_NUMBER {2048};				//total number of raindrops
constexpr int FRAME_DELAY {16};					//something to do with fps
constexpr int RES_X {1280};						//resolution in x dimension
constexpr int RES_Y {720};						//resolution in y dimension
constexpr const char title[11] = "Animations";  //window title

SDL_Renderer* init(const char* title, int height, int width, int flags){	
	if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){
		SDL_Window* window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,	height, width, flags);
		if(window != 0){
			return SDL_CreateRenderer(window, -1, 0);
		}
	}
	return nullptr;
}

void render(std::array<Raindrop, RAIN_NUMBER>& drop, const Textures& texture){
	SDL_SetRenderDrawColor(texture.renderer, 0, 0, 0, 255);
	SDL_RenderClear(texture.renderer);
	SDL_RenderCopy(texture.renderer, texture.backgroundTexture, &texture.backgroundSource, &texture.backgroundDest);
	for(const auto& point: drop){
		if(point.dest.x <0 || point.dest.y <0 || point.dest.x > RES_X || point.dest.y > RES_Y)
			continue;
		SDL_Rect rect{ (int) point.dest.x,(int) point.dest.y,(int) point.dest.h,(int) point.dest.w};
		SDL_RenderCopy(texture.renderer, texture.rainTexture, &texture.rainSource, &rect);
	}
	SDL_RenderPresent(texture.renderer);
}

void variant_butterfly(std::array<Raindrop, RAIN_NUMBER>& drop){
	for(auto& point: drop){
		point.dest.x +=5;
		if(point.dest.x > RES_X)
			point.dest.x =0;
		
		if(point.dest.y >= RES_Y || point.dest.y < point.max_h)
			point.speed= -point.speed;

		point.dest.y += point.speed;
	}
}

void variant_gravity(std::array<Raindrop, RAIN_NUMBER>& drop){
	for(auto& point: drop){
		point.dest.x +=4;
		if(point.dest.x > RES_X)
			point.dest.x =0;
		
		if(point.dest.y > RES_Y)
			point.speed = -point.speed;
		else
			point.speed+= 0.5;
		point.dest.y += point.speed;
	
	}
}

void variant_torrent(std::array<Raindrop, RAIN_NUMBER>& drop){ //points move circularly (and go toward the center if they're far away)
	for(auto& point: drop){
		int y = point.dest.y - RES_Y/2;
		int x = point.dest.x - RES_X/2;
		if(distance(x,y) >300){
			if(x > 0)
				point.dest.x += -0.03*y - 1; // e kishim thjesht koeficient*y, para se te benim afrimin
			else
				point.dest.x += -0.03*y + 1;
			if(y>0)
				point.dest.y += 0.03*x - 1;
			else
				point.dest.y += 0.03*x +1;
		}
		else{
			point.dest.x += -0.03*y;
			point.dest.y += 0.03*x;
		}
	}
}

void versionControl(){
	std::cout<< "Enter the desired type of effect:\n"
	<<"1 = circle that falls and comes back to it's initial position\n"
	<<"2 = points that fall based on gravity\n"
	<<"3 = circle that goes around\n"
	<<"4 = torrent\n";
}

void initializerControl(){
	std::cout<< "Enter the desired type of initializing:\n"
	<<"1 = circle with speed and maximum heightn\n"
	<<"2 = wide with a maximum height\n"
	<<"3 = circle that goes around\n"
	<<"4 = wide that fill the screen\n";
}

int event(int& version){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		if (e.type== SDL_QUIT){
			return -1;
		}	
		else if(e.type == SDL_KEYDOWN) {
			switch(e.key.keysym.sym){
				case(SDLK_ESCAPE):
					return -1;
				
				case(SDLK_r):
					showConsole();
					versionControl();
					std::cin>>version;
					while(version<=0 || version>4){
						std::cin>>version;
					}
					exeToForeground(title);
					break;

				case(SDLK_i):
					showConsole();
					int initializer{0};
					initializerControl();
					std::cin>>initializer;
					while(initializer<=0 || initializer>4){
						std::cin>>initializer;
					}
					exeToForeground(title);
					return initializer;
			}
		}
	}
	return 0;
}

int main(int argc, char** argv){
	std::ios_base::sync_with_stdio(false);
	
	int version{0};
	versionControl();
	while(version<= 0 || version >4){
		std::cin>> version;
	}

	SDL_Renderer* renderer = init(title, RES_X, RES_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if(renderer == nullptr)
		return 1;
	
	Textures text{renderer};	
	std::array<Raindrop, RAIN_NUMBER> drop;
	
	if(version == 1)
		Raindrop::rain_init_butterfly<RAIN_NUMBER>(drop, text.rainSource, RES_Y, RES_X);
	else if(version ==2)
		Raindrop::rain_init_gravity<RAIN_NUMBER>(drop, text.rainSource, RES_Y, RES_X);
	else if(version ==3)
		Raindrop::rain_init_reflection<RAIN_NUMBER>(drop, text.rainSource, RES_Y, RES_X);
	else if(version == 4)
		Raindrop::rain_init_torrent<RAIN_NUMBER>(drop, text.rainSource, RES_Y, RES_X);

	exeToForeground(title);
	int initializer = version;
	while(true){
		Uint32 frameStart = SDL_GetTicks();
		if(version == 1 || version == 3) //reflection and butterfly have similar mechanics, but differ based on speed and height
			variant_butterfly(drop);
		else if(version == 2)
			variant_gravity(drop);
		else if(version == 4)
			variant_torrent(drop);
		render(drop, text);
		int frameTime = SDL_GetTicks() - frameStart;
		if(FRAME_DELAY > frameTime)
			SDL_Delay(FRAME_DELAY - frameTime);
		int another = event(version);
		if(another == -1)
			break;
		if(initializer != another){
			initializer = another;
			switch(initializer){
				case 1:
					Raindrop::rain_init_butterfly<RAIN_NUMBER>(drop, text.rainSource, RES_Y, RES_X);
					break;
				case 2:
					Raindrop::rain_init_gravity<RAIN_NUMBER>(drop, text.rainSource, RES_Y, RES_X);
					break;
				case 3:
					Raindrop::rain_init_reflection<RAIN_NUMBER>(drop, text.rainSource, RES_Y, RES_X);
					break;
				case 4:
					Raindrop::rain_init_torrent<RAIN_NUMBER>(drop, text.rainSource, RES_Y, RES_X);
					break;
			}
		}
	}
	SDL_Quit();
	return 0;
}