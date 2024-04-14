#ifndef RAINDROP_HPP
#define RAINDROP_HPP

#include "SDL2/include/SDL_rect.h"
#include <array>
#include <random>
#include <time.h>
#include <math.h>

using Random = std::subtract_with_carry_engine<unsigned int,11,7,11>;

int distance(float x, float y){
    return (int) sqrt(x*x + y*y);
}

struct Raindrop{
    SDL_FRect dest;
    float speed{0};
    float max_h{0};


    template <int SIZE> static void rain_proto_straight(std::array<Raindrop, SIZE>& drop, const SDL_Rect& rainSource, int y_init, int y_range, int x_init, int x_range){
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
    template <int SIZE> static void rain_proto_circle(std::array<Raindrop, SIZE>& drop, const SDL_Rect& rainSource,int RES_Y,int RES_X){
        Random engine((unsigned)time(nullptr));
        const int center_x = RES_X/2;
        const int center_y = RES_Y/2;
        for(auto& point: drop){
            auto& y {point.dest.y};
            auto& x {point.dest.x};
            y = engine()%(RES_Y-200) +100;
            x = engine()%(RES_Y-200) + 380;
            while( distance(x-center_x, y-center_y) > 260)
                x = engine()%(RES_Y-200) + 380;
            point.dest.h = rainSource.h;
            point.dest.w = rainSource.w;
        }
    };


    template <int SIZE> static void rain_init_butterfly(std::array<Raindrop, SIZE>& drop, const SDL_Rect& rainSource,int RES_Y,int RES_X){
        rain_proto_circle<SIZE>(drop, rainSource, RES_Y, RES_X);
        for(auto& point: drop){
            point.max_h = point.dest.y;
            point.speed = (RES_Y - point.dest.y)/100;
        }
    };// will have proto_circle with another loop for speed and max
    template <int SIZE> static void rain_init_gravity(std::array<Raindrop, SIZE>& drop, const SDL_Rect& rainSource,int RES_Y,int RES_X){
        rain_proto_straight<SIZE>(drop, rainSource, 100, RES_Y-100, 100, RES_X);
    };
    
    template <int SIZE> static void rain_init_reflection(std::array<Raindrop, SIZE>& drop, const SDL_Rect& rainSource,int RES_Y,int RES_X){
        rain_proto_circle<SIZE>(drop, rainSource, RES_Y, RES_X);
    };
    template <int SIZE> static void rain_init_torrent(std::array<Raindrop, SIZE>& drop, const SDL_Rect& rainSource,int RES_Y,int RES_X){
        rain_proto_straight<SIZE>(drop, rainSource, (RES_Y-RES_X)/2 , RES_X, 0, RES_X);
    };

};

#endif