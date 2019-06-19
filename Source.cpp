#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <conio.h>
#include <ctime>
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
#define TABLE_SIZE 9
struct Node {
	char word[30];
	char info[800];
	int points;
	int sovp = 0;
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
WORD colorConvertTo(int ForgC, int BackC){
	return ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
}
struct Node* swap(struct Node* ptr1, struct Node* ptr2)
{
	Node* tmp = ptr2->next;
	ptr2->next = ptr1;
	ptr1->next = tmp;
	return ptr2;
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
char *Entering() {
	char *enterword = new char[30];
	cout << "Enter word with * : ";
	cin >> enterword;
	int wordsize = strlen(enterword);
	char *word = new char[wordsize];
	strcpy(word, enterword);
	delete enterword;
	for (int i = 0; i < strlen(word); i++) {
		word[i] = toupper(word[i]);
	}
	return word;
}
int pointscount(char*word) {
	int i = 0;
	for (int c = 0; c < strlen(word); c++) {
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
	new_node->points = pointscount(new_data);
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
		SetColor(White, Black);
		node = node->next;
	}
}
int bestV(Node *node, char *letters) {
	if (node == NULL) {
		return 0;
	}
	int sovp = 0;
	for (int j = 1; j < strlen(node->word)-1; j++) {
		for (int i = 0; i < 7; i++) {
			if (node->word[j] == letters[i]) {
				j++;
				sovp++;
			}
		}
	}
	if (sovp >= (strlen(node->word) - 2)) {
		node->sovp = 100;
	}
	else {
		node->sovp = sovp;
	}
	if (sovp > bestV(node->next, letters)) {
		return sovp;
	}
}
void find_max(struct Node*temp, char*letters) {

	if (temp != NULL) {
		int max_value = bestV(temp, letters);
		while (temp != NULL) {
			if (temp->sovp >= max_value) {
				for (int i = 0; i < 20; i++) {
					cout << "-";
				}
				cout << endl;
				if (temp->sovp == 100) {
					SetColor(Yellow, Black);
				}
				else {
					SetColor(White, Red);
				}
				cout << "   Best word for you : ";
				SetColor(Red, White);
				cout << "   " << temp->word << "   ";
				SetColor(White, Red);
				cout << "   Points : ";
				SetColor(Red, White);
				cout << "  " << temp->points << "   " << endl;
				SetColor(LightGreen, Black);
				cout << "\t\t\tDescription" << endl;
				cout << " \t\t\t" << temp->info << endl;
				SetColor(White, Black);
				cout << endl << endl << endl;
			}
			temp = temp->next;
		}
	}
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
void drawTable(char **table, bool text) {
	char *n = new char[15];
	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc,RGB(255, 0, 0));
	if (text == 1) {
		for (int x = 1251, i = 0; x < 1251+30* TABLE_SIZE; i++, x += 30) {
			_itoa(i, n, 15);
			TextOut(hdc, x + 5, 25, n , 1);
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
COORD TableCross(char **table, char *letters) {
	drawTable(table, 0);
	int iKey = 0;
	int i = TABLE_SIZE/2;
	int j = TABLE_SIZE/2;
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
				if (j < TABLE_SIZE-1) {
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
				if (i < TABLE_SIZE-1) {
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
			int c= 0;
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
bool SelectWordOnTable(char**table, char*lettres) {
	bool tab[TABLE_SIZE][TABLE_SIZE] = { 0 };
	HBRUSH def = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH selectR = CreateSolidBrush(RGB(0, 200, 0));
	int i = TABLE_SIZE/2;
	int j = TABLE_SIZE/2;
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
				if (j < TABLE_SIZE-1) {
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
				if (i < TABLE_SIZE-1) {
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
	char *selectedword = new char[49];
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
				if (j < TABLE_SIZE-1 && tab[i][j + 1] == 0) {
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
					if (j < TABLE_SIZE-1 && tab[i][j + 1] == 1) {
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
				if (i < TABLE_SIZE-1 && tab[i + 1][j] == 0) {
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
					if (i < TABLE_SIZE-1 && tab[i + 1][j] == 1) {
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
	for (int i = 0; i < pos+1; i++) {
		cout << selectedword[i];
	}
	if (check(selectedword, pos + 1) == 1) {
		cout << "	Nice true word!" << endl;
		drawTable(table, 0);
		return 1;
	}
	cout << endl;
	return 0;
}

int main(int argc, char *argv[]) {
	SetConsoleTitle("Scrabble Helper");
	hdc = GetDC(GetConsoleWindow());
	hwnd = GetConsoleWindow();
	SetColor(White, Black);
	srand(time(NULL));
	char **table = new char*[TABLE_SIZE];
	for (int i = 0; i < TABLE_SIZE; i++) {
		table[i] = new char[TABLE_SIZE];
	}

	initTable(table, TABLE_SIZE);
	for (int i = 0; i < TABLE_SIZE; i++) {
		for (int j = 0; j < TABLE_SIZE; j++) {
			cout << table[i][j] << " ";
		}
		cout << endl;
	}
	char *letters = new char[TABLE_SIZE];
	cout << endl << "Letters : ";
	for (int i = 0; i < TABLE_SIZE; i++) {
		letters[i] = 65 + rand() % 25;
		cout << letters[i] << " ";
	}
	cout << endl;
start:
	Node *fword = NULL;
	int fsize = 0;
	char *word = Entering();
	int wanted = 0;
	for (int i = 0; i < strlen(word); i++) {
		if (word[i] == '*') {
			wanted++;
		}
	}
	char *file = new char[17];
	strcpy(file, "Dictionary/");
	strncat(file, word, 1);
	strcat(file, ".txt");
	char *temp = new char[30];
	char *info = new char[800];
	ifstream f;
	f.open(file);
	if (!f.is_open()) {
		cout << "can't find" << endl;
		system("pause");
		return 0;
	}
	while (!f.eof()) {
		f >> temp;
		bool ind = false;
		int count = 0;
		if (strlen(word) == strlen(temp)) {
			for (int i = 0; i < strlen(temp); i++) {
				temp[i] = toupper(temp[i]);
				if (word[i] == temp[i]) {
					count++;
				}
			}
			if (word[strlen(word) - 1] == '*' || word[strlen(word) - 1] == temp[strlen(temp) - 1]) {
				ind = true;
			}
			if ((count >= (strlen(temp) - wanted)) && ind == true) {
				f.getline(info, 800, '\n');
				append(&fword, temp, info);
				fsize++;
			}
		}
		else {
			f.ignore(600, '\n');
		}
	}
	f.close();
	bubbleSort(&fword, fsize);
	int ind = 1;
	int iKey = 0;
	while (iKey != KEY_ESC) {
		SetColor(LightCyan, Black);
		cout << endl;
		cout << " 1 - Enter new Word" << endl;
		cout << " 2 - Show best variant" << endl;
		cout << " 3 - Show all variants" << endl;
		cout << " 4 - Drop on table" << endl;
		SetColor(White, Black);
		cin >> ind;
		switch (ind) {
		case 1: {
			system("cls");
			delete temp, file, info, word, letters;
			delete fword;
			goto start;
			break;
		}
		case 2: {
			system("cls");
			cout << endl << "You letters : ";
			for (int i = 0; i < TABLE_SIZE; i++) {
				cout << letters[i] << " ";
			}
			cout << endl << "Entered word is : ";
			for (int i = 0; i < strlen(word); i++) {
				cout << word[i] << " ";
			}
			cout << endl;
			find_max(fword, letters);
			drawTable(table, 0);
			break;
		}
		case 3: {
			system("cls");
			cout << endl << "You letters : ";
			for (int i = 0; i < TABLE_SIZE; i++) {
				cout << letters[i] << " ";
			}
			cout << endl << "Entered word is : ";
			for (int i = 0; i < strlen(word); i++) {
				cout << word[i] << " ";
			}
			cout << endl;
			printList(fword);
			drawTable(table, 0);
			break;
		}
		case 4: {
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
				Selected = TableCross(table, letters);
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
			if (SelectWordOnTable(table, letters) == 0) {
				table[Selected.X][Selected.Y] = ' ';
				cout << "Error! Cant find this word in dictionary" << endl;
			}
			drawTable(table, 0);
			break;
		}
		default: {
			break;
		}
		}
		iKey = _getch();
	}

	system("pause");
	return 0;
}