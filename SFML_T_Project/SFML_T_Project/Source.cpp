#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <thread>

using namespace sf;
using namespace std;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };
int w = 34;

struct Point
{
	int x, y;
}a[4], b[4];

int figures[7][4] = {
	1,3,5,7,
	2,4,5,7,
	3,5,4,6,
	3,5,4,7,
	2,3,5,7,
	3,5,7,6,
	2,3,4,5,
};

class User {
protected:
	string Name;
	string GameNickname;
	string Path = "TableOfRecords.txt";

public:
	User() {

	}

	User(string _Name, string _GameNickname) {
		Name = _Name;
		GameNickname = _GameNickname;
	}

	void Display() {
		cout << "Имя: " << Name << ". Игровой никнейм: " << GameNickname << endl;
	}
};

class UserSave : User {
protected:
	double Record;
	string N;
	string GN;

public:
	UserSave() {

	}

	UserSave(double _Record, string _N, string _GN) {
		Record = _Record;
		N = _N;
		GN = _GN;
	}

	void Save() {
		ofstream TOR;
		TOR.open(Path, ofstream::app);

		if (!TOR.is_open()) {
			cout << "!ERROR!" << endl << "File can`t be open!!!" << endl;
		}
		else {
			TOR << "Имя игрока: " << N << ". Игровой никнейм игрока: " << GN << ". Рекорд игрока: " << Record << endl;
		}

		TOR.close();
	}
};

bool check() {
	for (int i = 0; i < 4; i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
			return 0;
		else if (field[a[i].y][a[i].x])
			return 0;

	return 1;
}

int main()
{
	setlocale(LC_ALL, "ru");

	string _Name;
	string _GameNickname;

	double _RecordTimer = 0;

	cout << "Приветствую тебя мой дорогой игрок в игре TETRIS_Ultimate!" << endl << "Введи пожалуйста своё имя. ";
	cout << "Ваше имя: "; cin >> _Name; cout << endl;
	cout << "Теперь введи свой игровой ник пожалуйста." << endl;
	cout << "Ваш игровой ник: "; cin >> _GameNickname; cout << endl;

	User U(_Name, _GameNickname);

	U.Display();

	cout << "Благодарю тебя игрок! твои данные будут отображаться в таблице рекордов в корневом txt файле!" << endl;
	cout << "приятной игры тебе игрко! Или мне лучше тебя называть " << _Name << " ? Ладно, не будем ломать 4 стену." << endl;
	cout << "ИГРА НАЧИНАЕТСЯ! (для продолжения введите что угодно, что бы лать игре понять что вы готовы)" << endl;
	char KOstil; cin >> KOstil;

	srand(time(0));

	RenderWindow window(VideoMode(N * w, M * w), "Tetris!");

	Texture t;
	t.loadFromFile("D:/wpadpwdldalwfpwpf/SFML_T_Project/Paint/tiles.png");
	Sprite tiles(t);

	int dx = 0, colorNum = 1;
	bool rotate = false;
	float timer = 0, delay = 0.3;
	Clock clock;

	while (window.isOpen()) //корень
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Up)
					rotate = true;
				else if (event.key.code == Keyboard::Right)
					dx = 1;
				else if (event.key.code == Keyboard::Left)
					dx = -1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05;

		for (int i = 0; i < 4; i++) {
			b[i] = a[i];
			a[i].x += dx;
		}

		if (!check()) {
			for (int i = 0; i < 4; i++)
				a[i] = b[i];
		}

		if (rotate) {
			Point p = a[1];
			for (int i = 0; i < 4; i++) {
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;

				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}

			if (!check()) {
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
			}
		}

		if (timer > delay) {
			for (int i = 0; i < 4; i++) {
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!check()) {
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;
				colorNum = 1 + rand() % 7;

				int n = rand() % 7;
				for (int i = 0; i < 4; i++) {
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}

		int k = M - 1;
		for (int i = M - 1; i > 0; i--) {
			int count = 0;
			for (int j = 0; j < N; j++) {
				if (field[i][j]) {
					count++;
				}
				field[k][j] = field[i][j];
			}

			if (count < N) {
				k--;
			}

		}

		dx = 0;
		rotate = false;
		delay = 0.3;

		window.clear(Color::Black);

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)  {
				if (field[i][j] == 0) {
					continue;
				}
				tiles.setTextureRect(IntRect(field[i][j] * w, 0, w, w));
				tiles.setPosition(j * w, i * w);
				window.draw(tiles);
			}

		for (int i = 0; i < 4; i++) {
			tiles.setTextureRect(IntRect(colorNum * w, 0, w, w));
			tiles.setPosition(a[i].x * w, a[i].y * w);
			window.draw(tiles);
		}

		_RecordTimer += 1;

		window.display();

	}

	UserSave US(_RecordTimer, _Name, _GameNickname);

	US.Save();

	return 0;
}