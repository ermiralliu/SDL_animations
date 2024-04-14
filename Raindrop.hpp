#ifndef RAINDROP_HPP
#define RAINDROP_HPP

#include "SDL2/include/SDL_rect.h"
#include <math.h>
#include <time.h>
#include <random>

using Random =  std::subtract_with_carry_engine<unsigned int,11,7,11>;

inline float distance(int number1, int number2){
    return sqrt(number1*number1 + number2*number2);
}

struct Raindrop{
    SDL_FRect dest;
    float speed;
    float max_h;

    template <int SIZE> static void rain_init_1_prototype(std::array<Raindrop, SIZE>& drop, SDL_Rect& rainSource, int RES_Y, int RES_X, bool max){
        Random engine((unsigned)time(NULL));
        int center_x = RES_X/2;
        int center_y = RES_Y/2;
        constexpr int min_h{100};
        for(auto& point : drop){
            float& y {point.dest.y};
            float& x {point.dest.x};

            y = engine()%(RES_Y-200) +100;
            x = engine()%(RES_Y-200) + 380;
            while( distance(x-center_x, y-center_y) > 260 ) //making them fall inside the circle
                x = engine()%(RES_Y-200) + 380;

            
            if(max==true){
                point.speed = (float) (RES_Y - y)/min_h;
                point.max_h = point.dest.y; 
            }
            else{
                point.speed = 3; 
                point.max_h = 0; 
            }
            point.dest.h = rainSource.h;
            point.dest.w = rainSource.w;
        }
    };

    template <int SIZE> static void rain_init_reflection(std::array<Raindrop, SIZE>& drop, SDL_Rect& rainSource, int RES_Y, int RES_X){
        rain_init_1_prototype<SIZE>( drop, rainSource, RES_Y, RES_X, false);
    }

    template <int SIZE> static void rain_init_butterfly(std::array<Raindrop, SIZE>& drop, SDL_Rect& rainSource, int RES_Y, int RES_X){
        rain_init_1_prototype<SIZE>( drop, rainSource, RES_Y, RES_X, true);
    };

    template <int SIZE> static void rain_init_2_prototype(std::array<Raindrop, SIZE>& drop, SDL_Rect& rainSource, int x_range, int y_range, int x_init, int y_init){
        Random engine((unsigned)time(NULL));
        for(auto& point : drop){
            float& y {point.dest.y};
            float& x {point.dest.x};

            point.speed = 0;
            point.max_h = 0; 

            y = engine()%(y_range) + y_init;
            x = engine()%(x_range) + x_init;
            point.dest.h = rainSource.h;
            point.dest.w = rainSource.w;
        }
    };

    template <int SIZE> static void rain_init_gravity(std::array<Raindrop, SIZE>& drop, SDL_Rect& rainSource, int RES_Y, int RES_X){
        rain_init_2_prototype<SIZE>(drop, rainSource, RES_X, RES_Y-100, -100, +100);
    };

    template <int SIZE> static void rain_init_torrent(std::array<Raindrop, SIZE>& drop, SDL_Rect& rainSource, int RES_Y, int RES_X){
        rain_init_2_prototype<SIZE>(drop, rainSource, RES_Y, RES_Y, (RES_X-RES_Y)/2, 0);
    };
    
};

#endif