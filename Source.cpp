#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 
using namespace std;

HANDLE hStdOut;
CONSOLE_SCREEN_BUFFER_INFO csbInfo;
SMALL_RECT consolRect;
HDC hdc;
HWND hwnd;
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27
#define TABLE_SIZE 7
struct Node {
	char word[30];
	char *info = new char[800];
	char *info1 = NULL;
	char *info2 = NULL;
	char *info3 = NULL;
	int points;
	Node *next;
};


enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};
void SetColor(int text, int background) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
WORD colorConvertTo(int ForgC, int BackC) {
	return ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
}
void removeDuplicates(Node* head)
{
	/* Pointer to traverse the linked list */
	Node* current = head;

	/* Pointer to store the next pointer of a node to be deleted*/
	Node* next_next;

	/* do nothing if the list is empty */
	if (current == NULL) {
		return;
	}

	/* Traverse the list till last node */
	while (current->next != NULL)
	{
		/* Compare current node with next node */
		if (strcmp(current->word, current->next->word) == 0)
		{
			/* The sequence of steps is important*/
			next_next = current->next->next;
			if (next_next->info1 == NULL) {
				next_next->info1 = new char[800];
				strcpy(next_next->info1, current->info);
			}
			else {
				if (next_next->info2 == NULL) {
					next_next->info2 = new char[800];
					strcpy(next_next->info2, current->info);
				}
				else {
					if (next_next->info3 == NULL) {
						next_next->info3 = new char[800];
						strcpy(next_next->info3, current->info);
					}
				}
			}
			delete current->next;
			current->next = next_next;
		}
		else /* This is tricky: only advance if no deletion */
		{
			current = current->next;
		}
	}
}
struct Node* swap(struct Node* ptr1, struct Node* ptr2)
{
	Node* tmp = ptr2->next;
	ptr2->next = ptr1;
	ptr1->next = tmp;
	return ptr2;
}
void drawTable(char **table, bool text) {
	char *n = new char[15];
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 0, 0));
	if (text == 1) {
		for (int x = 1251, i = 0; x < 1251 + 30 * TABLE_SIZE; i++, x += 30) {
			_itoa(i, n, 15);
			TextOut(hdc, x + 5, 25, n, 1);
			for (int y = 40, j = 0; y < 40 + 30 * TABLE_SIZE; j++, y += 30) {
				n[0] = 65 + j;
				TextOut(hdc, 1251 - 15, y + 6, n, 1);
				TextOut(hdc, x + 6, y + 6, *(table + j) + i, 1);
			}
		}
		TextOut(hdc, 1251 + 30 * TABLE_SIZE - 30 + 6, 40 + 30 * TABLE_SIZE - 30 + 6, *(table + 6) + 6, 1);
	}
	else {
		HBRUSH rectangle = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, rectangle);
		Rectangle(hdc, 1251, 40, 1251 + 30 * TABLE_SIZE, 40 + 30 * TABLE_SIZE);
		for (int x = 1251, i = 0; x < 1251 + 30 * TABLE_SIZE; i++, x += 30) {
			_itoa(i, n, 15);
			TextOut(hdc, x + 5, 25, n, 1);
			for (int y = 40, j = 0; y < 40 + 30 * TABLE_SIZE; j++, y += 30) {
				n[0] = 65 + j;
				TextOut(hdc, 1251 - 15, y + 6, n, 1);
				Rectangle(hdc, x, y, x + 30, y + 30);
				TextOut(hdc, x + 6, y + 6, *(table + j) + i, 1);
			}
		}
		TextOut(hdc, 1251 + 30 * TABLE_SIZE - 30 + 6, 40 + 30 * TABLE_SIZE - 30 + 6, *(table + 6) + 6, 1);
		DeleteObject(rectangle);
	}
	SetBkMode(hdc, OPAQUE);
}
void bubbleSort(struct Node** head, int count)
{
	Node** h;
	int i, j, swapped;
	for (i = 0; i <= count; i++) {
		h = head;
		swapped = 0;
		for (j = 0; j < count - i - 1; j++) {
			Node* p1 = *h;
			Node* p2 = p1->next;
			if (p1->points > p2->points) {
				*h = swap(p1, p2);
				swapped = 1;
			}
			h = &(*h)->next;
		}
		if (swapped == 0) {
			break;
		}
	}
}
int pointscount(char*word, int size) {
	int i = 0;
	for (int c = 0; c < size; c++) {
		switch (word[c])
		{
		case 'E':
		case 'A':
		case 'I':
		case 'O':
		case 'N':
		case 'R':
		case 'T':
		case 'L':
		case 'S':
		case 'U': {
			i += 1;
			break;
		}
		case 'D':
		case 'G': {
			i += 2;
			break;
		}
		case 'B':
		case 'C':
		case 'M':
		case 'P': {
			i += 3;
			break;
		}
		case 'F':
		case 'H':
		case 'V':
		case 'W':
		case 'Y': {
			i += 4;
			break;
		}
		case 'K': {
			i += 5;
			break;
		}
		case 'J':
		case 'X': {
			i += 8;
			break;
		}
		case 'Q':
		case 'Z': {
			i += 10;
			break;
		}
		default:
			break;
		}
	}
	return i;
}
void append(struct Node** head_ref, char *new_data, char *info)
{
	Node* new_node = new Node;
	Node *last = *head_ref;
	for (int i = 0; i < strlen(new_data); i++) {
		new_data[i] = toupper(new_data[i]);
	}
	strcpy(new_node->word, new_data);
	strcpy(new_node->info, info);
	new_node->info1 = NULL;
	new_node->info2 = NULL;
	new_node->info3 = NULL;
	new_node->points = pointscount(new_data, strlen(new_data));
	new_node->next = NULL;
	if (*head_ref == NULL)
	{
		*head_ref = new_node;
		return;
	}
	while (last->next != NULL) {
		last = last->next;
	}
	last->next = new_node;
	return;
}
void printList(Node *node)
{
	while (node != NULL)
	{
		for (int i = 0; i < 20; i++) {
			cout << "-";
		}
		cout << endl;
		SetColor(White, Red);
		cout << " " << node->word << "   ";
		SetColor(Red, White);
		cout << "   " << "Points : " << node->points << " " << endl << endl;
		SetColor(LightGreen, Black);
		cout << "Description" << endl;
		cout << " " << node->info << endl;
		if (node->info1 != NULL) {
			cout << " " << node->info1 << endl;
		}
		if (node->info2 != NULL) {
			cout << " " << node->info2 << endl;
		}
		if (node->info3 != NULL) {
			cout << " " << node->info3 << endl;
		}
		SetColor(White, Black);
		Sleep(80);
		node = node->next;
	}
}
char *removeDuplicates(char *arr, int &size) {

	int end = size;

	for (int i = 0; i < end; i++) {
		for (int j = i + 1; j < end; j++) {
			if (arr[i] == arr[j]) {
				int shiftLeft = j;
				for (int k = j + 1; k < end; k++, shiftLeft++) {
					arr[shiftLeft] = arr[k];
				}
				end--;
				j--;
			}
		}
	}

	size = end;
	char *whitelist = new char[end];
	for (int i = 0; i < end; i++) {
		whitelist[i] = arr[i];
	}
	return whitelist;
}
void gotoxy(int x, int y)
{
	COORD Gon = { x, y };
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hStdOut, Gon);
}
void clearImg() {
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.bottom = 2000;
	rect.right = 1920;
	FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 0, 0)));
	gotoxy(0, 0);
}
void initTable(char **table, int size) {
init:
	int c = 0;
	char *ft = new char[17];
	strcpy(ft, "Dictionary/");
	char *word = new char[1];
	char *tmp = new char[20];
	word[0] = 65 + rand() % 25;
	strncat(ft, word, 1);
	strcat(ft, ".txt");
	ifstream f;
	f.open(ft);
	while (!f.eof()) {
		f >> tmp;
		if (strlen(tmp) == size) {
			c++;
		}
		else {
			f.ignore(600, '\n');
		}
	}
	f.close();
	f.open(ft);
	if (c == 0) {
		goto init;
	}
	int r = rand() % c;
	c = 0;
	while (!f.eof()) {
		f >> tmp;
		if (strlen(tmp) == size) {
			c++;
			if (c == r) {
				f.close();
				break;
			}
		}
		else {
			f.ignore(600, '\n');
		}
	}
	for (int i = 0; i < TABLE_SIZE; i++) {
		for (int j = 0; j < TABLE_SIZE; j++) {
			*(*(table + i) + j) = ' ';
		}
	}
	for (int i = 0; i < TABLE_SIZE; i++) {
		*(*(table + TABLE_SIZE / 2) + i) = toupper(tmp[i]);
	}

}
COORD TableCross(char **table) {
	drawTable(table, 0);
	int iKey = 0;
	int i = TABLE_SIZE / 2;
	int j = TABLE_SIZE / 2;
	HBRUSH rectangle = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, rectangle);
	Rectangle(hdc, 1251 + i * 30, 40 + j * 30, 1251 + i * 30 + 30, 40 + j * 30 + 30);
	drawTable(table, 1);
	SelectObject(hdc, rectangle);
	while (iKey != KEY_ENTER) {
		if (_kbhit()) {
			iKey = _getch();
			switch (iKey) {
			case KEY_LEFT: {
				if (j > 0) {
					drawTable(table, 0);
					j--;
					SelectObject(hdc, rectangle);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, rectangle);
				}
				break;
			}
			case KEY_RIGHT: {
				if (j < TABLE_SIZE - 1) {
					drawTable(table, 0);
					j++;
					SelectObject(hdc, rectangle);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, rectangle);
				}
				break;
			}case KEY_UP: {
				if (i > 0) {
					drawTable(table, 0);
					i--;
					SelectObject(hdc, rectangle);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, rectangle);
				}
				break;
			}
			case KEY_DOWN: {
				if (i < TABLE_SIZE - 1) {
					drawTable(table, 0);
					i++;
					SelectObject(hdc, rectangle);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, rectangle);
				}
				break;
			}
			case KEY_ENTER: {
				break;
			}
			}
		}
	}
	COORD ret;
	ret.X = i;
	ret.Y = j;
	DeleteObject(rectangle);
	return ret;
}
bool check(char *word, int size) {
	char *ft = new char[17];
	strcpy(ft, "Dictionary/");
	strncat(ft, word, 1);
	strcat(ft, ".txt");
	char *tmp = new char[20];
	ifstream f;
	f.open(ft);
	int c = 0;
	while (!f.eof()) {
		f >> tmp;
		if (strlen(tmp) == size) {
			int c = 0;
			for (int i = 0; i < size; i++) {
				if (toupper(tmp[i]) == word[i]) {
					c++;
				}
				if (c == size) {
					return 1;
				}
			}
		}

		f.ignore(600, '\n');
	}
	return 0;
}
bool SelectWordOnTable(char**table, int&res) {
	bool tab[TABLE_SIZE][TABLE_SIZE] = { 0 };
	HBRUSH def = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH selectR = CreateSolidBrush(RGB(0, 200, 0));
	int i = TABLE_SIZE / 2;
	int j = TABLE_SIZE / 2;
	SelectObject(hdc, selectR);
	Rectangle(hdc, 1251 + i * 30, 40 + j * 30, 1251 + i * 30 + 30, 40 + j * 30 + 30);
	drawTable(table, 1);
	int iKey = 0;
	while (iKey != KEY_ENTER) {
		if (_kbhit()) {
			iKey = _getch();
			switch (iKey) {
			case KEY_LEFT: {
				if (j > 0) {
					drawTable(table, 0);
					j--;
					SelectObject(hdc, selectR);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, selectR);
				}
				break;
			}
			case KEY_RIGHT: {
				if (j < TABLE_SIZE - 1) {
					drawTable(table, 0);
					j++;
					SelectObject(hdc, selectR);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, selectR);
				}
				break;
			}case KEY_UP: {
				if (i > 0) {
					drawTable(table, 0);
					i--;
					SelectObject(hdc, selectR);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, selectR);
				}
				break;
			}
			case KEY_DOWN: {
				if (i < TABLE_SIZE - 1) {
					drawTable(table, 0);
					i++;
					SelectObject(hdc, selectR);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, selectR);
				}
				break;
			}
			case KEY_ENTER: {
				break;
			}
			}
		}
	}
	SelectObject(hdc, selectR);
	iKey = 0;
	Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
	drawTable(table, 1);
	char *selectedword = new char[TABLE_SIZE*TABLE_SIZE];
	tab[i][j] = 2;
	COORD cent;
	cent.X = i;
	cent.Y = j;
	Sleep(50);
	int pos = 0;
	selectedword[0] = table[i][j];
	while (iKey != KEY_ENTER) {
		if (_kbhit()) {
			iKey = _getch();
			switch (iKey) {
			case KEY_LEFT: {
				if (j > 0 && tab[i][j - 1] == 0) {
					drawTable(table, 1);
					j--;
					tab[i][j] = 1;
					pos++;
					selectedword[pos] = table[i][j];
					SelectObject(hdc, selectR);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, selectR);
				}
				else {
					if (j > 0 && tab[i][j - 1] == 1) {
						drawTable(table, 1);
						SelectObject(hdc, def);
						Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
						tab[i][j] = 0;
						j--;
						pos--;
						drawTable(table, 1);
						SelectObject(hdc, def);
					}
				}
				break;
			}
			case KEY_RIGHT: {
				if (j < TABLE_SIZE - 1 && tab[i][j + 1] == 0) {
					drawTable(table, 1);
					j++;
					tab[i][j] = 1;
					pos++;
					selectedword[pos] = table[i][j];
					SelectObject(hdc, selectR);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, selectR);
				}
				else {
					if (j < TABLE_SIZE - 1 && tab[i][j + 1] == 1) {
						drawTable(table, 1);
						SelectObject(hdc, def);
						Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
						tab[i][j] = 0;
						j++;
						pos--;
						drawTable(table, 1);
						SelectObject(hdc, def);
					}
				}
				break;
			}
			case KEY_UP: {
				if (i > 0 && tab[i - 1][j] == 0) {
					drawTable(table, 1);
					i--;
					tab[i][j] = 1;
					pos++;
					selectedword[pos] = table[i][j];
					SelectObject(hdc, selectR);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, selectR);
				}
				else {
					if (i > 0 && tab[i - 1][j] == 1) {
						drawTable(table, 1);
						SelectObject(hdc, def);
						Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
						tab[i][j] = 0;
						i--;
						pos--;
						drawTable(table, 1);
						SelectObject(hdc, def);
					}
				}
				break;
			}
			case KEY_DOWN: {
				if (i < TABLE_SIZE - 1 && tab[i + 1][j] == 0) {
					drawTable(table, 1);
					i++;
					tab[i][j] = 1;
					pos++;
					selectedword[pos] = table[i][j];
					SelectObject(hdc, selectR);
					Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
					drawTable(table, 1);
					SelectObject(hdc, selectR);
				}
				else {
					if (i < TABLE_SIZE - 1 && tab[i + 1][j] == 1) {
						drawTable(table, 1);
						SelectObject(hdc, def);
						Rectangle(hdc, 1251 + j * 30, 40 + i * 30, 1251 + j * 30 + 30, 40 + i * 30 + 30);
						tab[i][j] = 0;
						i++;
						pos--;
						drawTable(table, 1);
						SelectObject(hdc, def);
					}
				}
				break;
			}
			case KEY_ENTER: {
				break;
			}
			}
		}
	}
	for (int i = 0; i < pos + 1; i++) {
		cout << selectedword[i];
	}
	if (check(selectedword, pos + 1) == 1) {
		cout << "	Nice true word!" << endl;
		res += pointscount(selectedword, pos + 1);
		drawTable(table, 0);
		return 1;
	}
	cout << endl;
	return 0;
}
void helper(Node *&fword, int &fsize, char *&word, int &wordsize, char**table) {
	fword = NULL;
	fsize = 0;
	word = new char[TABLE_SIZE];
	int c = 0;
	for (int i = 0; i < TABLE_SIZE; i++) {
		for (int j = 0; j < TABLE_SIZE; j++) {
			if (table[i][j] != ' ') {
				word[c] = table[i][j];
				c++;
			}
		}
	}
	word = removeDuplicates(word, c);
	for (int i = 0; i < c; i++) {
		cout << word[i] << " ";
	}
	wordsize = c;
	cout << endl;
	for (int letters = 0; letters <= TABLE_SIZE; letters++) {
		for (int strpos = 0; strpos < c; strpos++) {
			char *file = new char[17];
			strcpy(file, "Dictionary/");
			strncat(file, word + strpos, 1);
			strcat(file, ".txt");
			char *temp = new char[30];
			char *info = new char[800];
			ifstream f;
			f.open(file);
			if (f.is_open()) {
				while (!f.eof()) {
					f >> temp;
					int count = 0;
					bool ind = false;
					if (letters == strlen(temp) && temp[0] == word[strpos]) {
						ind = true;
						for (int i = 0; i < c; i++) {
							for (int j = 0; j < letters; j++) {
								temp[j] = toupper(temp[j]);
								if (temp[j] == word[i]) {
									count++;
								}
							}
						}
					}
					if (ind == true && count > letters - 1) {
						f.getline(info, 800, '\n');
						append(&fword, temp, info);
						fsize++;
					}
					else {
						f.ignore(600, '\n');
					}
				}
				f.close();
				bubbleSort(&fword, fsize);
			}
		}
	}
}


int main(int argc, char *argv[]) {
	SetConsoleTitle("Scrabble Helper");
	Sleep(60);
	//PlaySound("fon.wav", NULL, SND_FILENAME || SND_LOOP);
	hdc = GetDC(GetConsoleWindow());
	hwnd = GetConsoleWindow();
	SetColor(White, Black);
	srand(time(NULL));
	char **table = new char*[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++) {
		table[i] = new char[TABLE_SIZE];
	}
	int setf = 100;
	while (setf < 0 || setf>1) {
		cout << "Auto word? (1-yes,0-no)" << endl;
		cin >> setf;
	}
	initTable(table, TABLE_SIZE);
	if (setf == 0) {
		cout << "Enter word from " << TABLE_SIZE << " letters :" << endl;
		for (int i = 0; i < TABLE_SIZE; i++) {
			cin >> table[TABLE_SIZE / 2][i];
		}
	}
	for (int j = 0; j < TABLE_SIZE; j++) {
		cout << table[TABLE_SIZE / 2][j] << " ";
	}
	cout << endl;
	Node *fword = NULL;
	char *word = new char[15];
	int wordsize;
	int fsize = 0;
	int ind = 1;
	int res = 0;
	drawTable(table, 0);
	while (ind != 0) {
		SetColor(LightCyan, Black);
		cout << endl;
		cout << " 1 - Show points for letters" << endl;
		cout << " 2 - Enter new Word" << endl;
		cout << " 3 - Show best variant" << endl;
		cout << " 4 - Show all variants" << endl;
		cout << " 5 - Drop on table" << endl;
		SetColor(White, Black);
		cin >> ind;
		switch (ind) {
		case 1: {
			system("cls");
			SetColor(Yellow, Black);
			cout << endl << endl;
			cout << "\tPoints for every letter : " << endl << endl;
			SetColor(LightMagenta, Black);
			cout << "\t1 - E, A, I, O, N, R, T, L, S, U" << endl << endl;
			cout << "\t2 - D, G" << endl << endl;
			cout << "\t3 - B, C, M, P" << endl << endl;
			cout << "\t4 - F, H, V, W, Y" << endl << endl;
			cout << "\t5 - K" << endl << endl;
			cout << "\t8 - J, X" << endl << endl;
			cout << "\t10 - Q, Z" << endl << endl;
			drawTable(table, 0);
			break;
		}
		case 2: {
			system("cls");
			delete fword;
			helper(fword, fsize, word, wordsize, table);
			removeDuplicates(fword);
			printList(fword);
			drawTable(table, 0);
			break;
		}
		case 3: {
			system("cls");
			drawTable(table, 0);
			break;
		}
		case 4: {
			system("cls");
			cout << endl << "Entered word is : ";
			for (int i = 0; i < strlen(word); i++) {
				cout << word[i] << " ";
			}
			cout << endl;
			printList(fword);
			drawTable(table, 0);
			break;
		}
		case 5: {
			system("cls");
			COORD Selected;
			char a;
			do {
				cout << "Enter the Letter what you want to paste : ";
				drawTable(table, 0);
				cin >> a;
			} while (a < 65 || a>90);
			drawTable(table, 0);
			bool ind = 0;
			do {
				Selected = TableCross(table);
				if (table[Selected.X][Selected.Y] != ' ') {
					ind = 0;
				}
				else {
					if (Selected.X > 0 && table[Selected.X - 1][Selected.Y] != ' ') {
						ind = 1;
						break;
					}
					if (Selected.X < 6 && table[Selected.X + 1][Selected.Y] != ' ') {
						ind = 1;
						break;
					}
					if (Selected.Y < 0 && table[Selected.X][Selected.Y - 1] != ' ') {
						ind = 1;
						break;
					}
					if (Selected.Y < 6 && table[Selected.X][Selected.Y + 1] != ' ') {
						ind = 1;
						break;
					}
				}
				if (ind == 0) {
					cout << "Error!" << endl;
				}
			} while (ind == 0);
			table[Selected.X][Selected.Y] = toupper(a);
			drawTable(table, 0);
			if (SelectWordOnTable(table, res) == 0) {
				table[Selected.X][Selected.Y] = ' ';
				cout << "Error! Cant find this word in dictionary" << endl;
			}
			cout << res << endl;
			drawTable(table, 0);
			break;
		}
		default: {
			break;
		}
		}
	}

	system("pause");
	return 0;
}