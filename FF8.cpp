#include<iostream>
#include<fstream>
#ifdef __FF8_WINDOWS__
	#include<conio.h>
	#define TERMINAL_CLEAR system("cls")
	#define GET_CHAR_FN getch();
#else
	#define TERMINAL_CLEAR system("clear")
	#define GET_CHAR_FN fflush(stdin);getchar();
#endif
#include<string.h>
#include<time.h>
#include<stdlib.h>
using namespace std;
//TODO: Clean up all global vars

#define MAX_NAME_LEN 30

int getInputCharFromConsole()
{
	return GET_CHAR_FN;
}

class Equipment {
	int Rarity;
	int Advantage;
	int Type1;
	int Type2;
	int Type3;
public:
	char Set[20];
	char ReturnRarity() {
		switch(Rarity) {
			case 0: return 'E';
					break;
			case 1: return 'D';
					break;
			case 2: return 'C';
					break;
			case 3: return 'B';
					break;
			case 4: return 'A';
					break;
			case 5: return 'S';
					break;
			default:
					return -1;
					break;
		}
	}
};
class MoveSet{
	int Power;
	int A;
	char Name[MAX_NAME_LEN];
	char Type;
	static int move_count = 0;

public:
	MoveSet(){
		Power=0;
		strcpy(Name,"Has not been unlocked yet");
		Type='O';
		A=0;
	}
	
	void MoveChange(int Pow,char Xame[],int typecheck){
		Power=Pow;
		strcpy(Name,Xame);
		if(typecheck==1){
			Type='M';
		}
		if(typecheck==2){
			Type='P';
		}
		A=1;
		move_count++;
	}
	void MoveName(char Xame[]){
		strcpy(Name,Xame);
		A=1;
	}
	void MovePower(int Pow){
		Power=Pow;
	}
	void MoveDisplay(int x){
		if(Type=='M'){
			cout<<x<<". "<<Name<<"\t"<<"Magical Move"<<endl;
			cout<<Power<<endl;	
		}
		if(Type=='P'){
			cout<<x<<". "<<Name<<"\t"<<"Physical Move"<<endl;
			cout<<Power<<endl;	
		}
	}
	int GetPow(){
		return Power;
	}
	char GetType(){
		return Type;
	}
	int GetExistence(){
		return A;
	}		
};

class Player {
	char name[MAX_NAME_LEN];

	protected:
		CharacterBehavior baseBehavior;

	public:
		Player() {};
		void setName(char *in_name) {
			memset(name, 0, MAX_NAME_LEN);
			strncpy(name, in_name, MAX_NAME_LEN-1);
		}
		char *name() {
			return name;
		}
		int remainingHealth() {
			return (base.hitPoints() - base.damage());
		}
		bool alive() {
			return (remainingHealth() > 0);
		}
		void revive() {
			base.setDamage(0);
		}		
}

class Opponent : public Player {
	private:
		ComputerPlayerBehavior compBehavior;

	public:
		void init(int x) {
			char chapterNumStr[5] = {0};
			itoa(x, chapterNumStr, 10);
			char fileName[MAX_NAME_LEN] = {0};
			strcat(fileName, "opp_chapter_");
			strcat(fileName, chapterNumStr);
			strcat(fileName, ".dat");
			compBehavior.init(x, fileName, baseBehavior);
		}

		CharacterBehavior& characterBehavior() {
			return baseBehavior;
`		}
};

class ComputerPlayerBehavior {
	public:
		void init(int chapter, char *fileName, CharacterBehavior &base) {
			//ifstream O(fileName);
			//TODO: Serialize and de-serialize using CSV instead of binary
			//read from file in compPlayerBehavior, pass filename.

			//generate randomised opponent behavior metrics
			srand(time(0));
			int M=25*chapter;
			base.changeHitPoints(rand()%M);
			base.changePhysicalAttack(rand()%M);
			base.changePhysicalDefense(rand()%M);
			base.changeMagicalAttack(rand()%M);
			base.changeMagicalDefense(rand()%M);
			base.changeSpeed(rand()%M);
			base.changeEvasion(rand()%M);
			base.changeCriticalChance(rand()%M);
			base.changeAccuracy(rand()%M);
		}
};

class HumanPlayer : public Player {
	private:
		HumanPlayerBehavior humanBehavior;
		
	public:
		void setHumanBehavior(int playerRole) {
			humanBehavior.Charchange(playerRole);
		}

		void display() {
			TERMINAL_CLEAR;
			cout<<this->name()<<endl;
			humanBehavior.CharDisplay();
			cout<<"Stats"<<endl;
			baseBehavior.CharDisplay();
			getInputCharFromConsole();
		}

		void updateStats() {
			humanBehavior.StatUp(baseBehavior);
		}

		void battle(Player& opponent) {
			humanBehavior.MoveCheck(opponent.characterBehavior());
		}
};

class HumanPlayerBehavior {
	private:
		int Skill;
		char Class[10];
	public:
		Equipment Helm;
		Equipment Chestpiece;
		Equipment Boots;
		Equipment Weapon;
		Equipment Enhance;
		MoveSet Move[4];
		int Chapter;

		void CharDisplay(){
			cout<<Class<<endl;
			cout<<"Skill points left: "<<Skill<<endl;
		}

		void Charchange(int M, CharacterBehavior &base){
			char Name[20];
			base.setSkill(20);
			base.setHitPoints(40);
			base.setPhysicalAttack(20);
			base.setPhysicalDefense(20);
			base.setMagicalAttack(20);
			base.setMagicalDefense(20);
			base.setAccuracy(50);
			base.setEvasion(10);
			base.setSpeed(20);
			base.setCriticalChance(5);

			if(M==1){
				base.changeSpeed(10);
				base.changeEvasion(10);
				base.changeAccuracy(10);
				strcpy(Class,"Archer");
				strcpy(Name,"Flint Arrow");
				Move[0].MoveChange(25,Name,'2');
				strcpy(Name,"Vanisher");
				Move[1].MoveChange(25,Name,'1');
			}
			if(M==2){
				base.changePhysicalAttack(15);
				base.changePhysicalDefense(15);
				base.changeSpeed(5);
				strcpy(Class,"Brawler");
				strcpy(Name,"Punch");
				Move[0].MoveChange(30,Name,'2');
				strcpy(Name,"Zen Punch");
				Move[1].MoveChange(20,Name,'1');
			}
			if(M==3){
				base.changeMagicalAttack(15);
				base.changeMagicalDefense(15);
				base.changeCriticalChance(10);
				strcpy(Class,"Wizard");
				strcpy(Name,"Physical Hymn");
				Move[0].MoveChange(20,Name,'2');
				strcpy(Name,"Burn");
				Move[1].MoveChange(30,Name,'1');
			}
		}

		void StatUp(CharacterBehavior &base) {
			int x,y,z;
			while(Skill>0){
				TERMINAL_CLEAR;
				cout<<"Skill Points Left: "<<Skill<<endl;
				cout<<"Choose a stat you would like to upgrade."<<endl;	
				cout<<" 1.Hit Points \n 2.Physical Attack \n 3.Physical Defense"<<endl;
				cout<<" 4.Magic Attack \n 5.Magic Defense \n 6.Accuracy"<<endl;	
				cout<<" 7.Speed \n 8.Evasion \n 9.Critical Chance"<<endl;	
				cout<<" Enter 10 to quit and save your skill points."<<endl;
				cin>>x;
				cout<<"Choose the number of skill points to be used"<<endl;
				cin>>y;
				if(Skill>=y){
					switch(x){
						case 1:	base.changeHitPoints(y*5);
							break;
						case 2: base.changePhysicalAttack(y*2);
							break;
						case 3:	base.changePhysicalDefense(y*2);
							break;
						case 4: base.changeMagicalAttack(y*2);
							break;
						case 5:	base.changeMagicalDefense(y*2);
							break;
						case 6: base.changeAccuracy(y*2);
							break;
						case 7:	base.changeSpeed(y*3);
							break;
						case 8: base.changeEvasion(y);
							break;
						case 9:	base.changeCriticalChance(y*5);
							break;
						case 10:z=1;
							break;
					}
					if(z!=1){
						Skill-=y;	
					}
				}
				if(z==1){
					break;
				}
			}
		}

		void setOpponentDamage(int choice, CharacterBehavior& M) {
			int DMG;
			if(Move[choice-1].GetType()=='M'){
				Mrate = base.magicalAttack()/M.magicalDefense();
			} else {
				Mrate = base.physicalAttack()/M.physicalDefense();
			}
			DMG=(Move[choice-1].GetPow())*Mrate;
			M.changeDamage(DMG);
		}

		void setOwnDamage(CharacterBehavior& M) {
			int DMG;
			srand(time(0));
			int A=rand();
			Mrate=(M.physicalAttack()+M.magicalAttack())/(base.magicalDefense()+base.physicalDefense());
			DMG=Mrate*(A%30);
			changeDamage(DMG);
		}

		void MoveCheck(CharacterBehavior &M) {
			for(int i=0;Move[i].GetExistence()!=0;i++){
				Move[i].MoveDisplay(i+1);
			}
			int choice = 0;
			cin >> choice;
			bool ownAttackFirst = false;
			float Mrate;
			if(base.speed() > M.speed()){
				setOpponentDamage(choice, M);
				ownAttackFirst = true;
			} else {
				setOwnDamage(M);
			}
			if(ownAttackFirst) {
				setOwnDamage();
			} else {
				setOpponentDamage();
			}
		}
};

class CharacterBehavior {
	private:
		int HP;
		int Patt,Pdef,Matt,Mdef;
		int Acc,Eva,Sp,Crit;
		int damage;

	public:	
		CharacterBehavior(){}
		int damage() {
			return damage;
		}
		int hitPoints() {
			return HP;
		}
		int physicalAttack() {
			return Patt;
		}
		int physicalDefense() {
			return Pdef;
		}
		int magicalAttack() {
			return Matt;
		}
		int magicalDefense() {
			return Mdef;
		}
		int accuracy() {
			return Acc;
		}
		int evasion() {
			return Eva;
		}
		int speed() {
			return Sp;
		}
		int criticalChance() {
			return Crit;
		}
		void setDamage(int dmg) {
			damage = dmg;
		}
		void setHitPoints(int hitPoints) {
			HP = hitPoints;
		}
		void setPhysicalAttack(int val) {
			Patt = val;
		}
		void setPhysicalDefense(int val) {
			Pdef = val;
		}
		void setMagicalAttack(int val) {
			Matt = val;
		}
		void setMagicalDefense(int val) {
			Mdef = val;
		}
		void setAccuracy(int val) {
			Acc = val;
		}
		void setEvasion(int val) {
			Eva = val;
		}
		void setSpeed(int val) {
			Sp = val;
		}
		void setCriticalChance(int val) {
			Crit = val;
		}
		void changeDamage(int dmg) {
			damage += dmg;
		}
		void changeHitPoints(int hitPoints) {
			HP += hitPoints;
		}
		void changePhysicalAttack(int val) {
			Patt += val;
		}
		void changePhysicalDefense(int val) {
			Pdef += val;
		}
		void changeMagicalAttack(int val) {
			Matt += val;
		}
		void changeMagicalDefense(int val) {
			Mdef += val;
		}
		void changeAccuracy(int val) {
			Acc += val;
		}
		void changeEvasion(int val) {
			Eva += val;
		}
		void changeSpeed(int val) {
			Sp += val;
		}
		void changeCriticalChance(int val) {
			Crit += val;
		}
		void CharDisplay(){
			cout<<"Health: "<<HP<<endl;
			cout<<"Physical Attack: "<<Patt<<endl;
			cout<<"Physical Defense: "<<Pdef<<endl;
			cout<<"Magical Attack: "<<Matt<<endl;
			cout<<"Magical Defense: "<<Mdef<<endl;
			cout<<"Speed: "<<Sp<<endl;
			cout<<"Evasion: "<<Eva<<endl;
			cout<<"Accuracy: "<<Acc<<endl;
			cout<<"Critical: "<<Crit<<endl;
		}
};

class Battle {
	void run(HumanPlayer &protagonist){
		Opponent antagonist;
		computerPlayer.init();
		//TODO: Fix calling Chapter below.
		antagonist = Opp_Gen(protagonist.Chapter);
		antagonist.setName("bot");
		while(protagonist.alive() && antagonist.alive()) {
			TERMINAL_CLEAR;
			cout<<antagonist.name()<<"\t\t\t"<<protagonist.name()<<endl;
			cout<<antagonist.remainingHealth()<<"\t\t\t"<<protagonist.remainingHealth()<<endl;
			cout<<"Enter the move you would like to use"<<endl;
			protagonist.battle(antagonist);
			cout<<antagonist.remainingHealth()<<"\t\t\t"<<protagonist.remainingHealth()<<endl;
			//Moves need to be done in Unlockable Sequence
			getInputCharFromConsole();
		}
		if(antagonist.alive()) {
			cout<<"You Win";
		}
		if(protagonist.alive()) {
			cout<<"You Lose";
		}
		protagonist.revive();
		getInputCharFromConsole();
	}
}

class FF8Game {
	private:
		bool newAccount;
	public:
		void FileSave(Character X){
			ofstream Save("Savefile.dat",ios::app);
			Save.write((char*)&X,sizeof(X));
			Save.close();
		}

		void FileRead(HumanPlayer &X){
			HumanPlayer M;
			char x;
			ifstream Save("Savefile.dat",ios::out);
			while(Save.read((char*)&M,sizeof(M))){
				M.display();
				cout<<"Is this your savefile?(y/n)"<<endl;
				cin>>x;
				if(x=='y'){
					X=M;
					newAccount = false;
					break;		
				}
			}
			Save.close();
		}

		void StoryCheck(int x, Character X){
			ifstream Story("Story.txt");
			char Text[300];
			while(Story.getline(Text,300)){
				cout<<Text<<endl;
				if(Text[0]=='#'){
					X.Chapter++;
				}
				if(Text[0]==';'){

				}
				getInputCharFromConsole();	
			}
			getInputCharFromConsole();
		}

		static void start() {
			HumanPlayer LeadChar;
			FileRead(LeadChar);
			if(newAccount){
				TERMINAL_CLEAR;
				cout<<"Welcome to the Game. What character would you like to play?"<<endl;
				cout<<"1.Archer \n A fast and evasive class with an eye for details."<<endl;
				cout<<"2.Brawler \n Get ready to pack a punch with this class."<<endl;
				cout<<"3.Wizard \n A magical class of humans who took learning too far"<<endl;
				int choice;
				cin>>choice;
				cout<<"Name of Character?"<<endl;
				char temp_name[MAX_NAME_LEN] = {0};
				cin>>temp_name;
				LeadChar.setName(temp_name);
				LeadChar.setHumanBehavior(choice);
				LeadChar.display();
				LeadChar.StatUp();
				FileSave(LeadChar);
			}
			LeadChar.display();
			TERMINAL_CLEAR;
			StoryCheck(X.Chapter,X);
		}
};

int main() {
	FF8Game::start();
	return 0;
}

