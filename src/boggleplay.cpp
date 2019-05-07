/*
* CS 106B Boggle
* client to perform console UI and work with Boggle class
*
* Author : Elyas Esmaeili
*/

#include "lexicon.h"
#include "Boggle.h"
#include "bogglegui.h"
#include "simpio.h"
#include "console.h"

string getBoardTxt();
bool validateInput(string str);
void humanTurns(Boggle& boggle);
void computerTurns(Boggle& boggle);

void playOneGame(Lexicon& dictionary) {
	
	if (BoggleGUI::isInitialized()) {
		BoggleGUI::reset();
	}
	else {
		BoggleGUI::initialize(4, 4);
	}

    string boardTxt = getBoardTxt();
    Boggle boggle(dictionary, boardTxt);

    humanTurns(boggle);
    computerTurns(boggle);
}

void computerTurns(Boggle& boggle)
{
    cout << endl << "It's my turn!" << endl;

    set<string> words = boggle.computerWordSearch();
    cout << "My words " << "(" << words.size() << "): {";
    for (auto& wrd : words) {
        cout << "\"" << wrd << "\", ";
    }
    cout << "}" << endl;

    cout << "My Score: " << boggle.getScoreComputer() << endl;
    if (boggle.getScoreComputer() > boggle.getScoreHuman()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    }
}
void humanTurns(Boggle& boggle)
{
	cout << "It's your turn." << endl;
	string word;

    while(true)
    {
        cout << boggle << endl << endl;

        set<string> humanWords = boggle.getHumanWords();
		cout << "Your words " << "(" << humanWords.size() << "): {";
		for (auto& wrd : humanWords) {
			cout << "\"" << wrd << "\", ";
		}
        cout << "}" << endl;

        cout << "Your score: " << boggle.getScoreHuman() << endl;
		word = getLine("Type a word (or Enter to stop):");
        clearConsole();

        if (word == "") {
            break;
        }

		if (boggle.humanWordSearch(word)) {
			cout << "You found a new word! \"" << word << "\"" << endl;
		}
		else {
			cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
		}
    }
}
string getBoardTxt()
{
    string boardTxt;
    if (getYesOrNo("Do you want to generate a random board? ")) {
        boardTxt = "";
    }
    else {
        while (true) {
            boardTxt = getLine("Type the 16 letters to appear on the board:");
            if (validateInput(boardTxt)) {
                break;
            }
            cout << "That is not a valid 16-letter board string. Try again." << endl;;
        }
    }

    return boardTxt;
}

bool validateInput(string str)
{
    for (auto& chr : str) {
        if (!isalpha(chr)) {
            return false;
        }
    }

    if (str.size() != 16) {
        return false;
    }

    return true;
}
