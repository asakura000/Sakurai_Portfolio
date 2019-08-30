#include <iostream>
#include <cstdlib>
#include <vector>
#include <iomanip>
#include <string>
#include <ctime>
#include <cassert>
#include <algorithm>
using namespace std;

void createWords(vector<string>&wordVector, const string& l) {
	string word; bool found = false;
	for (int i = 0, len = l.length(); i < len; ++i){
		if (!isspace(l[i]) && isalpha(l[i])){
			word += l[i];
			found = true;
			continue;
		}
		if (!found) continue;
		wordVector.push_back(word);
		word = "";
		found = false;
	}
	if(found)wordVector.push_back(word);
}

void createChallengeSet(vector<string>&v1, vector<string>&v2){
	random_shuffle(v1.begin(), v1.end());
	int numberPicks = rand()% 3 + 1;
	for (int i = 0; i < numberPicks; ++i){
		v2.push_back(v1[i]);
	}
}

string createOutput(vector<string>&wordVector, int x){
	string s = "";
	if(x == 0){ // output with no strike outs
		for (int i = 0, len = wordVector.size(); i < len; ++i){
			s += wordVector[i]; //concatenated string
		}
		random_shuffle(s.begin(), s.end());
		}
	else if(x >= 1){ //strike outs
		for(int i = 0, len = wordVector.size(); i < len; ++i){
			s += wordVector[i]; //concatenated string
			if(x > s.length()){
				for(int i = 0, len = s.length(); i < len; ++i){
					s.replace(i,1,"_");
				}
			}
			else if(x <= s.length()){
				for(int i = 0; i < x; ++i){
					s.replace(i,1,"_");
				}
			}
		}
		random_shuffle(s.begin(), s.end());
	}
	return s;//scrambled, concatenated string
}

bool winRound(vector<string>&v1, vector<string>&v2){
	bool win = true;
	int a = 0;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	for(int i = 0, len = v1.size(); i < len; ++i){
		if(v1[i] == v2[i]) ++a;
		else a = 0;
	}
	if(a == v2.size())win = true;
	else win = false;
	return win;
}

void outputAnswers(vector<string>&s){
	for (int i = 0, len = s.size(); i < len; ++i){ //outputs the answers
		cout << i + 1 << ": " << s[i] << endl;
	}
}

void foundNonAlpha(string s){
    try{
		for(int i = 0, len = s.length(); i < len; ++i){
			if(isalpha(s[i]) || isspace(s[i])){
			}
			else throw '*';
		}
	}
	catch (char e){
		cout << "Error: invalid input (must be all letters)" << endl;
	}
}

int main() {

	srand(time(0));
	vector<string>animalBank;
	string animal;
	cout << "Enter at least five animal names, e.g., cat, dog, etc..." << endl;

	while(animalBank.size() < 5){
		do{
			cout << "> ";
			getline(cin, animal);
			foundNonAlpha(animal);
			createWords(animalBank, animal);
		}while(!animal.empty());
		if(animalBank.size() < 5 && animal.empty())cout << "Enter at least 5." << endl;
	}

	for (int i = 0, len = animalBank.size(); i < len; ++i){
		cout << i + 1 << ": " << animalBank[i] << endl;
	}

	vector<string>challengeSet; //Answers
	string guess;
	int winCount = 0, lossCount = 0, blankCount = 0;

	while(true){

		createChallengeSet(animalBank, challengeSet);

		if(challengeSet.size() == 1){
			cout << "What is " << challengeSet.size() << " animal in " << "\"" << createOutput(challengeSet, blankCount) << "\""<< "?" << endl;
		}
		else{
			cout << "What are " << challengeSet.size() << " animals in " << "\"" << createOutput(challengeSet, blankCount) << "\""<< "?" << endl;
		}

		vector<string>userInput;

		while(userInput.size() != challengeSet.size() && guess != "quit"){
			getline(cin, guess);
			if(guess == "quit")continue;
			else if(guess == "?"){
				outputAnswers(challengeSet);
				break;
			}
			foundNonAlpha(guess);
			createWords(userInput, guess);
			if(userInput.size() != challengeSet.size()){
				cout << "Your number of input is incorrect. Enter again: " << endl;
				userInput.clear();
			}
		}

		if(guess == "quit") break;

		if(winRound(userInput, challengeSet) == true){
			cout << "Yes!" << endl;
			++ winCount;
			lossCount = 0;
			if(winCount == 2){
				cout << "Succeeded two consecutive times, challenge goes up!" << endl;
				++blankCount;
			}
		}
		else if(winRound(userInput, challengeSet) == false){
			cout << "Nope!" << endl;
			++ lossCount;
			winCount = 0;
			if(lossCount == 2){
				cout << "Missed two consecutive times, challenge goes down!" << endl;
				--blankCount;
				if(blankCount < 0) blankCount = 0;
			}
		}
		//resets
		challengeSet.clear();
		userInput.clear();
		if(winCount >= 2) winCount = 0;
		else if(lossCount >= 2) lossCount = 0;
	}
	cout << "Bye..." << endl;
	return 0;
}



