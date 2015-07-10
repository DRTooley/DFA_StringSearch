
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <random>
#include <ctime>
#include <chrono>

using namespace std;

int RecursiveFind(char letter, string findPattern, string previous){
	int LettersBack = findPattern.rfind(letter);
	if (LettersBack < 0)
		return 0;
	string NewStr = previous.substr(previous.length() - LettersBack);
	string ComparePattern = findPattern.substr(0, LettersBack);
	bool Flag = (ComparePattern == NewStr);

	if (Flag)
		return NewStr.length() + 1;

	string NewPattern = previous.substr(0, LettersBack);
	return RecursiveFind(letter, NewPattern, previous);
}

void CreateDFA(vector<vector<int>> &myDFA, vector<char> Lang, string srchstr){
	vector<vector<int>> EmptyDFA;
	myDFA = EmptyDFA;
	int srchLen = srchstr.length();
	vector<int> pushed;
	for (int j = 0; j < Lang.size(); j++)
		pushed.push_back(0);
	for (int i = 0; i <= srchLen; i++) 
		myDFA.push_back(pushed);

	for (int k = 0; k < myDFA.size(); k++){ // for every state of the automata
		for (int l = 0; l < Lang.size();l++){ // Find where each letter goes in each state
			if (k < srchLen){
				if (Lang[l] == srchstr[k]){ // If it is the next character in the search string
					myDFA[k][l] = k + 1;
				}
			}
			if (myDFA[k][l] != k + 1){
				string PreviousLetters = srchstr.substr(0, k);
				myDFA[k][l] = RecursiveFind(Lang[l], PreviousLetters, PreviousLetters);
			}
		}
	}
}

int SearchText(vector<vector<int>> DFA, string text, map<char,int> L){
	int state = 0;
	int count = 0;
	for (auto character : text){
		int NextStateFinder = L.find(character)->second;
		state = DFA[state][NextStateFinder];
		if (state == DFA.size()-1)
			count++;
	}
	return count;
}

void PrintDFA(vector<vector<int>> DFA, vector<char> L){
	cout << "State |";
	for (auto character : L){
		cout << " " << character << " |";
	}
	cout << endl << "------";
	for (auto character : L)
		cout << "----";
	cout << endl;

	for (int i = 0; i < DFA.size(); i++){
		if ( i<10)
			cout << "  "<<i << "   |";
		else
			cout <<"  "<< i << "  |";
		for (auto state:DFA[i]){
			if (state < 10)
				cout << " " << state << " |";
			else
				cout << " " << state << "|";
		}
		cout << endl;
	}
	cout << endl;
}

string GenerateRandText(int length, map<int, char> RLM){
	string text = "";
	for (int i = 0; i < length; i++){
		unsigned int randNum = rand() % 4;
		char RandLetter = RLM.find(randNum)->second;
		text = text + RandLetter;
	}
	return text;
}

void LargeSearch(map<int, char> RLM,map<char,int> LM, vector<char> Language, vector<vector<int>> DFA, unsigned int length ){
	cout << "Running Test for text length of " << length << "." << endl << endl;
	string text = "";
	string search = "";
	vector<clock_t> StartTimes;
	vector<clock_t> EndTimes;
	int SearchCount;
	for (int i = 0; i < 10; i++){
		text = GenerateRandText(length, RLM);
		search = text.substr(text.length() - 50);
		CreateDFA(DFA, Language, search);
		
		StartTimes.push_back(clock());
		SearchCount = SearchText(DFA, text, LM);
		EndTimes.push_back(clock());

		//cout << "There were " << SearchCount << " matche(s) found." << endl
			//<< "This Took " << ((float)EndTimes[i] - (float)StartTimes[i])/CLOCKS_PER_SEC << " seconds" << endl << endl;
	}
	float AvgTime =0;
	for (int i = 0; i < 10; i++){
		AvgTime = AvgTime + (float)EndTimes[i] - (float)StartTimes[i];
	}
	AvgTime = AvgTime / (10*CLOCKS_PER_SEC);
	cout << "The average time the search of " << length << " characters took: " << AvgTime << " seconds." << endl << endl;
}
int main(int argc, char * argv[]) {
	vector<char> Language = { 'G', 'A', 'T', 'C' };
	map<char, int> LangMap = { { 'G', 0 }, { 'A', 1 }, { 'T', 2 }, { 'C', 3 } };
	map<int, char> ReverseLangMap = { { 0,'G'}, {1, 'A' }, {2, 'T'}, {3, 'C'} };
	vector<vector<int>> DFA;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	srand(time(NULL));

	string search = "";
	string text = "";
	int SearchCount;

	cout << "Generating Test 1: Correct DFA Table" << endl << endl;

	search = "GACGA";
	CreateDFA(DFA, Language, search);
	PrintDFA(DFA, Language);

	search = "AAGCATTTAAGCA";
	CreateDFA(DFA, Language, search);
	PrintDFA(DFA, Language);

	cout << "Generating Test 2: Instance of Subsets" << endl << endl;

	//Create random 100 char string
	text = GenerateRandText(100, ReverseLangMap);
	cout << "Your randomly generated text is :" << endl << endl << text << endl << endl;

	for (int i = 3; i <= 10; i++){
		search = text.substr(0, i);
		CreateDFA(DFA, Language, search);
		SearchCount = SearchText(DFA, text, LangMap);
		cout << "For the substring \"" << search << "\" There were " << SearchCount << " matche(s) found." << endl;
	}
	cout << endl << "Generating Test 3: Test of Time" << endl << endl;
	LargeSearch(ReverseLangMap, LangMap, Language, DFA, 10000);
	LargeSearch(ReverseLangMap, LangMap, Language, DFA, 20000);
		
}