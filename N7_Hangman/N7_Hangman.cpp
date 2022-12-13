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

void writeAsHTML(Player p[], const int n) {
	std::ostringstream ss;

	ss << "<table border=\"1\"\n style=\"text-align:center;>\"";
	ss << "<thead>\n<tr>\n<th>ID</th>\n<th>Name</th>\n<th>Word</th>\n<th>Guesses</th>\n<th>Time</th>\n<th>Next</th>\n</tr>\n</thead>\n<tbody>\n";
	for (int i = 0; i < n; i++) {
		ss << "<tr>\n";
		ss << "<td>" << p[i].id << "</td>\n";
		ss << "<td>" << p[i].name << "</td>\n";
		ss << "<td>" << p[i].current_guess << "</td>\n";
		ss << "<td>" << p[i].history.guesses << "</td>\n";
		ss << "<td>" << p[i].history.time << " s</td>\n";
		ss << "<td>" << (p[i].next != nullptr ? std::to_string(p[i].next->id) : "Null") << "</td>\n";
		ss << "</tr>\n";
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

void unused() {
	const char* words[]{
		"time",
		"year",
		"people",
		"way",
		"day",
		"man",
		"thing",
		"woman",
		"life",
		"child",
		"world",
		"school",
		"state",
		"family",
		"student",
		"group",
		"country",
		"problem",
		"hand",
		"part",
		"place",
		"case",
		"week",
		"company",
		"system",
		"program",
		"question",
		"work",
		"government",
		"number",
		"night",
		"point",
		"home",
		"water",
		"room",
		"mother",
		"area",
		"money",
		"story",
		"fact",
		"month",
		"lot",
		"right",
		"study",
		"book",
		"eye",
		"job",
		"word",
		"business",
		"issue",
		"side",
		"kind",
		"head",
		"house",
		"service",
		"friend",
		"father",
		"power",
		"hour",
		"game",
		"line",
		"end",
		"member",
		"law",
		"car",
		"city",
		"community",
		"name",
		"president",
		"team",
		"minute",
		"idea",
		"kid",
		"body",
		"information",
		"back",
		"parent",
		"face",
		"others",
		"level",
		"office",
		"door",
		"health",
		"person",
		"art",
		"war",
		"history",
		"party",
		"result",
		"change",
		"morning",
		"reason",
		"research",
		"girl",
		"guy",
		"moment",
		"air",
		"teacher",
		"force",
		"education"
	};


	const char* frames[]{
		"+----------+\n|          |\n|          |\n|          |\n|          |\n|          |\n|   /|\\    |\n+----------+",
		"+----------+\n|          |\n|          |\n|          |\n|          |\n|    |     |\n|   /|\\    |\n+----------+",
		"+----------+\n|          |\n|          |\n|          |\n|    |     |\n|    |     |\n|   /|\\    |\n+----------+",
		"+----------+\n|          |\n|          |\n|    |     |\n|    |     |\n|    |     |\n|   /|\\    |\n+----------+",
		"+----------+\n|    +     |\n|    |     |\n|    |     |\n|    |     |\n|    |     |\n|   /|\\    |\n+----------+",
		"+----------+\n|    +--+  |\n|    |     |\n|    |     |\n|    |     |\n|    |     |\n|   /|\\    |\n+----------+",
		"+----------+\n|    +--+  |\n|    |  |  |\n|    |     |\n|    |     |\n|    |     |\n|   /|\\    |\n+----------+",
		"+----------+\n|    +--+  |\n|    |  |  |\n|    |  o  |\n|    |     |\n|    |     |\n|   /|\\    |\n+----------+",
		"+----------+\n|    +--+  |\n|    |  |  |\n|    |  o  |\n|    | -+- |\n|    |     |\n|   /|\\    |\n+----------+",
		"+----------+\n|    +--+  |\n|    |  |  |\n|    |  o  |\n|    | -+- |\n|    | / \\ |\n|   /|\\    |\n+----------+",
	};
}

int main() {
	const int FRAMES = 10;
	const int BEST = 5;
	const int PNUM = 10;

	int l;
	do {
		std::cout << "Word length (bounds 3-11): ";
		std::cin >> l;
	} while (l<3 || l > 11);

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
	
	
	bool win = false;
	std::string guess = "", guessed_ch = "";
	char ch;
	int choice, wrong_guesses, correct_guesses;
	clock_t start;

	Player p[PNUM];

	for (int i = 0; i < PNUM; i++) {
		win = false;
		p[i].history.guesses = 0;
		guessed_ch = "";
		wrong_guesses = 0;
		correct_guesses = 0;
		srand(time(nullptr));

		std::cout << "Name: ";
		std::cin >> p[i].name;
		p[i].id = i + 1;

		p[i].current_guess = words[rand() % words.size()];
		start = clock();

		if (i < PNUM - 1) {
			p[i].next = &p[i + 1];
		}

		do {
			//Testing purposes only
			std::cout << p[i].current_guess << '\n';

			for (int j = 0; j < p[i].current_guess.length(); j++) {
				if (!checkChInWord(guessed_ch, p[i].current_guess[j]))
					std::cout << p[i].current_guess[j];
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
				p[i].history.guesses++;
				std::cin >> guess;
				if (guess == p[i].current_guess) {
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
				p[i].history.guesses++;
				guessed_ch += ch;
				if (checkChInWord(p[i].current_guess, ch)) {
					wrong_guesses++;
					break;
				}
				correct_guesses++;
				if (correct_guesses == p[i].current_guess.length()) {
					win = true;
				}

				break;

			default:
				std::cout << "Wrong input\n";
				break;
			}
			std::cout << '\n';
		} while (!win && wrong_guesses < 10);

		p[i].history.time = (int)(clock() - start) / CLOCKS_PER_SEC;

		if (win) {
			std::cout << "Congratilations!!! ";
		}
		else {
			std::cout << "Too bad. ";
		}
		std::cout << "It took you " << p[i].history.guesses << " guesses and " << p[i].history.time << " seconds.\n";

	}


	for (int i = 0; i < PNUM; i++) {
		for (int j = 1; j < PNUM; j++) {
			if (p[j].history.guesses < p[j - 1].history.guesses) {
				std::swap(p[j], p[j - 1]);
			}
		}
	}

    

	for (int i = 0; i < BEST; i++) {
		if (i < BEST-1) {
			p[i].next = &p[i + 1];
		}
		else {
			p[i].next = nullptr;
		}
	}

	std::cout << "| ID | Name | Word | Guesses | Time | Next |\n";
	for (int i = 0; i < BEST; i++) {
		printPlayer(&p[i]);
	}

	//std::cout << "Checking next element id:\n";
	//for (int i = 0; i < 3; i++) {
	//	std::cout << p[i].id << ": " << (p[i].next != nullptr ? p[i].next->id : 69) << "\n";
	//}



	std::cout << '\n';
	writeAsHTML(p, BEST);
}


