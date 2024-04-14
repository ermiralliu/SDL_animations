#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include "SDL2/include/SDL_rect.h"
#include "SDL2/include/SDL_render.h"


class Textures{
    private:
        void loadRainTexture();
        void loadBackgrounds();

    public:
        //should be private but idc
        SDL_Renderer* renderer;

        Textures(SDL_Renderer*);
        SDL_Texture* rainTexture;
        SDL_Texture* backgroundTexture;
        SDL_Rect rainSource;
        SDL_Rect backgroundSource;
        SDL_Rect backgroundDest;
    
        Textures(const Textures& obj) = delete;
        void loadTextures();
        static Textures* createTextures();
        static SDL_Texture* TextureLoader(const char* texture, SDL_Renderer* renderer);

};

#endif