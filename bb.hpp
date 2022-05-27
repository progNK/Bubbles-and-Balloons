//
//  bb.hpp
//  LastBalls
//
//

#ifndef bb_hpp
#define bb_hpp

#include <iostream>
#include <ostream>
#include <cmath>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Button.H>


enum {
    spacing = 5, // distance between
    action_spacing = 20,
    button_w = 200, // главные кнопки
    button_h = 150,
    option_w = 200, // радио-кнопки
    option_h = 20,
    action_w = 400,
    action_h = 150,
    letter_size = 2 // шрифт
};

class Scene;
class Empty;
class Window;

class Box {
    Fl_Button *button;
    int want;
    int type;
public:
    Box(): button(nullptr){};
    Box(Box *);
    Empty* beginning(double x, double y, double size);
    ~Box();
    virtual void choose_cell(int num, int N, int *cells1) = 0;
    virtual void change_dir(int num, int N, int *cells1) = 0;
    virtual void move(int i, Scene *s, int want, int *cells1) = 0;
    static void blow_pin(Fl_Widget *w, void *user);
    int get_type() {return type;}
    void set_type(int op) {type = op;}
    int get_want() const {return want;}
    void set_want(int op) {want = op;}
    Fl_Button* get_button() {return button;};
    void set_button(Fl_Button* op1) {button = op1;};
};

class Empty: public Box {
public:
    Empty(): Box(){};
    ~Empty();
    Empty(Box *);//принимает на вход box* создаются пустые ячейки от указателя box*, иерархия наследования
    void choose_cell(int num, int N, int *cells1){};
    void change_dir(int num, int N, int *cells1){};
    void move(int i, Scene *s, int want, int *cells1){};
};

class Round: public Box {
public:
    Round(Box *e): Box(e) {};
    ~Round();
    void choose_cell(int num, int N, int *cells1);
    void change_dir(int num, int N, int *cells1){};
};

class Bubble: public Round {
public:
    Bubble(Box*);//вызывают 
    ~Bubble();
    virtual void move(int i, Scene *s, int want, int *cells1);
};

class Balloon: public Round {
public:
    Balloon(Box*);
    ~Balloon();
    virtual void move(int i, Scene *s, int want, int *cells1);
    void change_dir(int num, int N, int *cells1);
};

class Scene {
    Box **cells;
    int N;
public:
    friend class Box;
    friend class Empty;
    friend class Balloon;
    friend class Bubble;
    friend int next_step_button(int N, double k, Window *s);
    friend void next_step_callback(Fl_Widget *w, void *user);
    friend void one_more(Scene *s, bool *try_again, Box **before_move);
    Scene(int N, double k);
    ~Scene();
    int get_N() {return N;}
    void set_N(int op) {N = op;}
 //   Box** get_cells(int i) {return cells[i];};
  //  void set_cells(Box** op1, int i) {cells[i] = op1;};
};

struct Radio_Buttons {
    Fl_Radio_Button *rb[4];
public:
    Radio_Buttons(int N, double k);
    int is_on();
    ~Radio_Buttons();
};

class Window {
    Scene sc;
    Radio_Buttons buttons;
public:
    friend class Box;
    friend class Empty;
    friend class Balloon;
    friend class Bubble;
    friend int next_step_button(int N, double k, Window *s);
    Window(int N, double k);
    ~Window();
  //  Scene get_sc() {return sc;};
  //  void set_sc(Scene op) {sc = op;};
  //  Radio_Buttons get_buttons() {return buttons;};
  //  void set_buttons(Radio_Buttons op) {buttons = op;};
};

int error_window();
int next_step_button(Fl_Widget *w, void *user);


#endif /* bb_hpp */
