#include <string>
using namespace std;

class Game
{
private:
	void Fieldgame();
	void GetRandNums(int*, int*);
	void Initialize();
	void Bot();
	void GetBestStick();
	void BotSideLeft(string, int);
	void BotSideRight(string, int);
	void whofirst();
	void Input(); 
	void choosePlace(string,int,const int);
    void giveSticks(string);
	void bazar();
	void bazgiveStick(int);
	void Botbazar();
	void CheckArrAvailableSticks();
	void clear();
	void Score();
	void restart();
	void passAvailable();
	void quit();
	void Menu();
	void FileSaving();
	void FileSort(string);
public:
	bool Run();
	void Draw();
};
