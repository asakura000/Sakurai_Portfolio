#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <sstream>
#include <vector>
#include "Graph.h"
#include "std_lib_facilities.h"
#include "GUI.h"
#include "Window.h"

using namespace Graph_lib;
using namespace std;

//------------Class Definition --------------------------------------------

struct Buffons_window : Graph_lib::Window {
    Buffons_window(Point xy, int w, int h, const string& title );
    ~Buffons_window(){ }

private:
    // Data:

    vector<Line*> p_needles;
	vector<Point*> p_center_points;
	vector<double*> first_points;
	vector<double*> second_points;
	vector<Text*> vtxt;

	Line line_1;
    Line line_2;
    Line line_3;
    Line line_4;
    Line line_5;
    Line line_6;
    Rectangle text_box;
    Text* output_pi;

    bool nbr_listed;

    const double PI;
    const int WIDTH;
    const int RADIUS;
    const int X_START_FIRST_LINE;
    const int LEFT_LIMIT, RIGHT_LIMIT;
    const int X_RANGE;

    int d_theta;
    double r_theta;
    int x_center;
    int y_center;
    int number_needles;
    int cross_count;
    double estimate;
    double pt_x, pt_y, pt_x_prime, pt_y_prime;


    // Widgets:

    Button drop_button;
    Button quit_button;
    Button menu_button;
    In_box drop_count;
    Menu motion_menu;

    // Functions:

    void hide_menu() { motion_menu.hide(); menu_button.show(); }

    // Actions invoked by callbacks:

    void count_pressed();
    void rotate_pressed();
	void unlist_pressed();
	void menu_pressed() { menu_button.hide(); motion_menu.show(); }
    void drop();
    void quit();

    // Callback functions:

    static void cb_count(Address, Address);
    static void cb_rotate(Address, Address);
    static void cb_unlist(Address, Address);
    static void cb_menu(Address, Address);
    static void cb_drop(Address, Address);
    static void cb_quit(Address, Address);

};

//------------ Constructor --------------------------------------------

Buffons_window::Buffons_window(Point xy, int w, int h, const string& title)
    :Window(xy,w,h,title),

	line_1(Point(120,30),Point(120,500)),
	line_2(Point(240,30),Point(240,500)),
	line_3(Point(360,30),Point(360,500)),
	line_4(Point(480,30),Point(480,500)),
	line_5(Point(600,30),Point(600,500)),
	line_6(Point(720,30),Point(720,500)),
	drop_button(Point(x_max()-150,0), 70, 20, "Drop", cb_drop),
	quit_button(Point(x_max()-70,0), 70, 20, "Quit", cb_quit),
	menu_button(Point(x_max()-80,30), 80, 20, "Motion Menu", cb_menu),
	drop_count(Point(x_max()-210,0), 50, 20, "Enter drop count "),
	motion_menu(Point(x_max()-70,30),70,20,Menu::vertical,"motion"),
	text_box(Point(200,225),400,20),
	output_pi(),
	nbr_listed(false),

	// constants
	PI(3.14159265),
	WIDTH(120),
	RADIUS(WIDTH/2),
	X_START_FIRST_LINE(120),
	LEFT_LIMIT(120 - RADIUS),
	RIGHT_LIMIT(720 + RADIUS),
	X_RANGE(RIGHT_LIMIT - LEFT_LIMIT),

	// variables
	d_theta(0),
	r_theta(0.0),
	x_center(0),
	y_center(0),
	number_needles(0),
	cross_count(0),
	pt_x(0.0),
	pt_y(0.0),
	pt_x_prime(0.0),
	pt_y_prime(0.0),
	estimate(0.0)
{
    attach(drop_button);
    attach(quit_button);
    attach(drop_count);

    line_1.set_color(Color(76));
    attach(line_1);
    line_2.set_color(Color(76));
    attach(line_2);
    line_3.set_color(Color(76));
    attach(line_3);
    line_4.set_color(Color(76));
    attach(line_4);
    line_5.set_color(Color(76));
    attach(line_5);
    line_6.set_color(Color(76));
    attach(line_6);

    text_box.set_color((Color(76)));
    text_box.set_fill_color((Color(76)));

    motion_menu.attach(new Button(Point(0,0),0,0,"Count",cb_count));
    motion_menu.attach(new Button(Point(0,0),0,0,"Rotate",cb_rotate));
    motion_menu.attach(new Button(Point(0,0),0,0,"(Un)list",cb_unlist));
    attach(motion_menu);
    motion_menu.hide();
    attach(menu_button);
}

void Buffons_window::cb_quit(Address, Address pw)
{
	reference_to<Buffons_window>(pw).quit();
}

void Buffons_window::quit()
{
	hide();
}

void Buffons_window::cb_drop(Address, Address pw)
{
	reference_to<Buffons_window>(pw).drop();
}

void Buffons_window::drop()
{
	detach(text_box);
	detach(*output_pi);

	number_needles = drop_count.get_int();
	if(p_needles.size() != 0){
		for(int i = 0, len = p_needles.size(); i < len; ++i){
			detach(*p_needles[i]);
			p_needles.pop_back();
			p_center_points.pop_back();
			first_points.pop_back();
			second_points.pop_back();
		}
	}

	for(int i = 0; i < number_needles; ++i){

		// Set-up: random center point for one needle
		x_center = rand() % X_RANGE + LEFT_LIMIT;
		y_center = rand() % 500;

		// store center points in memory
		Point* cp = new Point(Point(x_center,y_center));
		p_center_points.push_back(cp);
		d_theta = rand() % 360;
		r_theta = (d_theta * PI) / 180;

		// store first points in memory
		pt_x = x_center + RADIUS * cos(r_theta);
		pt_y = y_center + RADIUS * sin(r_theta);
		double* fp = new double(pt_x);
		first_points.push_back(fp);

		// store second points in memory
		pt_x_prime = x_center - RADIUS * cos(r_theta);
		pt_y_prime = y_center - RADIUS * sin(r_theta);
		double* sp = new double(pt_x_prime);
		second_points.push_back(sp);

		// store needles into a vector of lines
		Line * pl = (new Line(Point(pt_x,pt_y),Point(pt_x_prime,pt_y_prime)));
		p_needles.push_back(pl);
		pl->set_color(Color::black);
		attach(*pl);

	}
	redraw();
}

void Buffons_window::cb_count(Address, Address pw)
{
	reference_to<Buffons_window>(pw).count_pressed();
}

void Buffons_window::count_pressed(){

	cross_count = 0;
	number_needles = drop_count.get_int();
	for(int i = 0, len = p_needles.size(); i < len; ++i){
		double pt_left = 0.0, pt_right = 0.0;
		if(*first_points[i] <= *second_points[i]){ // put the 2 x-coordinates in ascending order
			pt_left = *first_points[i];
			pt_right = *second_points[i];
		}
		else if(*first_points[i] >= *second_points[i]){
			pt_left = *second_points[i];
			pt_right = *first_points[i];
		}

		for (int j = 0; j < 6; ++j){
			bool check = false;
			if(X_START_FIRST_LINE * (j+1) >= pt_left && X_START_FIRST_LINE * (j+1) <= pt_right){
				check = true;
				p_needles[i]->set_color(Color::red);
				++cross_count;
				break;
			}
		}
	}

	if(cross_count == 0)estimate = 0;
	else if(cross_count > 0) estimate = (2.0 * number_needles) / cross_count;

	ostringstream pi_estimate;
	pi_estimate << "Found " << cross_count << " crossed needles, and estimate of pi is " << estimate << "!";
	output_pi = new Text(Point(220,240),pi_estimate.str());
	output_pi->set_color(Color::white);

	attach(text_box);
	attach(*output_pi);
	redraw();
}

void Buffons_window::cb_rotate(Address, Address pw)
{
	reference_to<Buffons_window>(pw).rotate_pressed();
}

void Buffons_window::rotate_pressed()
{
	detach(text_box);
	detach(*output_pi);

	for(int i = 0, len = p_needles.size(); i < len; ++i){
		detach(*p_needles[i]);
		p_needles.pop_back();
		first_points.pop_back();
		second_points.pop_back();
	}

	for(int i = 0, len = p_center_points.size(); i < len; ++i){

		d_theta = rand() % 360;
		r_theta = (d_theta * PI) / 180;
		pt_x = p_center_points[i]->x + RADIUS * cos(d_theta);
		double* fp = new double(pt_x);
		first_points.push_back(fp);
		pt_y = p_center_points[i]->y + RADIUS * sin(d_theta);
		pt_x_prime = p_center_points[i]->x - RADIUS * cos(d_theta);
		double* sp = new double(pt_x_prime);
		second_points.push_back(sp);
		pt_y_prime = p_center_points[i]->y - RADIUS * sin(d_theta);

		Line* plr = new Line(Point(pt_x,pt_y),Point(pt_x_prime,pt_y_prime));
		plr->set_color(Color::black);
		p_needles.push_back(plr);
		attach(*plr);
	}

	redraw();
}

void Buffons_window::cb_unlist(Address, Address pw)
{
	reference_to<Buffons_window>(pw).unlist_pressed();
}

void Buffons_window::unlist_pressed()
{
	detach(text_box);
	detach(*output_pi);

	if(nbr_listed == true){
		nbr_listed = false;
		for(int i = 0, len = vtxt.size(); i < len; ++i){
			detach(*vtxt[i]);
			vtxt.pop_back();
		}
	}
	else if(nbr_listed == false){
		for(int i = 0, len = p_center_points.size(); i < len; ++i){
			ostringstream numbers;
			numbers << i;
			Text* ptxt = new Text(Point(p_center_points[i]->x+5, p_center_points[i]->y+5),numbers.str());
			ptxt->set_color(Color::black);
			vtxt.push_back(ptxt);
			attach(*ptxt);
		}
		nbr_listed = true;
	}
	redraw();
}

void Buffons_window::cb_menu(Address, Address pw)
{
	reference_to<Buffons_window>(pw).menu_pressed();
}

//------------ Main --------------------------------------------

int main()
try {
	Buffons_window win(Point(100,100),840,510,"Programming Assignment #7");
    return gui_main();
}
catch(exception& e) {
    cerr << "exception: " << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "Some exception\n";
    return 2;
}

