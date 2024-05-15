#include "Game.h"
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
using namespace std;
// глобальні змінні
int n1 = 0, n2 = 0, botwins = 0, playerwins = 0, BotArrNums = 0, CheckNums = 0;
static string prevVarL = "0", prevVarR = "0", peretvor, points = "0-0", firstCourse = "player", NickName =""; // points зліва Бот, справа користувач
bool isrestart = false, run = true, firstTime = true, firstChoose = true, bazarFull = false, isPass, menu = true;
const int row = 3, col = 57, rows= 7, cols = 4;
string gameField[row][col], Botarr[7], Botarr01[7] = {"1","1","1","1","1","1","1"};
const string arrStick[rows][cols] = {   {"0|0","1|0","1|1","2|0"},
										{"2|1","2|2","3|0","3|1"},
										{"3|2","3|3","4|0","4|1"},
										{"4|2","4|3","4|4","5|0"},
										{"5|1","5|2","5|3","5|4"},
										{"5|5","6|0","6|1","6|2"},
										{"6|3","6|4","6|5","6|6"} };
int arrFill[rows][cols];
// методи(функції)

// Меню гри
void Game::Menu() 
{
	if (menu) 
	{
		toMain:
		string menuChoice, underChoice, leadersChoice;
		cout << "*------------DOMINO-------------*" << endl;
		cout << "|\t1.Почати гру\t\t|\n";
		cout << "|\t2.Список лідерів\t|\n";
		cout << "|\t3.Правила й інстуркція\t|\n";
		cout << "|\t4.Вихід\t\t\t|\n";
		cout << "*-------------------------------*";
		cout << "\nОберіть дію(1-4): "; cin >> menuChoice;
		switch (menuChoice[0])
		{
		case '1':
			clear();
			cout << "Введіть ваш нікнейм(ім'я) для відображення в списку лідерів: "; cin >> NickName;	
			menu = false;
			clear();
			break;
		case '2':
			clear();
			toTry:
			FileSort("save.txt");
			cout << "Для виходу введіть q: "; cin >> leadersChoice;
			if (leadersChoice[0] == 'q') { clear(); goto toMain; }
			else { cout << "Повторіть спробу!"; Sleep(700); clear(); goto toTry; }
			break;
		case '3':
			clear();
			toBegin:
			cout << """ПРАВИЛО: Гра починається з 7 фішок у гравця та комп'ютера.\nПереможе той у кого або не залишиться фішок, або у кого сума чисел з фішок менша, а також нічия,\nякщо сума однакова(гра рахує автоматично).\nТакож є базар, звідки ви будете брати фішки в \"руки\"(Максимум 7).\nІНСТУРКЦІЯ: Для виходу з гри введіть q.\nДля того щоб обрати фішку введіть цифри від 1 до 7.\nДля взяття фішок з базару введіть клавішу b та оберіть положення фішки.\nДля пропуску ходу введіть клавішу p.\nОчки розміщенні зверху гри, де зліва рахунок комп'ютера, зправа - ваш.\n\t !!!ЯКЩО БУДЕ ВВЕДЕНА НЕ ТА ЛІТЕРА ЧИ ЦИФРА, ГРА ВАС ПРО ЦЕ ПОПЕРЕДИТЬ!!!\n""";
			cout << "Для виходу введіть q: "; cin >> underChoice;
			if (underChoice[0] == 'q') { clear(); goto toMain; }
			else { cout << "Повторіть спробу!"; Sleep(700); clear(); goto toBegin; }
			break;
		case '4':
			run = false;
			break;
		default:
			cout << "Помилка! Повторіть спробу!"; 
			Sleep(600); 
			clear();
			goto toMain;
		break;
		}
	}
}
// Збереження
void Game::FileSaving() 
{
	ofstream file("save.txt", std::ios::app);
	string full;
	if (!file) {
		cerr << "Не вдалося відкрити файл!" << endl;
	}
	full = NickName + ": " + points + "\n";
	file << full;
	file.close();
}
// Відсортування Файлу
struct Record {
	string name;
	double score = 0;
};
void Game::FileSort(string filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Помилка відкриття файлу!" << endl;
		return;
	}
	const int MAX_RECORDS = 100;
	Record* records = new Record[MAX_RECORDS];
	int numRecords = 0;
	string line;
	while (getline(file, line)) {
		size_t pos = line.find(":");
		if (pos != string::npos) {
			// розділення строки
			string Name = line.substr(0, pos);
			// взяти число після тире
			double balance = stod(line.substr(line.find("-", pos) + 1));
			// пошук схожих імен
			int index = -1;
			for (int i = 0; i < numRecords; ++i) {
				if (records[i].name == Name) {
					index = i;
					break;
				}
			}
			// запис рахунку
			if (index != -1) records[index].score += balance;
			else {
				records[numRecords].name = Name;
				records[numRecords].score = balance;
				numRecords++;
			}
		}
	}
	// сортування
	for (int i = 0; i < numRecords - 1; ++i) {
		for (int j = 0; j < numRecords - i - 1; ++j) {
			if (records[j].score < records[j + 1].score) {
				swap(records[j], records[j + 1]);
			}
		}
	}
	// вивід
	for (int i = 0; i < numRecords; ++i) {
		cout << records[i].name << ": " << records[i].score << endl;
	}
	delete[] records;
}
// Хто виграв?
void Game::Score() 
{
	int bot = 0, player = 0;
	for (int i = 0; i < 7; ++i)
		if (Botarr[i] == "0") ++bot;
	for (int j = 12; j < 19; ++j)
		if (gameField[2][j] == "0") ++player;
	if (bot == 7) 
	{
		if (playerwins >= 10) 
		{
			botwins += 1;
			points.erase(0, 2); points.insert(0, to_string(botwins));
			restart(); clear(); firstCourse = "bot";
		}
		else { botwins += 1; points.erase(points.begin()); points.insert(0, to_string(botwins)); restart(); clear(); firstCourse = "bot"; }
	}
	else if (player == 7) 
	{
		if (botwins >= 10)
		{
			playerwins += 1;
			points.erase(3); points.insert(3, to_string(playerwins));
			restart(); clear(); firstCourse = "player";
		}
		else { playerwins += 1; points.erase(2); points.insert(2, to_string(playerwins)); restart(); clear(); firstCourse = "player"; }
	}
	int labelcount = 0;
	for (int l = 0; l < 7; l++)
		if (Botarr[l] == "0" or gameField[2][(12 + l)] == "0") ++labelcount;
	if (bazarFull or labelcount == 0)
	{
		int Canput = 0, BotsSum = 0, PlayersSum = 0; string BotsSticks,PlayersSticks, peretBotSt, peretPlayerSt;
		for (int h = 0; h < 7; h++)
		{
			if (Botarr[h] != "0" and gameField[2][(12 + h)] != "0")
			{
				BotsSticks = Botarr[h]; PlayersSticks = gameField[2][(12 + h)];
				peretBotSt = BotsSticks; peretPlayerSt = PlayersSticks;
				swap(peretBotSt[0], peretBotSt[2]); swap(peretPlayerSt[0], peretPlayerSt[2]);
				if ((BotsSticks[2] == prevVarL[0] or peretBotSt[0] == prevVarR[2]) or (BotsSticks[0] == prevVarR[2] or peretBotSt[2] == prevVarL[0]))
					++Canput;
				if ((PlayersSticks[2] == prevVarL[0] or peretPlayerSt[0] == prevVarR[2]) or (PlayersSticks[0] == prevVarR[2] or peretPlayerSt[2] == prevVarL[0]))
					++Canput;
			}
		}
		if (Canput == 0) 
		{
			for (int b = 0; b < 7; b++)
			{
				if (Botarr[b] != "0" and gameField[2][(12 + b)] != "0")
				{
					BotsSticks = Botarr[b]; PlayersSticks = gameField[2][(12 + b)];
					BotsSum = (int)BotsSticks[0] + (int)BotsSticks[2];
					PlayersSum = (int)PlayersSticks[0] + (int)PlayersSticks[2];
				}
			}
			if (BotsSum < PlayersSum)
			{
				if (playerwins >= 10)
				{
					botwins += 1;
					points.erase(0, 2); points.insert(0, to_string(botwins));
					restart(); clear(); firstCourse = "bot";
				}
				else { botwins += 1; points.erase(points.begin()); points.insert(0, to_string(botwins)); restart(); clear(); firstCourse = "bot"; }

			}
			else if (BotsSum > PlayersSum)
			{
				if (botwins >= 10)
				{
					playerwins += 1;
					points.erase(3); points.insert(3, to_string(playerwins));
					restart(); clear(); firstCourse = "player";
				}
				else { playerwins += 1; points.erase(2); points.insert(2, to_string(playerwins)); restart(); clear(); firstCourse = "player"; }

			}
			else if (BotsSum == PlayersSum)
			{
				if (playerwins >= 10)
				{
					botwins += 1;
					points.erase(0, 2); points.insert(0, to_string(botwins));
					restart(); clear(); firstCourse = "bot";
				}
				else { botwins += 1; points.erase(points.begin()); points.insert(0, to_string(botwins)); restart(); clear(); firstCourse = "bot"; }
				//+
				if (botwins >= 10)
				{
					playerwins += 1;
					points.erase(3); points.insert(3, to_string(playerwins));
					restart(); clear(); firstCourse = "player";
				}
				else { playerwins += 1; points.erase(2); points.insert(2, to_string(playerwins)); restart(); clear(); firstCourse = "player"; }
			}
		}
	}
	// кінець
}
// Запуск
bool Game::Run()
{
	return run;
}
// очистка екарну
void Game::clear() 
{
	system("cls");
} // для Windows, для Unix подібних system("clear");
// вихід
void Game::quit() {
	FileSaving();
	run = false;
}
// рестарт гри
void Game::restart()
{
	firstChoose = true; firstTime = true; CheckNums = 0; BotArrNums = 0; bazarFull = false; isrestart = true;
	prevVarL = "0"; prevVarR = "0";
	for (int i = 0; i < 7; ++i)
		for (int j = 0; j < 4; ++j)
			arrFill[i][j] = 0;
	for (int iF = 0; iF < 57; iF++) 
		gameField[1][iF] = "";
	for (int iP = 12; iP < 19; iP++)
		gameField[2][iP] = "0";
	for (int iB = 0; iB < 7; iB++)
	{
		Botarr[iB] = "0";
		Botarr01[iB] = "1";
	}
}
// рандом числа
void Game::GetRandNums(int* n1, int* n2) 
{ 
	Sleep(1);
	*n1 = 0 + rand() % 7;
	*n2 = 0 + rand() % 4;
}
// видача фішок для базару
void Game::bazgiveStick(int numPlace)
{
	int a = 0;
	GetRandNums(&n1, &n2);
	while (a != 1)
	{
		if (arrFill[n1][n2] == 1)
			GetRandNums(&n1, &n2);
		else
		{ arrFill[n1][n2] = 1; gameField[2][numPlace] = arrStick[n1][n2]; a = 1;}
	}
}
// базар 
void Game::bazar()
{
	int l = 0;
	string corr, peretv;
	int n = 0;
	// перевірка
	while (l != 1)
	{
		for (int i = 12; i <= 18; i++)
		{
			if (gameField[2][i] != "0")
			{
				corr = gameField[2][i];
				// перетворення
				{
					peretv = corr;
					char temp = peretv[0];
					peretv[0] = peretv[2];
					peretv[2] = temp;
				}
				if ((corr[2] == prevVarL[0] or peretv[0] == prevVarR[2]) or (corr[0] == prevVarR[2] or peretv[2] == prevVarL[0]))
					++n;
			}
		}
		l = 1;
	}
	// реалізація
	if (n > 0)
		cout << "У вас ще є " << n << " ходів";
	else
	{
		if (!bazarFull)
		{
			string bazChoose;
			int numPlace = 12;
			cout << "Оберіть місце розташування фішки(1-7): "; cin >> bazChoose;
			switch (bazChoose[0])
			{
			case '1':
				if (gameField[2][numPlace] == "0") bazgiveStick(numPlace);
				else cout << "Вже є фішка!";
				break;
			case '2':
				if (gameField[2][(numPlace + 1)] == "0") bazgiveStick((numPlace + 1));
				else cout << "Вже є фішка!";
				break;
			case '3':
				if (gameField[2][(numPlace + 2)] == "0") bazgiveStick((numPlace + 2));
				else cout << "Вже є фішка!";
				break;
			case '4':
				if (gameField[2][(numPlace + 3)] == "0") bazgiveStick((numPlace + 3));
				else cout << "Вже є фішка!";
				break;
			case '5':
				if (gameField[2][(numPlace + 4)] == "0") bazgiveStick((numPlace + 4));
				else cout << "Вже є фішка!";
				break;
			case '6':
				if (gameField[2][(numPlace + 5)] == "0") bazgiveStick((numPlace + 5));
				else cout << "Вже є фішка!";
				break;
			case '7':
				if (gameField[2][(numPlace + 6)] == "0") bazgiveStick((numPlace + 6));
				else cout << "Вже є фішка!";
				break;
			default:
				cout << "Помилка! Повторіть спробу!";
				break;
			}
		}
	}
}
// базар для бота
void Game::Botbazar() 
{
	CheckArrAvailableSticks();
	AgainLoop:
	string stockstick, changedstick;
	int a = 0, i = 0;
	GetRandNums(&n1, &n2);
	if (!bazarFull) 
	{
		while (a != 1)
		{
			if (arrFill[n1][n2] == 1)
				GetRandNums(&n1, &n2);
			if (arrFill[n1][n2] != 1)
			{
				if (bazarFull) { a = 1; goto Ais1; }
				if (Botarr[i] == "0")
				{
					arrFill[n1][n2] = 1;
					Botarr[i] = arrStick[n1][n2];
					Botarr01[i] = "1";
					a = 1;
				}
				++i;
			}
			GetRandNums(&n1, &n2);
		Ais1:
			if (i == 7) { a = 1; }
		}
	}
	if (!bazarFull)
	{
		for (int j = 0; j < 7; j++)
		{
			stockstick = Botarr[j];
			if (stockstick != "0")
			{
				changedstick = stockstick;
				swap(changedstick[0], changedstick[2]);
				if ((stockstick[2] == prevVarL[0] or changedstick[2] == prevVarL[0]) or (stockstick[0] == prevVarR[2] or changedstick[0] == prevVarR[2]))
					++BotArrNums;
			}
		}
		if (BotArrNums == 0 and CheckNums != 0)
		{
			int numbersinBotArr = 0;
			for (int k = 0; k < 7; k++)
				if (Botarr[k] != "0") ++numbersinBotArr;
			if (numbersinBotArr != 7 and CheckNums != 0) goto AgainLoop;
			BotArrNums = 0;
		}
	}
	else BotArrNums = 0;
}
// поле
void Game::Fieldgame()
{
	for (int i = 0; i < row; i++, cout << endl)
		for (int j = 0; j < col; j++)
		{
			if (i == 0)
			{
				if (j >= 12 and j <= 18)
				{
					cout << gameField[i][j] << "   ";
				} else cout << gameField[i][j] << " ";
			}
			if (i == 2)
			{
				cout << gameField[i][j] << " ";
			}
			if (i == 1)
				cout << gameField[i][j];
		}
}
// Комп'ютер
void Game::Bot() 
{
	for (int i = 0; i < 7; ++i)
		gameField[0][(12+i)] = Botarr01[i];
	GetBestStick();

}
// видача фішок
void Game::giveSticks(string gamer)
{
	if (gamer == "player")
	{
		GetRandNums(&n1, &n2);
		bool loop = true;
		int i = 12;
		while (loop)
		{
			if (arrFill[n1][n2] == 1)
				GetRandNums(&n1, &n2);
			if (arrFill[n1][n2] != 1)
			{
				gameField[2][i] = arrStick[n1][n2];
				arrFill[n1][n2] = 1;
				GetRandNums(&n1, &n2);
				++i;
			}
			if (i == 19)
				loop = false;
		}
	}
	if (gamer == "bot")
	{
		GetRandNums(&n1, &n2);
		bool loop = true;
		int i = 12;
		int n = 0;
		while (loop)
		{
			if (arrFill[n1][n2] == 1)
				GetRandNums(&n1, &n2);
			if (arrFill[n1][n2] != 1)
			{
				Botarr[n] = arrStick[n1][n2];
				arrFill[n1][n2] = 1;
				GetRandNums(&n1, &n2);
				++i;
				++n;
			}
			if (i == 19) loop = false;
		}
	}
}
// Обирання фішок для ходу(бот)
void Game::GetBestStick()
{
	if (!isrestart or firstChoose) 
	{
		if (firstChoose)
		{
			BotSideLeft(Botarr[0], 12);
			Botarr[0] = "0";

		}
		else if (!firstChoose)
		{
		tryAgain:
			int arrN[7], ArrWithNums[7] = { 12,13,14,15,16,17,18 }, prevN1 = 0, N_1 = 0, prevN2 = 0, N_2 = 0, index = 0;
			string a, peretv, place1, place2, Side, arrSide[7], arrChangeStick[7]; // arrChangeStick - перетворена або звичайна(chganged or usual), place(1-ліва, 2-права)(перетворена чи ні(usual or chnaged))
			bool loop = true;
			while (loop)
			{
				prevN1 = 0, N_1 = 0, prevN2 = 0, N_2 = 0;
				/*a = gameField[0][ArrWithNums[index]];*/
				a = Botarr[index];
				if (a == "0")
				{
					arrN[index] = 0;
					arrSide[index] = "0";
					arrChangeStick[index] = "0";
					++index;
					goto Bottom;
				}
				{
					peretv = a;
					if (peretv != "0|0")
					{
						char temp = peretv[0];
						peretv[0] = peretv[2];
						peretv[2] = temp;
					}
				}
				if ((a[2] == prevVarL[0] or peretv[0] == prevVarR[2]) or (a[0] == prevVarR[2] or peretv[2] == prevVarL[0]))
				{
					//ліва сторона
					if ((a[2] == prevVarL[0] or peretv[2] == prevVarL[0]))
					{
						string now, nowChanged;
						// звичайна фішка
						if (a[2] == prevVarL[0])
						{
							for (int i = 0; i < 7;)
							{
								/*now = gameField[0][ArrWithNums[i]];*/
								now = Botarr[i];
								// перетворення обраної фішки(перебір)
								if (now != "0")
								{
									{
										nowChanged = now;
										char temp = nowChanged[0];
										nowChanged[0] = nowChanged[2];
										nowChanged[2] = temp;
									}
									if (a != now)
									{
										if (a[0] == now[2] or a[0] == nowChanged[2]) { ++N_1; ++i; }
										else ++i;
									}
									else ++i;
								}
								else ++i;
							}
						}
						// перетворена
						if (peretv[2] == prevVarL[0])
						{
							for (int i = 0; i < 7;)
							{
								/*now = gameField[0][ArrWithNums[i]];*/
								now = Botarr[i];
								// перетворення обраної фішки(перебір)
								if (now != "0")
								{
									{
										nowChanged = now;
										char temp = nowChanged[0];
										nowChanged[0] = nowChanged[2];
										nowChanged[2] = temp;
									}
									if (a != now)
									{
										if (peretv[0] == now[2] or peretv[0] == nowChanged[2]) { ++prevN1; ++i; }
										else ++i;
									}
									else ++i;
								}
								else ++i;
							}
						}
						if (prevN1 >= N_1) { N_1 = prevN1; place1 = "changed"; }
						else place1 = "usual";
					}
					// права сторона
					if ((a[0] == prevVarR[2] or peretv[0] == prevVarR[2]))
					{
						string now2, nowChanged2;
						// звичайна
						if (a[0] == prevVarR[2])
						{
							for (int i = 0; i < 7;)
							{
								/*now2 = gameField[0][ArrWithNums[i]];*/
								now2 = Botarr[i];
								// перетворення обраної фішки(перебір)
								if (now2 != "0")
								{
									{
										nowChanged2 = now2;
										char temp = nowChanged2[0];
										nowChanged2[0] = nowChanged2[2];
										nowChanged2[2] = temp;
									}
									if (a != now2)
									{
										if (a[2] == now2[0] or a[2] == nowChanged2[0]) { ++N_2; ++i; }
										else ++i;
									}
									else ++i;
								}
								else ++i;
							}
						}
						// перетворена
						if (peretv[0] == prevVarR[2])
						{
							for (int i = 0; i < 7;)
							{
								/*now2 = gameField[0][ArrWithNums[i]];*/
								now2 = Botarr[i];
								// перетворення обраної фішки(перебір)
								if (now2 != "0")
								{
									{
										nowChanged2 = now2;
										char temp = nowChanged2[0];
										nowChanged2[0] = nowChanged2[2];
										nowChanged2[2] = temp;
									}
									if (a != now2)
									{
										if (peretv[2] == now2[0] or peretv[2] == nowChanged2[0]) { ++prevN2; ++i; }
										else ++i;
									}
									else ++i;
								}
								else ++i;
							}
						}
						if (prevN2 >= N_2) { N_2 = prevN2; place2 = "changed"; }
						else place2 = "usual";
					}
					if (N_1 >= N_2)
					{
						Side = "L"; arrSide[index] = Side; arrChangeStick[index] = place1; arrN[index] = N_1; ++index;
					}
					else if (N_1 <= N_2) { Side = "R"; arrSide[index] = Side; arrChangeStick[index] = place2; arrN[index] = N_2; ++index; }
					if (N_1 == 0 or N_2 == 0)
					{
						if (arrSide[index] == "R" or arrSide[index] == "L")
						{
							arrN[index] = 1; ++index;
						}
					}
				}
				else
				{
					arrN[index] = 0;
					arrSide[index] = "0";
					arrChangeStick[index] = "0";
					++index;
				}
			Bottom:
				if (index == 7) loop = false;
			}
			// аналіз й передача у функцію фішки
		Again:
			int arrindex = 0, arrmax;
			arrmax = arrN[0];
			for (int i = 0; i < 7; i++)
			{
				if (arrN[i] >= arrmax) { arrmax = arrN[i]; arrindex = i; }
			}
			if (arrmax == 0)
			{
				string stock, peretvorena; int countes = 0;
				for (int i = 0; i < 7; i++)
				{
					if (arrSide[i] != "0")
					{
						stock = Botarr[i];
						{ peretvorena = stock; swap(peretvorena[0], peretvorena[2]); }
						if ((stock[2] == prevVarL[0] or peretvorena[2] == prevVarL[0]))
						{
							arrSide[i] = "L";
							arrN[i] = 1;
							if (stock[2] == prevVarL[0]) arrChangeStick[i] = "usual";
							if (peretvorena[2] == prevVarL[0]) arrChangeStick[i] = "changed";
							countes++;
						}
						else if ((stock[0] == prevVarR[2] or peretvorena[0] == prevVarR[2]))
						{
							arrSide[i] = "R";
							arrN[i] = 1;
							if (stock[0] == prevVarR[2]) arrChangeStick[i] = "usual";
							if (peretvorena[0] == prevVarR[2]) arrChangeStick[i] = "changed";
							countes++;
						}
					}
				}
				if (countes != 0) goto Again;
				else if (countes == 0)
				{
					Botbazar();
					if (!bazarFull)
					{
						/*Botbazar();*/
						if (BotArrNums > 0) { BotArrNums = 0; goto tryAgain; }
						else if (BotArrNums == 0) { cout << "Пас! Хід Користувача!"; Sleep(600);}
					}
					else { cout << "\nВ Базарі немає фішок!"; Sleep(600);}
				}
			}
			else if (arrmax != 0)
			{
				if (arrSide[arrindex] == "R")
				{
					if (arrChangeStick[arrindex] == "usual")
					{
						BotSideRight(Botarr[arrindex], ArrWithNums[arrindex]); 
						Botarr[arrindex] = "0"; Botarr01[arrindex] = "0";
					}
					else if (arrChangeStick[arrindex] == "changed")
					{
						peretv = Botarr[arrindex];
						char temp;
						temp = peretv[0];
						peretv[0] = peretv[2];
						peretv[2] = temp;
						BotSideRight(peretv, ArrWithNums[arrindex]);
						Botarr[arrindex] = "0"; Botarr01[arrindex] = "0";
					}
				}
				else if (arrSide[arrindex] == "L")
				{
					if (arrChangeStick[arrindex] == "usual")
					{
						BotSideLeft(Botarr[arrindex], ArrWithNums[arrindex]); 
						Botarr[arrindex] = "0"; Botarr01[arrindex] = "0";
					}
					else if (arrChangeStick[arrindex] == "changed")
					{
						peretv = Botarr[arrindex];
						char temp;
						temp = peretv[0];
						peretv[0] = peretv[2];
						peretv[2] = temp;
						BotSideLeft(peretv, ArrWithNums[arrindex]);
						Botarr[arrindex] = "0"; Botarr01[arrindex] = "0";
					}
				}
			}
		}
		firstChoose = false;
	}
	// кінець
}
// перевірка фішок в масиві
void Game::CheckArrAvailableSticks() 
{
	CheckNums = 0;
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 4; ++j) {
			{ if (arrFill[i][j] != 1) ++CheckNums; }
		}
	}
	if (CheckNums == 0) { bazarFull = true; }
}
// перевірка Пасу
void Game::passAvailable() 
{
	int l = 0;
	string corr, peretv;
	int n = 0;
	// перевірка
	while (l != 1)
	{
		for (int i = 12; i <= 18; i++)
		{
			if (gameField[2][i] != "0")
			{
				corr = gameField[2][i];
				// перетвор
				{
					peretv = corr;
					char temp = peretv[0];
					peretv[0] = peretv[2];
					peretv[2] = temp;
				}
				if ((corr[2] == prevVarL[0] or peretv[0] == prevVarR[2]) or (corr[0] == prevVarR[2] or peretv[2] == prevVarL[0]))
					++n;
			}
		}
		l = 1;
	}
	// реалізація
	if (n > 0)
	{
		cout << "У вас ще є " << n << " ходів!";
		isPass = false;
	}
	else if (n == 0) isPass = true;
}
//Ініціалізація раздачи фішок
void Game::Initialize()
{
	if (firstTime) {
		giveSticks("player");
		giveSticks("bot");
		firstTime = false;}
}
//хід вліво
void SideLeft(string a, int ncol, const int nrow = 2)
{
	gameField[nrow][ncol] = "0";
	for (int i = 1; i <= 1; i++) {
		for (int j = 28; j >= 0;)
		{
			if (gameField[i][j] == "")
			{
				a.insert(0, "-");
				gameField[i][j] = a;
				j = -1;
			}
			else if (gameField[i][j] != "") j--;
		}
	}
}
// хід вправо
void SideRight(string a, int ncol, const int nrow = 2) 
{
	gameField[nrow][ncol] = "0";
	for (int i = 1; i <= 1; i++) {
		for (int j = 29; j < 57;) {
			if (gameField[i][j] == "")
			{
				a.append("-");
				gameField[i][j] = a;
				j = 58;
			}
			else if (gameField[i][j] != "")
				j++;
		}
	}
}
// хід бота вліво
void Game::BotSideLeft(string stick, int sticknum)
{
	gameField[0][sticknum] = "0";
	if (firstChoose) { prevVarL = stick; prevVarR = stick; stick.append("-");}
	else prevVarL = stick;
	for (int i = 1; i <= 1; i++) {
		for (int j = 28; j >= 0;)
		{
			if (gameField[i][j] == "")
			{
				stick.insert(0, "-");
				gameField[i][j] = stick;
				j = -1;
			}
			else if (gameField[i][j] != "") j--;
		}
	}
}
// хід бота вправо
void Game::BotSideRight(string stick, int sticknum) 
{
	gameField[0][sticknum] = "0";
	if (firstChoose) { prevVarL = stick; prevVarR = stick; }
	else prevVarR = stick;
	for (int i = 1; i <= 1; i++) {
		for (int j = 29; j < 57;) {
			if (gameField[i][j] == "")
			{
				stick.append("-");
				gameField[i][j] = stick;
				j = 57;
			}
			else if (gameField[i][j] != "")
				j++;
		}
	}
}
// вибір ходу
void Game::choosePlace(string a, int ncol, const int nrow = 2)
{
	//Перший хід
	if (firstChoose) 
	{
		prevVarL = a; prevVarR = a;
		a.insert(0, "-");
		a.append("-");
		gameField[1][28] = a;
		gameField[nrow][ncol] = "0";
		firstChoose = false;
	}
	else if (!firstChoose) // інші хода 
	{
		// перевертання фішки
		{
			peretvor = a;
			char temp = peretvor[0];
			peretvor[0] = peretvor[2];
			peretvor[2] = temp;
		}
		// перевірка ходу
		if ((a[2] == prevVarL[0] or peretvor[0] == prevVarR[2]) or (a[0] == prevVarR[2] or peretvor[2] == prevVarL[0]))
		{
			string hidplace;
			cout << "Оберіть сторону (1-ліво, 2-право): "; cin >> hidplace;
			switch (hidplace[0])
			{
			case '1':
				if (a[2] == prevVarL[0] or peretvor[2] == prevVarL[0])
				{
					cout << "Ліво!";
					if (a[2] == prevVarL[0])
					{
						prevVarL = a;
						SideLeft(a, ncol);
					}
					else { prevVarL = peretvor; SideLeft(peretvor, ncol); }
				}
				else { cout << "Цією фішкою не можна походити наліво!\n"; Input(); }
				break;
			case '2':
				if (a[0] == prevVarR[2] or peretvor[0] == prevVarR[2]) 
				{
					cout << "Право!";
					if (a[0] == prevVarR[2]) 
					{
						prevVarR = a;
						SideRight(a, ncol);
					}
					else { prevVarR = peretvor; SideRight(peretvor, ncol); }
				}
				else { cout << "Цією фішкою не можна походити направо!\n"; Input(); }
				break;
			default:
				cout << "Помилка! Повторіть спробу!";
				Sleep(600);
				clear();
				Score();
				cout << "\t\t\t" << points << endl;
				Fieldgame();
				Input();
				break;
			}
		}
		else { cout << "Немає можливого ходу!\n"; Sleep(600); clear(); cout << "\t\t\t" << points << endl; Fieldgame(); Input(); }
	}
}
// Основний вибір
void Game::Input()
{
	string choice;
	choice.reserve(1000);
	tryAgains:
	cout << "Оберіть дію(q - вихід,p-пас ,b-базар, (1-7) - обрати фішку для ходу): "; cin >> choice;
	if (!choice.empty()) 
	{
		switch (choice[0])
		{
		case '1':
			cout << "Обрали 1-у фішку! ";
			if (gameField[2][12] != "0")
			{
				choosePlace(gameField[2][12], 12);
			}
			else if (gameField[2][12] == "0") { cout << "Немає фішки!\n"; goto tryAgains; }
			break;
		case '2':
			cout << "Обрали 2-у фішку!";
			if (gameField[2][13] != "0")
			{
				choosePlace(gameField[2][13], 13);
			}
			else if (gameField[2][13] == "0") { cout << "Немає фішки!\n"; goto tryAgains; }
			break;
		case '3':
			cout << "Обрали 3-у фішку!";
			if (gameField[2][14] != "0")
			{
				choosePlace(gameField[2][14], 14);
			}
			else if (gameField[2][14] == "0") { cout << "Немає фішки!\n"; goto tryAgains; }
			break;
		case '4':
			cout << "Обрали 4-у фішку!";
			if (gameField[2][15] != "0")
			{
				choosePlace(gameField[2][15], 15);
			}
			else if (gameField[2][15] == "0") { cout << "Немає фішки!\n"; goto tryAgains; }
			break;
		case '5':
			cout << "Обрали 5-у фішку!";
			if (gameField[2][16] != "0")
			{
				choosePlace(gameField[2][16], 16);
			}
			else if (gameField[2][16] == "0") { cout << "Немає фішки!\n"; goto tryAgains; }
			break;
		case '6':
			cout << "Обрали 6-у фішку!";
			if (gameField[2][17] != "0")
			{
				choosePlace(gameField[2][17], 17);
			}
			else if (gameField[2][17] == "0") { cout << "Немає фішки!\n"; goto tryAgains; }
			break;
		case '7':
			cout << "Обрали 7-у фішку!";
			if (gameField[2][18] != "0")
			{
				choosePlace(gameField[2][18], 18);
			}
			else if (gameField[2][18] == "0") { cout << "Немає фішки!\n"; goto tryAgains; }
			break;
		case 'b':
			CheckArrAvailableSticks();
			if (bazarFull) cout << "\nВ базарі немає фішок!";
			else 
			{
				bazar();
				Sleep(600);
				clear();
				Score();
				cout << "\t\t\t" << points << endl;
				Fieldgame();
				goto tryAgains;
			}
			break;
		case 'p':
			passAvailable();
			if (!isPass) goto tryAgains;
			else cout << "Пас!";
			break;
		case 'q':
			quit();
			cout << "Ви вийшли з гри!";
			firstCourse = "player";
			break;
		default:
			cout << "Не вірно, повторіть спробу!";
			goto tryAgains;
			break;
		}
	}
}
// Перший хід
void Game::whofirst()
{
	CheckArrAvailableSticks();
	if (firstCourse == "player") {
		cout << "\t\t\t" << points << endl;
		Initialize();
		for (int i = 0; i < 7; ++i)
			gameField[0][(12 + i)] = Botarr01[i];
		Fieldgame();
		Input();
		if (CheckNums != 0) {cout << "\nХід Бота! Зачекайте...." << endl; }
		isrestart = false;
		firstCourse = "bot";
		Sleep(1000);
		clear();
	}
	else if (firstCourse == "bot")
	{
		Initialize();
		Bot();
		Score();
		clear();
		if (isrestart) { clear();}
		else {
			cout << "\t\t\t" << points << endl;
			Fieldgame(); Input();
			if (CheckNums != 0 and firstCourse == "bot") cout << " Хід Бота! Зачекайте...." << endl;
			Score();
		}
	}
}
void Game::Draw()
{
	Menu();
	if(!menu)whofirst();
	isrestart = false;
	Sleep(800);
}
