#include <iostream>
#include <Windows.h>
using namespace std;

struct zmey {
	zmey* prev;
	int x_g;
	int y_g;
	zmey* next;
};

void plus_length(zmey** tek, int x_0, int y_0, int& len) {
	zmey* q = new zmey();
	q->x_g = x_0;
	q->y_g = y_0;
	if (*tek == NULL) {
		*tek = q;
	}
	else {
		(*tek)->next = q;
		q->prev = *tek;
		*tek = q;
	}
	len++;
}

class Graph {
private:
	int rows;
	int colums;
	int* visited;
	bool* zan_k;
public:
	Graph(int r, int c, int x_0, int y_0) {
		rows = r;
		colums = c;
		visited = new int[r * c] {};
		zan_k = new bool[r * c] {};
		visited[0] = y_0 * colums + x_0;
		zan_k[0] = 1;
	}

	void append(int value) {
		int i = 0;
		while (zan_k[i]) i++;
		visited[i] = value;
		zan_k[i] = true;
	}
	void remove(int value) {
		int i = 0;
		while (visited[i] != value) i++;
		visited[i] = 0;
		zan_k[i] = false;
	}
	bool contains(int value) {
		for (int i = 0; i < rows * colums; i++) {
			if (visited[i] == value and zan_k) return true;
		}
		return false;
	}

	bool gamilton(int x_0, int y_0)
	{
		int n = rows * colums;
		/*gamilton(int x_0, int y_0) вместо k будем смотреть каждую итерацию последний элемент visited
		если он не мусор, то смотрим окрестность первой точки, если в ней есть последняя точка то возвращаем true
		убрать таблицу смежности и цикл и
		создать цикл с 4 switch, где 0 - вверхнее направление, 1 - правое, 2 - нижнее, 3 - левое, для каждой соседней точки
		после добавить новую точку в visited, если она находится в пределах поля и её в массиве нет,
		и запустить следующий шаг gamilton от этой точки, где x_0 = visited[i] % colums, y_0 = visited[i] / colums
		если последний вызов функии вернул 1, то возвращем 1 в остальных вызовах
		если вернул 0, то visited[i] = -1
		после цикла возвращаем false, потому что дальше из этой точки следовать нельзя
		*/
		if (zan_k[n - 1]) {
			return (visited[0] % colums == (visited[n - 1] % colums + 1) && visited[0] / colums == visited[n - 1] / colums) ||
				(visited[0] % colums == (visited[n - 1] % colums - 1) && visited[0] / colums == visited[n - 1] / colums) ||
				(visited[0] % colums == visited[n - 1] % colums && visited[0] / colums == (visited[n - 1] / colums + 1)) ||
				(visited[0] % colums == visited[n - 1] % colums && visited[0] / colums == (visited[n - 1] / colums - 1));
		}
		int n_x = 0, n_y = 0, numer = 0;
		for (int i = 0; i < 4; i++) {
			int n_x, n_y;
			switch (i) {
			case 0:
				n_y = y_0 - 1;
				n_x = x_0;
				break;
			case 1:
				n_y = y_0;
				n_x = x_0 + 1;
				break;
			case 2:
				n_y = y_0 + 1;
				n_x = x_0;
				break;
			case 3:
				n_y = y_0;
				n_x = x_0 - 1;
				break;
			}
			numer = n_y * colums + n_x;
			//cout << "new_x = " << n_x << "\tnew_y = " << n_y << "\tnumer = " << numer << endl;
			if (0 <= n_x && n_x < colums && 0 <= n_y && n_y < rows && !contains(numer)) {
				append(numer);
				if (gamilton(n_x, n_y)) {
					return true;
				}
				remove(numer);
			}
		}
		return false;
	}
	int CreateYab(zmey* tek, int len) {
		bool sost = 1;
		int numer = -1;
		if (len == rows * colums) return -1;
		while (sost) {
			sost = 0;
			zmey* q = tek;
			numer = rand() % (rows * colums);
			while (q) {
				int old_numer = (q->y_g) * colums + (q->x_g);
				if (old_numer == numer) {
					sost = 1;
					break;
				}
				q = q->next;
			}
		}
		return numer;
	}

	void show_yab(int numer) {
		COORD POS;
		POS.X = numer % colums * 2; POS.Y = numer / colums;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), POS);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 68);
		cout << "  ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);
	}

	bool move(zmey* gol, int& numer, int& len) {
		if (len == rows * colums) return false;
		int i = 0;
		int new_numer = (gol->y_g) * colums + gol->x_g;
		while (visited[i] != new_numer && i < rows * colums) i++;
		i++;
		int new_y = visited[i % (rows * colums)] / colums, new_x = visited[i % (rows * colums)] % colums;
		zmey* q = gol, * h = NULL;
		while (q) {
			int zam = q->x_g;
			q->x_g = new_x;
			new_x = zam;
			zam = q->y_g;
			q->y_g = new_y;
			new_y = zam;
			if (!q->next) h = q;
			q = q->next;
		}
		if ((gol->y_g) * colums + (gol->x_g) == numer) {
			i = 0;
			while (visited[i] != (h->y_g) * colums + h->x_g && i < colums * rows) i++;
			plus_length(&h, visited[i - 1] % colums, visited[i - 1] / colums, len);
			numer = CreateYab(gol, len);
			show_yab(numer);
		}
		return true;
	}
	void show_pole(zmey* tek, int x_ya, int y_ya, const bool show = false) {
		HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD POS;
		if (show) {
			for (int y = 0; y < rows; y++) {
				for (int x = 0; x < colums; x++) {
					POS.X = x * 2; POS.Y = y;
					SetConsoleCursorPosition(hWnd, POS);
					SetConsoleTextAttribute(hWnd, 255);
					cout << "  ";
				}
			}
		}
		//написать изменение поля, а не перепечатку
		zmey* qur = tek;
		while (qur) {
			POS.X = (qur->x_g) * 2; POS.Y = qur->y_g;
			SetConsoleCursorPosition(hWnd, POS);
			if (!qur->prev) {
				SetConsoleTextAttribute(hWnd, 17);
			}
			else {
				SetConsoleTextAttribute(hWnd, 33);
			}
			cout << "  ";
			SetConsoleTextAttribute(hWnd, 0);
			if (!qur->next && !show) {
				int i = 0;
				while (visited[i] != (qur->y_g) * colums + qur->x_g && i < colums * rows) i++;
				i--;
				if (i >= 0) { POS.X = visited[i] % colums; POS.Y = visited[i] / colums; }
				else {
					POS.X = visited[colums * rows - 1] % colums; POS.Y = visited[colums * rows - 1] / colums;
				}
				if (!(POS.X == tek->x_g && POS.Y == tek->y_g) && !(POS.X == x_ya && POS.Y == y_ya)) {
					POS.X *= 2;
					SetConsoleCursorPosition(hWnd, POS);
					SetConsoleTextAttribute(hWnd, 255);
					cout << "  ";
					SetConsoleTextAttribute(hWnd, 0);
				}
			}
			qur = qur->next;
		}
		Sleep(5);
	}
};

int main() {
	srand(time(0));
	const int rows = 20, colums = 20;
	int len = 0;
	zmey* gol = NULL;
	plus_length(&gol, 0, 0, len);
	Graph g(rows, colums, 0, 0);
	g.gamilton(0, 0);
	int numer = g.CreateYab(gol, len);
	g.show_pole(gol, numer % colums, numer / colums, 1);
	g.show_yab(numer);
	while (g.move(gol, numer, len)) {
		g.show_pole(gol, numer % colums, numer / colums);
	}
	/*g.clear_visit();
	g.visit_tchk(gol->x_g, gol->y_g);
	g.gamilton(1, h->x_g, h->y_g);*/
	g.show_pole(gol, 0, 0);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 0, rows + 1 });
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0 << 4 | 0xf);
}
