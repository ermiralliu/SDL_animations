#include "Textures.hpp"
#include "SDL2/include/SDL_image.h"

Textures::Textures(SDL_Renderer* renderer){
	this->renderer = renderer;
	this->loadTextures();
}

void Textures::loadRainTexture(){
	rainTexture = TextureLoader("assets/snow.png", renderer);
	SDL_QueryTexture(rainTexture, nullptr, nullptr, &rainSource.w, &rainSource.h);
	rainSource.x = 0;
	rainSource.y = 0;
}
void Textures::loadBackgrounds(){
    backgroundTexture = TextureLoader("assets/sky1.png", renderer);
	SDL_QueryTexture(backgroundTexture, nullptr, nullptr, &backgroundSource.w, &backgroundSource.h);
	backgroundDest = {0, 0, backgroundSource.w, backgroundSource.h};
	backgroundSource.x = 0;
	backgroundSource.y = 0;
}

void Textures::loadTextures(){
    loadRainTexture();
    loadBackgrounds();
}

SDL_Texture* Textures::TextureLoader(const char* texture, SDL_Renderer* renderer){
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	return tex;
}