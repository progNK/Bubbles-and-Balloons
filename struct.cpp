//
//  struct.cpp
//  LastBalls
//
//

#include <stdio.h>
#include "bb.hpp"

Scene::Scene(int N, double k):N(N)
{
    int i;
    cells = new Box *[N * N];// массив с уазателями на ячейки типа box
    for (i = 0; i < N * N; i++)
    cells[i] = cells[i]->beginning(k * action_w + action_spacing + k * ((i % N) * 100), action_spacing + k * ((i / N) * 100), k * 100);
}

Radio_Buttons::Radio_Buttons(int n, double k)
{
    int i;
    rb[0] = new Fl_Radio_Button(k * spacing, k * (action_spacing), k * action_w, k * action_h, "blow_balloon");
    rb[1] = new Fl_Radio_Button(k * spacing, k * (2 * action_spacing + action_h), k * action_w, k * action_h, "blow_bubble");
    rb[2] = new Fl_Radio_Button(k * spacing, k * (3 * action_spacing + 2 * action_h), k * action_w, k * action_h, "take_balloon");
    rb[3] = new Fl_Radio_Button(k * spacing, k * (4 * action_spacing + 3 * action_h), k * action_w, k * action_h, "pin_bubble");
    for (i = 0; i < 4; i++)
    rb[i]->labelsize(rb[i]->labelsize() * letter_size);
}

int Radio_Buttons::is_on()
{
    int i;
    for (i = 0; i < 4; i++)
    if (rb[i]->value()) return i;
    return 0;
}

Scene::~Scene()
{
    for (int i = 0; i < N * N; i++) {
        //delete cells[i]->button;
        delete cells[i]->get_button();//удаление для каждого по полям button
    }
    delete[] cells;
};

Bubble::~Bubble(){};

Balloon::~Balloon(){};

Round::~Round(){};

Box::~Box(){};

Empty::~Empty(){};

Radio_Buttons::~Radio_Buttons()
{
    int i;
    for (i = 0; i < 4; i++)
        delete rb[i];
};

Window::~Window()
{
    delete &sc;
    delete &buttons;
};

Box::Box(Box *c): button(c->button), type(c->type) {};//поля button и type, на вход элемент c; передаю поля через ->

Empty::Empty(Box *cell): Box(cell)
{
  //  Box::type = 0;
    Box::set_type(0);
    //Box::button->color(FL_GRAY);
    Box::get_button()->color(FL_GRAY);
    Fl::redraw();
}

Balloon::Balloon(Box *e): Round(e)//метод от round(e) который вызовет box
{//иерархия до box и в box все начинается делаться
    //Box::type = 1;
    Box::set_type(1);
    //Box::button->color(FL_DARK_RED);
    Box::get_button()->color(FL_DARK_RED);
    Fl::redraw();
}

Bubble::Bubble(Box *e): Round(e)
{
    //Box::type = 2;
    Box::set_type(2);
    //Box::button->color(FL_DARK_BLUE);
    Box::get_button()->color(FL_DARK_BLUE);
    Fl::redraw();
}

Empty* Box::beginning(double x, double y, double size)
{
    Empty *e = new Empty;
    e->type = 0;
    e->button = new Fl_Button(x, y, size, size, "");
    e->button->color(FL_GRAY);
    return e;
}

Window::Window(int n, double k): sc(n, k), buttons(n, k) {};//сразу передаю поля

int error_window()
{
    Fl_Window *win = new Fl_Window(500, 250, "");
    Fl_Box *b = new Fl_Box (0, 0, 500, 250, "Impossible");
    b->labelsize(b->labelsize() * letter_size);
    win->show();
    int c = Fl::run();
    delete b;
    delete win;
    return c;
};

