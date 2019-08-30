#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <locale>
using namespace std;

class Board{
	vector<vector<char> > sudoku;

public:
	Board(){ //Initial sudoku board

		vector<char> row_0 = {'1', '9', '5', '2', '6', '8', '3', '7', '4'}; sudoku.push_back(row_0);
		vector<char> row_1 = {'4', '8', '7', '1', '3', '5', '6', '2', '9'}; sudoku.push_back(row_1);
		vector<char> row_2 = {'6', '3', '2', '7', '4', '9', '1', '5', '8'}; sudoku.push_back(row_2);
		vector<char> row_3 = {'2', '1', '8', '3', '7', '6', '9', '4', '5'}; sudoku.push_back(row_3);
		vector<char> row_4 = {'3', '4', '9', '8', '5', '1', '2', '6', '7'}; sudoku.push_back(row_4);
		vector<char> row_5 = {'5', '7', '6', '4', '9', '2', '8', '1', '3'}; sudoku.push_back(row_5);
		vector<char> row_6 = {'7', '2', '1', '5', '8', '3', '4', '9', '6'}; sudoku.push_back(row_6);
		vector<char> row_7 = {'9', '5', '3', '6', '1', '4', '7', '8', '2'}; sudoku.push_back(row_7);
		vector<char> row_8 = {'8', '6', '4', '9', '2', '7', '5', '3', '1'}; sudoku.push_back(row_8);
	}
	void eraser(Board& b){
		vector<int> numbers;
		for(int i = 0; i < 81; ++i) numbers.push_back(i);

		random_shuffle(numbers.begin(), numbers.end());

		int position = 0, row = 0, col = 0;

		position = numbers[0];
		row = position / 9;
		col = position % 9;

		cout << "Erasing row " << char('P' + row) << " column " << char('A' + col) << endl;
		b.sudoku[row][col] = '-';

		numbers.erase(numbers.begin() + 0);
	}
	bool check_one_box(const Board& b, int r_start, int c_start){
		bool check_box = false;
		int count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0, count6 = 0, count7 = 0, count8 = 0, count9 = 0;
		for(int row = r_start; row < r_start + 3; ++row){
			for(int col = c_start; col < c_start + 3; ++col){
				if(b.sudoku[row][col] == '1') ++count1;
				else if(b.sudoku[row][col] == '2') ++count2;
				else if(b.sudoku[row][col] == '3') ++count3;
				else if(b.sudoku[row][col] == '4') ++count4;
				else if(b.sudoku[row][col] == '5') ++count5;
				else if(b.sudoku[row][col] == '6') ++count6;
				else if(b.sudoku[row][col] == '7') ++count7;
				else if(b.sudoku[row][col] == '8') ++count8;
				else if(b.sudoku[row][col] == '9') ++count9;
			}
		}
			if(count1 != 1 || count2 != 1 || count3 != 1 || count4 != 1 || count5 != 1 || count6 != 1 || count7 != 1 || count8 != 1 || count9 != 1){
				check_box = false;
				cout << "- Found inconsistency in component starting at row " << char('P' + r_start) << " and column " << char('A' + c_start) << "..." << endl;
			}
			else check_box = true;

		return check_box;
	}

	void swapper(Board& b){

		int swap_type = 0;
		swap_type = rand() % 2; // 0 = swap rows, 1 = swap columns

		int first_rand = 0, second_rand = 0, temp = 0;
		first_rand = rand() % 9; second_rand = rand() % 9;

		while(first_rand == second_rand){
			second_rand = rand() % 9;
			if(first_rand != second_rand) break;
		}

		if(first_rand > second_rand){
			temp = first_rand;
			first_rand = second_rand;
			second_rand = temp;
		}

		if (swap_type == 0) cout << "Trying to swap rows " << char('P' + first_rand) << " and " << char('P' + second_rand) << "..." << endl;
		else if(swap_type == 1) cout << "Trying to swap columns " << char('A' + first_rand) << " and " << char('A' + second_rand) << "..." << endl;

		int group_first = 0, group_second = 0; // Groups board into 3 groups of 3 rows/columns, assigns each rand to a group

		if(first_rand <= 2) group_first = 0;
		else if(first_rand > 2 && first_rand <= 5) group_first = 1;
		else if(first_rand > 5 && first_rand <= 8) group_first = 2;

		if(second_rand <= 2) group_second = 0;
		else if(second_rand > 2 && second_rand <= 5) group_second = 1;
		else if(second_rand > 5 && second_rand <= 8) group_second = 2;

		int pick = 0, first = 0, second = 0, third = 0;

		pick = first_rand % 3; // finds the position of the randomly generated row/column within the three row/col group

		if(pick == 0){ //sorts the three rows/columns in ascending order
			first = pick;
			second = pick + 1;
			third = pick + 2;
		}
		else if(pick == 1){
			first = pick - 1;
			second = pick;
			third = pick + 1;
		}
		else if(pick == 2){
			first = pick - 2;
			second = pick -1;
			third = pick;
		}

		int diff = 0;
		diff = group_second - group_first; // determines whether swap will happen within adjacent groups or whether there is a skip

		if(group_first == group_second){ // simple swap
			if(swap_type == 0){
				for(int col = 0; col < 9; ++col) swap(b.sudoku[first_rand][col], b.sudoku[second_rand][col]);
				cout << "- Rows " << char('P' + first_rand) << " and " << char('P' + second_rand) << " were swapped... " << endl;
			}
			else if(swap_type == 1){
				for(int row = 0; row < 9; ++row) swap(b.sudoku[row][first_rand], b.sudoku[row][second_rand]);
				cout << "- Columns " << char('A' + first_rand) << " and " << char('A' + second_rand) << " were swapped... " << endl;
			}
		}
		else if(group_first != group_second){ // not a simple swap
			if(swap_type == 0){ // swapping rows
				for(int col = 0; col < 9; ++col) swap(b.sudoku[first][col], b.sudoku[first + diff * 3][col]);
				cout << "- Rows " << char('P' + (group_first * 3 + first)) << " and " << char('P' + (group_second * 3 + first)) << " were swapped... " << endl;
				for(int col = 0; col < 9; ++col) swap(b.sudoku[second][col], b.sudoku[second + diff * 3][col]);
				cout << "- Rows " << char('P' + (group_first * 3 + second)) << " and " << char('P' + (group_second * 3 + second)) << " were swapped... " << endl;
				for(int col = 0; col < 9; ++col) swap(b.sudoku[third][col], b.sudoku[third + diff * 3][col]);
				cout << "- Rows " << char('P' + (group_first * 3 + third)) << " and " << char('P' + (group_second * 3 + third)) << " were swapped... " << endl;
			}
			else if(swap_type == 1){ // swapping columns
				for(int row = 0; row < 9; ++row) swap(b.sudoku[row][first], b.sudoku[row][first + diff * 3]);
				cout << "- Columns " << char('A' + (group_first * 3 + first)) << " and " << char('A' + (group_second * 3 + first)) << " were swapped... " << endl;
				for(int row = 0; row < 9; ++row) swap(b.sudoku[row][second], b.sudoku[row][second + diff * 3]);
				cout << "- Columns " << char('A' + (group_first * 3 + second)) << " and " << char('A' + (group_second * 3 + second)) << " were swapped... " << endl;
				for(int row = 0; row < 9; ++row) swap(b.sudoku[row][third], b.sudoku[row][third + diff * 3]);
				cout << "- Columns " << char('A' + (group_first * 3 + third)) << " and " << char('A' + (group_second * 3 + third)) << " were swapped... " << endl;
			}
		}
	}

	void verifier(const Board& b){
		bool verified = false;
		bool check_row = false;
		bool check_column = false;
		bool check_boxes = false;
		int count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0, count6 = 0, count7 = 0, count8 = 0, count9 = 0;
		int true_count = 0;

		// 1) Check all rows

		for(int row = 0; row < 9; ++row){
			for(int col = 0; col < 9; ++col){
				if(b.sudoku[row][col] == '1') ++count1;
				else if(b.sudoku[row][col] == '2') ++count2;
				else if(b.sudoku[row][col] == '3') ++count3;
				else if(b.sudoku[row][col] == '4') ++count4;
				else if(b.sudoku[row][col] == '5') ++count5;
				else if(b.sudoku[row][col] == '6') ++count6;
				else if(b.sudoku[row][col] == '7') ++count7;
				else if(b.sudoku[row][col] == '8') ++count8;
				else if(b.sudoku[row][col] == '9') ++count9;
			}
				if(count1 != 1 || count2 != 1 || count3 != 1 || count4 != 1 || count5 != 1 || count6 != 1 || count7 != 1 || count8 != 1 || count9 != 1){
					check_row = false;
					cout << "- Found inconsistency in row " << char('P' + row) << "..." << endl; // test purposes only
				}
				else ++ true_count;
				count1 = 0; count2 = 0, count3 = 0, count4 = 0, count5 = 0, count6 = 0, count7 = 0, count8 = 0, count9 = 0;
			}
			if(true_count == 9)check_row = true;
			else if(true_count != 9)check_row = false;

		// 2) Check all columns

		true_count = 0;
		for(int col = 0; col < 9; ++col){
			for(int row = 0; row < 9; ++row){
				if(b.sudoku[row][col] == '1') ++count1;
				else if(b.sudoku[row][col] == '2') ++count2;
				else if(b.sudoku[row][col] == '3') ++count3;
				else if(b.sudoku[row][col] == '4') ++count4;
				else if(b.sudoku[row][col] == '5') ++count5;
				else if(b.sudoku[row][col] == '6') ++count6;
				else if(b.sudoku[row][col] == '7') ++count7;
				else if(b.sudoku[row][col] == '8') ++count8;
				else if(b.sudoku[row][col] == '9') ++count9;
			}
			if(count1 != 1 || count2 != 1 || count3 != 1 || count4 != 1 || count5 != 1 || count6 != 1 || count7 != 1 || count8 != 1 || count9 != 1){
				check_column = false;
				cout << "- Found inconsistency in column " << char('A' + col) << "..." << endl; // Test purposes only
			}
			else ++ true_count;
			count1 = 0; count2 = 0, count3 = 0, count4 = 0, count5 = 0, count6 = 0, count7 = 0, count8 = 0, count9 = 0;
		}
		if(true_count == 9) check_column = true;
		else if(true_count != 9) check_column = false;

		// 3) Check 3 x 3 boxes

		true_count = 0;
		if(check_one_box(b, 0, 0) == true) ++ true_count; // top left
		if(check_one_box(b, 0, 3) == true) ++ true_count; // top middle
		if(check_one_box(b, 0, 6) == true) ++ true_count; // top right
		if(check_one_box(b, 3, 0) == true) ++ true_count; // middle left
		if(check_one_box(b, 3, 3) == true) ++ true_count; // middle middle
		if(check_one_box(b, 3, 6) == true) ++ true_count; // middle right
		if(check_one_box(b, 6, 0) == true) ++ true_count; // bottom left
		if(check_one_box(b, 6, 3) == true) ++ true_count; // bottom middle
		if(check_one_box(b, 6, 6) == true) ++ true_count; // bottom right

		if(true_count == 9) check_boxes = true;
		else if(true_count != 9) check_boxes = false;

		// 4) Do row checks, column checks, and box checks all return true?

		if(check_row == true && check_column == true && check_boxes == true){
		 verified = true;
		 cout << "- All columns, rows, and components are OK..." << endl;
		}
		else verified = false;
	}
	friend ostream& operator<<(ostream& o, const Board& b){
		o << "   ";
		for (int col = 0; col < 9; ++col) o << setw(2) << char('A' + col);
		o << endl;
		for(int row = 0; row < 9; ++row){
			o << setw(2) << char('P' + row) << " ";
			for(int col = 0; col < 9; ++col){
			o << setw(2) << b.sudoku[row][col];
			}
			o << endl;
		}
			return o;
		}
};

class Game{
	Board b;

public:
	void start(){
		string command;
		while(true){
			cout << "> " << endl;
			cin >> command;
			for(int i = 0, len = command.length(); i < len; ++i) command[i] = tolower(command[i]);
			if(command != "quit" && command != "show" && command != "erase" &&  command != "verify" && command != "swap")
			cout << "Please enter a valid command." << endl;
			if(command == "show") cout << b;
			else if(command == "erase") b.eraser(b);
			else if(command == "verify")b.verifier(b);
			else if(command == "swap")b.swapper(b);
			else if(command == "quit")break;
		}
		cout << "Bye..." << endl;
	}
};

int main() {
	srand(time(0));
	Board b; Game g;
	cout << "Welcome Sudoku Initializer!" << endl;
	g.start();
	return 0;
}










