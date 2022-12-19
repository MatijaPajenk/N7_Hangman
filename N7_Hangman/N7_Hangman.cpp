#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>

typedef std::vector<std::string> vStr;

struct History {
	int guesses = 0;
	int time = 0;
};

struct Player
{
	std::string name = "";
	int id = 0;
	std::string current_guess = "";
	History history;
	Player* next = nullptr;
};

void printGuessedCharacters(std::string& guessed_ch) {
	std::cout << "Already guessed characters: ";
	for (int i = 0; i < guessed_ch.length(); i++) {
		std::cout << guessed_ch[i] << ", ";
	}
	std::cout << '\n';
}

bool checkChInWord(std::string& word, char ch) {
	for (int i = 0; i < word.length(); i++)
		if (word[i] == ch) return false;
	return true;
}

void printPlayer(Player* p) {
	std::cout << "| " << p->id << " | " << p->name << " | " << p->current_guess << " | " << p->history.guesses << " | " << p->history.time << " | " << (p->next != nullptr ? std::to_string(p->next->id) : "Null") << " |\n";
}

void writeAsHTML(Player* head) {
	std::ostringstream ss;

	ss << "<table border=\"1\"\n style=\"text-align:center;>\"";
	ss << "<thead>\n<tr>\n<th>ID</th>\n<th>Name</th>\n<th>Word</th>\n<th>Guesses</th>\n<th>Time</th>\n<th>Next</th>\n</tr>\n</thead>\n<tbody>\n";
	for (int i = 0; i < 5; i++) {
		ss << "<tr>\n";
		ss << "<td>" << head->id << "</td>\n";
		ss << "<td>" << head->name << "</td>\n";
		ss << "<td>" << head->current_guess << "</td>\n";
		ss << "<td>" << head->history.guesses << "</td>\n";
		ss << "<td>" << head->history.time << " s</td>\n";
		ss << "<td>" << (head->next != nullptr ? std::to_string(head->next->id) : "Null") << "</td>\n";
		ss << "</tr>\n";
		head = head->next;
	}
	ss << "</tbody>\n</table>";

	std::ofstream os;
	os.open("Results.html");
	os << ss.str();
	os.close();
}

void getFrames(std::string frames[]) {
	std::string path = "frames.txt";

	std::ifstream file;

	file.open(path);
	if (file.is_open()) {
		std::string line;
		int id = 0;
		while (!file.eof()) {
			std::getline(file, line);
			if (line == ";") id++;
			else {
				line += "\n";
				frames[id] += line;
			}
			//std::cout << line << '\n';
		}
	}
	file.close();
}

void insertPlayer(Player*& head, Player* p, const int limit) {
	if (head == nullptr) {
		head = p;
		return;
	}

	Player* curr = head;
	Player* previous = nullptr;
	while (curr != nullptr && curr->history.guesses < p->history.guesses) {
		previous = curr;
		curr = curr->next;
	}

	if (previous == nullptr) {
		p->next = head;
		head = p;
	}
	else {
		p->next = curr;
		previous->next = p;
	}

	int c = 0;
	curr = head;
	while (curr != nullptr) {
		c++;
		curr = curr->next;
	}
	if (c > limit) {
		curr = head;
		previous = nullptr;
		while (curr->next != nullptr) {
			previous = curr;
			curr = curr->next;
		}
		delete curr;
		previous->next = nullptr;
	}

	//std::cout << "Curr ID: " << curr->id << "; Curr V: " << curr->history.guesses << "\nCurr->Next ID: " << p->id << "; Curr->Next V: " << p->history.guesses << '\n';
}

int main() {
	// setup
	const int FRAMES = 10;
	const int BEST = 5;
	const int NUM_OF_PLAYERS = 10;

	int l = 0;
	do {
		std::cout << "Word length (bounds 3-11): ";
		std::cin >> l;
	} while (l < 3 || l > 11);

	std::string frames[10];
	getFrames(frames);

	vStr words;
	const char* path = "words.txt";
	std::ifstream file;

	file.open(path);
	if (file.is_open()) {
		//std::cout << "Open\n";
		std::string line;
		while (file >> line) {
			if (line.length() == l) {
				//std::cout << line << '\n';
				words.push_back(line);
			}
		}
	}
	file.close();

	// gameplay
	bool win = false;
	std::string guess = "", guessed_ch = "";
	char ch;
	int choice, wrong_guesses, correct_guesses;
	clock_t start;

	Player* head = nullptr;
	Player* p;
	Player* index = head;


	for (int i = 0; i < NUM_OF_PLAYERS; i++) {
		win = false;
		guessed_ch = "";
		wrong_guesses = 0;
		correct_guesses = 0;
		srand(time(nullptr));

		p = new Player;

		//if (i == 0) {
		//	head = p;
		//	index = head;
		//}

		std::cout << "Name: ";
		std::cin >> p->name;
		p->id = i + 1;

		p->current_guess = words[rand() % words.size()];
		start = clock();

		do {
			//Testing purposes only
			std::cout << p->current_guess << '\n';

			for (int j = 0; j < p->current_guess.length(); j++) {
				if (!checkChInWord(guessed_ch, p->current_guess[j]))
					std::cout << p->current_guess[j];
				else
					std::cout << '_';
			}
			std::cout << '\n';

			std::cout << frames[wrong_guesses] << '\n';
			printGuessedCharacters(guessed_ch);

			std::cout << "\n1 - Guess word\n2 - Guess character\n";

			std::cin >> choice;
			if (std::cin.fail())
			{
				std::cout << "Wrong input!!!\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
			}

			switch (choice) {
			case 1:
				std::cout << "Guess word: ";
				p->history.guesses++;
				std::cin >> guess;
				if (guess == p->current_guess) {
					win = true;
				}
				else {
					wrong_guesses++;
				}
				break;
			case 2:
				std::cout << "Guess character: ";
				std::cin >> ch;
				if (!checkChInWord(guessed_ch, ch)) {
					std::cout << "Character already guessed!!!";
					continue;
				}
				p->history.guesses++;
				guessed_ch += ch;
				if (checkChInWord(p->current_guess, ch)) {
					wrong_guesses++;
					break;
				}
				correct_guesses++;
				if (correct_guesses == p->current_guess.length()) {
					win = true;
				}
				break;
			default:
				std::cout << "Wrong input\n";
				break;
			}

			std::cout << '\n';
		} while (!win && wrong_guesses < 10);

		p->history.time = (int)(clock() - start) / CLOCKS_PER_SEC;

		if (win) {
			std::cout << "Congratilations!!! ";
		}
		else {
			std::cout << "Too bad. ";
		}
		std::cout << "It took you " << p->history.guesses << " guesses and " << p->history.time << " seconds.\n";

		insertPlayer(head, p, BEST);
		
	}

	// TODO sort players
	//sortPlayers(p);
	std::cout << '\n';

	//std::cout << "Head: " << head << '\n';
	for (p = head; p != nullptr; p = p->next) {
		std::cout << "ID: " << p->id << " ; Name: " << p->name << " ;Guesses: " << p->history.guesses << '\n';
	}

	writeAsHTML(head);

	while (head != nullptr) {
		Player* next = head->next;
		delete head;
		head = next;
	}

	return 0;
}


