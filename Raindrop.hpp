#ifndef RAINDROP_HPP
#define RAINDROP_HPP

#include "SDL2/include/SDL_rect.h"
#include <array>
#include <random>
#include <time.h>

using Random = std::subtract_with_carry_engine<unsigned int,11,7,11>;

struct Raindrop{
    SDL_FRect dest;
    RainType type = SNOW;
    float speed{0};
    float max_h{0};

    template <int SIZE> static void rain_proto_straight(std::array<Raindrop, SIZE> drop, SDL_Rect& rainSource, int y_init, int y_range, int x_init, int x_range){
        Random engine((unsigned) time(nullptr));
        for(auto& point: drop){
            auto& x {point.dest.x};
            auto& y {point.dest.y};
            x = engine()%x_range + x_init;
            y = engine()%y_range + y_init;
            point.dest.h = rainSource.h;
            point.dest.w = rainSource.w;
        }
    };
    template <int SIZE> static void rain_proto_circle(std::array<Raindrop, SIZE> drop, SDL_Rect& rainSource,int RES_Y,int RES_X){
        Random engine((unsigned)time(nullptr));
        int center_x = RES_X/2;
        int center_y = RES_Y/2;
        for(auto& point: drop){
            int& y {point.dest.y};
            int& x {point.dest.x};
            y = engine()%(RES_Y-200) +100;
            x = engine()%(RES_Y-200) + 380;
            while( distance(x-center_x, y-center_y) > 260)
                x = engine()%(RES_Y-200) + 380;
            point.dest.h = rainSource.h;
            point.dest.w = rainSource.w;
        }
    };


    template <int SIZE> static void rain_init_butterfly(std::array<Raindrop, SIZE> drop, SDL_Rect& rainSource,int RES_Y,int RES_X);// will have proto_circle with another loop for speed and max
    template <int SIZE> static void rain_init_gravity<RAIN_NUMBER>(std::array<Raindrop, SIZE> drop, SDL_Rect& rainSource,int RES_Y,int RES_X);
    
    template <int SIZE> static void rain_init_reflection<RAIN_NUMBER>(std::array<Raindrop, SIZE> drop, SDL_Rect& rainSource,int RES_Y,int RES_X);
    template <int SIZE> static void rain_init_torrent<RAIN_NUMBER>(std::array<Raindrop, SIZE> drop, SDL_Rect& rainSource,int RES_Y,int RES_X);

};

#endif