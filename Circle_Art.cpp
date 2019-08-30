
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <sstream>
#include <vector>
#include "Simple_window.h"
#include "Graph.h"
#include "std_lib_facilities.h"

using namespace Graph_lib;
using namespace std;

int main() {

	srand(time(0));
	const double PI = 3.14159265;
	const double OUTER_RADIUS = 340.0, X_CENTER = 400.0, Y_CENTER = 350.0;

	int user_input = 0, number_rings = 0, starting_angle = 0, color = 0;
	double r_theta = 0.0, d_theta = 0.0, inner_radius = 0.0, r_starting_angle = 0.0;
	double x_outer = 0.0, y_outer = 0.0, x_inner = 0.0, y_inner = 0.0;

	do{
		Vector_ref<Circle> inner_circles;
		cout << "Enter the round of inner circle generations: " << endl;
		cin >> user_input;

		if(user_input < 0) break;

		Point tl(500,100);
		Simple_window display(tl,800,800,"Programming Assignment #6");

		color = rand() % 255;

		Circle boundary(Point(X_CENTER,Y_CENTER),OUTER_RADIUS);
		boundary.set_color(Color(color));
		display.attach(boundary);

		for(int i = 0; i < user_input; ++i){ // Draw the inner circles
			number_rings = rand() % 48 + 2;
			r_theta = (2.00 * PI) / number_rings;
			d_theta = r_theta * 180.00 / PI;
			inner_radius = (OUTER_RADIUS * sin(PI / number_rings)) / (1.0 + sin(PI / number_rings));
			starting_angle = rand() % 360; // Pick a random point on the circle to begin and end ring generation
			r_starting_angle = starting_angle * PI / 180.0; // convert to radians to prepare for calculations below

			for(int j = 0; j < number_rings; ++j){

				color = rand() % 255; // set color for each individual ring
				// Positions inner circle edge on the outer circle
				x_outer = X_CENTER + OUTER_RADIUS * cos((r_starting_angle + j * r_theta)); // R*cos(theta)
				y_outer = Y_CENTER - OUTER_RADIUS * sin((r_starting_angle + j * r_theta)); // R*sin(theta)
				// Identify center point for each ring
				x_inner = X_CENTER + (OUTER_RADIUS - inner_radius) * cos((r_starting_angle + j * r_theta)); // (R - r)*cos(theta)
				y_inner = Y_CENTER - (OUTER_RADIUS - inner_radius) * sin((r_starting_angle + j * r_theta)); // (R - r)*sin(theta)

				inner_circles.push_back(new Circle(Point(x_inner,y_inner),inner_radius));
				int len = inner_circles.size();
				inner_circles[len - 1].set_color(Color(color));
				display.attach(inner_circles[len - 1]);
			}
		}

		ostringstream oss;
		if(user_input <= 1) oss << "After " << user_input << " ring generation. ";
		else if(user_input > 1) oss << "After " << user_input << " rings generation. ";
		Text output(Point(320,720),oss.str());
		output.set_color(Color::white);
		display.attach(output);
		display.wait_for_button();

		for(int i = 0, len = inner_circles.size(); i < len; ++i){
			display.detach(inner_circles[i]);
		}
		display.detach(boundary);
		display.detach(output);

	}while(user_input >= 0);

	return 0;
}


