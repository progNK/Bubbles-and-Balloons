//
//  bb.cpp
//  LastBalls
//
//

#include "bb.hpp"
//указатель на fltk

void Box::blow_pin(Fl_Widget *w, void *user)
{
    int i;
    Window *win = (Window *)user;
    i = 0;
    while(win->sc.cells[i]->button != w) i++;//find index of the pressed button
    if (win->buttons.is_on() == 0) {
        if (win->sc.cells[i]->get_type() == 0)//+balloon
            win->sc.cells[i] = new Balloon(win->sc.cells[i]);
        else
            error_window();
    } else if (win->buttons.is_on() == 1) {//+bubble
        if (win->sc.cells[i]->get_type() == 0)
            win->sc.cells[i] = new Bubble(win->sc.cells[i]);
        else
            error_window();
    }  else if (win->buttons.is_on() == 2) {//-balloon
        if (win->sc.cells[i]->get_type() == 1)
            //win->Sc.cells[i] = new Round(win->Sc.cells[i]);
            win->sc.cells[i] = new Empty(win->sc.cells[i]);
        else
            error_window();
    } else if (win->buttons.is_on() == 3) {//-bubble
        if (win->sc.cells[i]->get_type() == 2)
            //win->Sc.cells[i] = new Round(win->Sc.cells[i]);
            win->sc.cells[i] = new Empty(win->sc.cells[i]);
        else
            error_window();
    }
};

//num - номер ячейки
//i - номер строчки j - номер столбца
void Round::choose_cell(int num, int N, int *cells1)
{
    int i, j;
    i = num / N;
    j = num % N;
  
    set_want( rand() % 8);

    if (get_want() == 0) {i--; j--;}//левый верхний
    else if (get_want() == 1) {i--;}
    else if (get_want() == 2) {i--; j++;}
    else if (get_want() == 3) {j++;}
    else if (get_want() == 4) {i++; j++;}
    else if (get_want() == 5) {i++;}
    else if (get_want() == 6) {i++; j--;}
    else if (get_want() == 7) {j--;};

    if (i >= 0 && j >= 0 && i < N && j < N)//для границы
        //want = i * N + j;
        set_want(i * N + j);
    else
        //want = num;
        set_want(num);
    cells1[get_want()]++;
    this->set_want(get_want());
};

void Bubble::move(int i, Scene *s, int want, int *cells1)
{
    s->cells[want] = new Bubble (s->cells[want]);
    if (cells1[i] == 0 || cells1[i] > 1) {//cells1 - массив претендентов на эту ячейку
        s->cells[i] = new Empty (s->cells[i]);
    }
}

void Balloon::move(int i, Scene *s, int want, int *cells1) {
    s->cells[want] = new Balloon (s->cells[want]);
    if (cells1[i] == 0 || cells1[i] > 1) {
        s->cells[i] = new Empty (s->cells[i]);
    }
}

void Balloon::change_dir(int num, int N, int *cells1)
{
    int i, j;
    i = num / N;
    j = num % N;
    if (get_want() == num - 1 - N) {i++; j++;}//левый верхний
    else if (get_want() == num - N) i++;
    else if (get_want() == num + 1 - N) {i++; j--;}
    else if (get_want() == 1 + num) j--;//направо
    else if (get_want() == num + 1 + N) {i--; j--;}
    else if (get_want() == num + N) i--;
    else if (get_want() == num - 1 + N) {i--; j++;}
    else if (get_want() == num - 1) j++;
    
    
    if (i >= 0 && j >= 0 && i < N && j < N)//для границы
        set_want(i * N + j);
    else
        set_want(num);
    cells1[get_want()]++;
    this->set_want(get_want());
    
};

void one_more(Scene *s, bool *try_again, Box **before_move) {
    int n = (s->get_N()) * (s->get_N());
    int i;
    int *cells1 = new int[n];
    for (i = 0; i < n; i++)
        cells1[i] = 0;
    for (i = 0; i < n; i++) {//fill massives
        if (try_again[i])
            s->cells[i]->change_dir(i, s->get_N(), cells1);//opposite direction
        if (s->cells[i] != 0 && !try_again[i])//if taken by alredy moved obj
            cells1[i]++;//колво претендентов
        before_move[i] = s->cells[i];// save current state
    }
    bool *second_try = new bool[n];
    for (i = 0; i < n; i++)
        second_try[i] = false;
    for (i = 0; i < n; i++) {
        if (try_again[i]) {
            if (cells1[before_move[i]->get_want()] > 1)//>1
                second_try[i] = true;
            else {
                s->cells[i]->move(i, s, before_move[i]->get_want(), cells1);
                second_try[before_move[i]->get_want()] = false;
            }
        } else
            second_try[i] = false;
    }
    delete[] cells1;
    delete[] try_again;
    try_again = second_try;
};

void next_step_callback(Fl_Widget *w, void *user) {
    Scene *s = (Scene *)user;
    int n = (s->get_N()) * (s->get_N());
    int i;
    int *cells1 = new int[n];
    for (i = 0; i < n; i++)
        cells1[i] = 0;
    Box **before_move = new Box *[n];
    for (i = 0; i < n; i++) {
        s->cells[i]->choose_cell(i, s->get_N(), cells1);
        before_move[i] = s->cells[i];
    }//fill number of pretending obj
    bool *try_again = new bool[n];
    for (i = 0; i < n; i++)
        try_again[i] = false;
    for (i = 0; i < n; i++) {//rand direction
        if (before_move[i]->get_type() != 0) {
            if (cells1[before_move[i]->get_want()] >= 2) {//if conflict(more than 1)
                if (before_move[i]->get_type() == 2) {// if Bubble => pop
                    s->cells[i] = new Empty (s->cells[i]);
                    try_again[i] = false;
                } else {//if Balloon
                    try_again[i] = true;
                }
            }
            else {// no more pretendors
                s->cells[i]->move(i, s, before_move[i]->get_want(), cells1);
                try_again[before_move[i]->get_want()] = false;
            }
        } else//if empty
            try_again[i] = false;
    }
    delete[] cells1;
    one_more(s, try_again, before_move);//1 for unmoving
    one_more(s, try_again, before_move);//2 for unmoving
    delete[] before_move;
};

int next_step_button(int N, double k, Window *s)
{
    int i;
    Fl_Button *Next_step = new Fl_Button (k * spacing, k * (5 * action_spacing + 4 * action_h), k * action_w, k * action_h, "Next_step -->");
    Next_step->labelsize(Next_step->labelsize() * letter_size);
    Next_step->callback(next_step_callback, (void *)(&(s->sc)));
    for (i = 0; i < N * N; i++) {
        s->sc.cells[i]->get_button()->callback(s->sc.cells[i]->blow_pin, (void *) s);
    };
    int c;
    c = Fl::run();
    return c;
};

