//
//  main.cpp
//  LastBalls
//
//
//

#include <iostream>
#include "bb.hpp"

int main(int argc, char *argv[])
{
    /*
     int N = atoi(argv[2]);
     srand(atoi(argv[1]));
     double k = atof(argv[3]);
     
     int N = 5;
     double k = 0.5;
    */
    int seed, N;
    double k;
    std::cin >> seed;
    srand(seed);
    std::cin >> N;
    std::cin >> k;
    
    int width, height; //ширина и высота сцены
    if (N > 4) {
        width = (2 * (2 * N + 100) + action_h * N);
        height = (action_w * N * 0.5);
    }
    else {
        width = (200 * N + action_h * N);//вниз
        height = (action_w * N); //вбок
    }
    Fl_Window *win = new Fl_Window(height * k, width * k, "");
    Window *s = new Window(N, k);
    next_step_button(N, k, s);
    win->end();
    win->show();
    int c;
    c = Fl::run();
    delete win;
    return c;
}
