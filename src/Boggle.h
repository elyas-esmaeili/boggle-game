/*
* CS 106B Boggle
* a Boggle class representing the Boggle game state
*
* Author : Elyas Esmaeili
*/

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <vector>
#include <set>
#include <string>

#include "lexicon.h"
#include "grid.h"

using namespace std;

struct cube {
    char up_letter;
    bool is_visited = false;
};

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    set<string> getHumanWords();

    // TODO: add any other member functions/variables necessary
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    string generateRndmText();
    bool humanWordSearchRecursive(string word, int row, int col, int index);
    void computerWordSearchRecursive(set<string>& result, string word, int row, int col);

private:
    Lexicon dictionary;
    Grid<cube> game_board;
    set<string> humanWords;
    int humanScore = 0;
    int computerScore = 0;
};

#endif // _boggle_h
