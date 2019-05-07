/*
* CS 106B Boggle
* a Boggle class representing the Boggle game state
*
* Author : Elyas Esmaeili
*/

#include "Boggle.h"
#include "bogglegui.h"
#include "shuffle.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Boggle::Boggle(Lexicon& dictionary, string boardText) {

    game_board = Grid<cube> (4, 4);
    this->dictionary = dictionary;

    if (!boardText.length()) {
        boardText = generateRndmText();
    }

    for (int i = 0; i < game_board.numRows(); i++) {
        for (int j = 0; j < game_board.numCols(); j++) {
            game_board[i][j].up_letter = tolower(boardText[4 * i + j]);
            BoggleGUI::labelCube(i, j, toupper(game_board[i][j].up_letter));
        }
    }
}

char Boggle::getLetter(int row, int col) {
    if (!game_board.inBounds(row, col)) {
        throw (-1);
    }

    return game_board[row][col].up_letter;
}

bool Boggle::checkWord(string word) {
    for (auto& chr : word) {
        tolower(chr);
    }

    if (word.length() < 4 || !dictionary.contains(word) ||
            humanWords.count(word)) {
        return false;
    }

    return true;
}

bool Boggle::humanWordSearch(string word) {
	for (auto& chr : word) {
        chr = tolower(chr);
	}

    BoggleGUI::clearHighlighting();

	if (checkWord(word)) {
		for (int i = 0; i < game_board.numRows(); i++) {
			for (int j = 0; j < game_board.numCols(); j++) {

				if (humanWordSearchRecursive(word, i, j, 0)) {

					humanWords.insert(word);
					humanScore += (word.length() - 3);

					BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
					BoggleGUI::setScore(humanScore, BoggleGUI::HUMAN);

					return true;
				}

			}
		}
	}
	return false;   // remove this
}

int Boggle::getScoreHuman() {
    return humanScore;
}

set<string> Boggle::computerWordSearch() {
    set<string> result;

    for (int i = 0; i < game_board.numRows(); i++) {
        for (int j = 0; j < game_board.numCols(); j++) {

            game_board[i][j].is_visited = true;
            computerWordSearchRecursive(result, string(1, game_board[i][j].up_letter), i, j);
            game_board[i][j].is_visited = false;
        }
    }

    for (auto& word : result) {
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }

    BoggleGUI::setScore(computerScore, BoggleGUI::COMPUTER);


    return result;
}

int Boggle::getScoreComputer() {
    return computerScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int i = 0; i < boggle.game_board.numRows(); i++) {
        for (int j = 0; j < boggle.game_board.numCols(); j++) {
            cout << boggle.game_board[i][j].is_visited;
        }
        cout << endl;
    }

    return out;
}

string Boggle:: generateRndmText() {
    int cubeSize = 16;

    shuffle(CUBES, cubeSize);
    for (int i = 0; i < cubeSize; i++) {
        shuffle(CUBES[i]);
    }

    string txt;
    for (int i = 0; i < cubeSize; i++) {
        txt += CUBES[i][0];
    }

    return txt;
}

bool Boggle::humanWordSearchRecursive(string word, int row, int col, int index)
{
	game_board[row][col].is_visited = true;
	BoggleGUI::setHighlighted(row, col);
    BoggleGUI::setAnimationDelay(100);

	if (word[index] != game_board[row][col].up_letter) {
		game_board[row][col].is_visited = false;
		BoggleGUI::setHighlighted(row, col, false);

        return false;
	}

    if (word.length() - 1 == index) {
        game_board[row][col].is_visited = false;
        return true;
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newRow = row + i;
            int newCol = col + j;

            if (game_board.inBounds(newRow, newCol) &&
                    !game_board[newRow][newCol].is_visited) {

                if (humanWordSearchRecursive(word, newRow, newCol, index + 1)) {
					game_board[row][col].is_visited = false;
                    return true;
                }
            }
        }
    }
    game_board[row][col].is_visited = false;
    BoggleGUI::setHighlighted(row, col, false);
    return false;
}

void Boggle::computerWordSearchRecursive(set<string>& result, string word, int row, int col)
{
    if (!dictionary.containsPrefix(word)) {
        return;
    }

    if (dictionary.contains(word) && word.size() >= 4) {
        if (!humanWords.count(word)) {
            auto iter = result.insert(word);
            if(iter.second) {
                computerScore += (word.length() - 3);
            }
        }
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newRow = row + i;
            int newCol = col + j;

            if (game_board.inBounds(newRow, newCol) &&
                    !game_board[newRow][newCol].is_visited) {

                 game_board[newRow][newCol].is_visited = true;

                 computerWordSearchRecursive(result, word + game_board[newRow][newCol].up_letter,
                                            newRow, newCol);

                 game_board[newRow][newCol].is_visited = false;
            }
        }
    }
}

set<string> Boggle::getHumanWords()
{
    return humanWords;
}
