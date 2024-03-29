#include <iostream>
#include <fstream>
#include <stdio.h>
#ifdef WIN32
    #include<conio.h>
    #define TERMINAL_CLEAR system("cls")
    #define GET_CHAR_FN return getch();
#else
    #define TERMINAL_CLEAR system("clear")
    #define GET_CHAR_FN return cin.get();
#endif
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

#define MAX_NAME_LEN 30

/*
Storyline for the game is read from an external file - Story.txt in the same
folder as the executable. Each line signifies a statement in the story. Lines
starting with special characters are used to control flow, as follows:
$ --> initiate battle sequence
# --> mark end of chapter and save
+ --> move to next chapter
*/

int getInputCharFromConsole()
{
    cout << "\n\n\n\n\n\n\t\t\t\t\t\tPress any key to continue...";
    GET_CHAR_FN;
}

/*
TODO: support for equipments
TODO: shopping using gold
TODO: support for player levels based on experience
*/

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
    static int move_count;

    public:
    MoveSet(){
        Power=0;
        strcpy(Name,"Has not been unlocked yet");
        Type='O';
        A=0;
    }

    void MoveChange(int Pow, char Xame[], int typecheck){
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
        } else if(Type=='P') {
            cout<<x<<". "<<Name<<"\t"<<"Physical Move"<<endl;
        }
        cout<<"Power: " << Power<<endl; 
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

int MoveSet::move_count = 0;

class CharacterBehavior {
    private:
        int HP;
        int Patt,Pdef,Matt,Mdef;
        int Acc,Eva,Sp,Crit;
        int damagePoints;

    public: 
        CharacterBehavior(){
            HP = 0;
            Patt = Pdef = Matt = Mdef = 0;
            Acc = Eva = Sp = Crit = damagePoints = 0;
        }
        int damage() {
            return damagePoints;
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
            damagePoints = dmg;
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
            damagePoints += dmg;
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
        /*
           Supposed to be a behaviour of a character. Hence, it is declared 
           here. Also used in battle sequence to fix a bug that did not factor
           in speed.
           */
        bool playeralive(){ 
            return ((HP-damagePoints)>0);
        }
};

class Player {
    char _name[MAX_NAME_LEN];

    protected:
    CharacterBehavior base;

    public:
    Player() {};
    void setName(char *in_name) {
        memset(_name, 0, MAX_NAME_LEN);
        strncpy(_name, in_name, MAX_NAME_LEN-1);
    }
    char *name() {
        return _name;
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
    CharacterBehavior& characterBehavior() {
        return base;
    }
};

class ComputerPlayerBehavior {
    private:
        int readParamFromFile(ifstream &in)
        {
            //read and convert to int.
            char param_c[8];
            memset(param_c, 0, 8);
            in.getline(param_c, 3, ','); 
            stringstream param_str;
            param_str << param_c;
            unsigned int param_i = 0;
            param_str >> param_i;
            return param_i;
        }

    public:
        void init(int chapter, char *fileName, CharacterBehavior &base) {
            /*
               CSV format is as follows:

               HitPoints,PhysicalAttack,PhysicalDefence,MagicalAttack,
               MagicalDefence,Speed, Evasion,CriticalChance,Accuracy

               Each value can go up to 999 (3 digits). Any change in limits
               should be evaluated against the size so that there is no
               overflow.
               */
            ifstream in(fileName);
            base.setHitPoints(readParamFromFile(in));
            base.setPhysicalAttack(readParamFromFile(in));
            base.setPhysicalDefense(readParamFromFile(in));
            base.setMagicalAttack(readParamFromFile(in));
            base.setMagicalDefense(readParamFromFile(in));
            base.setSpeed(readParamFromFile(in));
            base.setEvasion(readParamFromFile(in));
            base.setCriticalChance(readParamFromFile(in));
            base.setAccuracy(readParamFromFile(in));
            in.close();

            //generate randomised opponent behavior metrics
            cout << "File" << fileName << endl;
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

            base.CharDisplay();
        }
};

class HumanPlayerBehavior {
    private:
        int Skill;
        char Class[10];
        MoveSet Move[4];
    public:
        Equipment Helm;
        Equipment Chestpiece;
        Equipment Boots;
        Equipment Weapon;
        Equipment Enhance;

        void CharDisplay(){
            cout<<Class<<endl;
            cout<<"Skill points left: "<<Skill<<endl;
        }

        void Charchange(int M, CharacterBehavior &base){
            char Name[20];
            base.setHitPoints(40);
            base.setPhysicalAttack(20);
            base.setPhysicalDefense(20);
            base.setMagicalAttack(20);
            base.setMagicalDefense(20);
            base.setAccuracy(50);
            base.setEvasion(10);
            base.setSpeed(20);
            base.setCriticalChance(5);
            Skill = 20;

            if(M==1){
                base.changeSpeed(10);
                base.changeEvasion(10);
                base.changeAccuracy(10);
                strcpy(Class,"Archer");
                strcpy(Name,"Flint Arrow");
                Move[0].MoveChange(25, Name, 2);
                strcpy(Name,"Vanisher");
                Move[1].MoveChange(25, Name, 1);
            }
            if(M==2){
                base.changePhysicalAttack(15);
                base.changePhysicalDefense(15);
                base.changeSpeed(5);
                strcpy(Class,"Brawler");
                strcpy(Name,"Punch");
                Move[0].MoveChange(30, Name, 2);
                strcpy(Name,"Zen Punch");
                Move[1].MoveChange(20, Name, 1);
            }
            if(M==3){
                base.changeMagicalAttack(15);
                base.changeMagicalDefense(15);
                base.changeCriticalChance(10);
                strcpy(Class,"Wizard");
                strcpy(Name,"Physical Hymn");
                Move[0].MoveChange(20, Name, 2);
                strcpy(Name,"Burn");
                Move[1].MoveChange(30, Name, 1);
            }
        }

        void StatUp(CharacterBehavior &base) {
            int x = 0, y = 0, z = 0;
            while(Skill>0){
                TERMINAL_CLEAR;
                cout<<"Skill Points Left: "<<Skill<<endl;
                cout<<"Choose a stat you would like to upgrade."<<endl; 
                cout<<" 1.Hit Points \n 2.Physical Attack \n ";
                cout<<"3.Physical Defense"<<endl;
                cout<<" 4.Magic Attack \n 5.Magic Defense \n 6.Accuracy"<<endl; 
                cout<<" 7.Speed \n 8.Evasion \n 9.Critical Chance"<<endl;   
                cout<<" Enter 10 to quit and preserve your skill points."<<endl;
                cin>>x;
                if ((x > 10) || (x < 1)) { 
                    continue;
                }
                if (x < 10) {
                    cout<<"Choose the number of skill points to be used"<<endl;
                    cin>>y;
                }
                if(Skill>=y){
                    switch(x){
                        case 1: base.changeHitPoints(y*5);
                                break;
                        case 2: base.changePhysicalAttack(y*2);
                                break;
                        case 3: base.changePhysicalDefense(y*2);
                                break;
                        case 4: base.changeMagicalAttack(y*2);
                                break;
                        case 5: base.changeMagicalDefense(y*2);
                                break;
                        case 6: base.changeAccuracy(y*2);
                                break;
                        case 7: base.changeSpeed(y*3);
                                break;
                        case 8: base.changeEvasion(y);
                                break;
                        case 9: base.changeCriticalChance(y*5);
                                break;
                        case 10:
                                //flow through
                        default:
                                z = 1;
                                break;
                    }
                    if(z == 1) {
                        break;
                    } else { 
                        Skill-=y;   
                    }
                }
            }
        }

        void setOpponentDamage(int choice, CharacterBehavior& base,
                CharacterBehavior& Opponent) {
            int DMG;
            bool miss=false;
            float Mrate;
            srand(time(0));
            int A=rand();
            char type = Move[choice-1].GetType();
            if(type == 'M') {
                Mrate =
                    (float)(base.magicalAttack())
                    /(float)(Opponent.magicalDefense());
            } else if (type == 'P') {
                Mrate =
                    (float)(base.physicalAttack())
                    /(float)(Opponent.physicalDefense());
            }
            DMG=(Move[choice-1].GetPow())*Mrate;
            if(Opponent.evasion() > base.accuracy()){
                if(A%3 == 0){
                    miss = true;
                }
                else{
                    miss = false;
                }
            }
            else if(Opponent.evasion() == base.accuracy()){
                if(A%5 == 0){
                    miss = true;
                }
                else{
                    miss = false;
                }
            }
            else{
                if(A%7 == 0){
                    miss = true;
                }
                else{
                    miss = false;
                }   
            }
            if ((rand()%base.criticalChance() > 40) ||
                    (rand()%base.criticalChance() < 2)) {
                DMG=DMG*1.5;
                cout<<"Let's Go.. A critical hit."<< endl;
            }
            if(miss==true){
                cout<<"Your attack missed."<< endl;
            }
            else{
                Opponent.changeDamage(DMG);
                cout<<"You just dealt " << DMG <<" damage"<<endl;
            }
        }

        void setOwnDamage(CharacterBehavior& base,
                CharacterBehavior& Opponent) {
            int DMG;
            float Mrate;
            bool miss;
            srand(time(0));
            Mrate = 
                (float)(Opponent.physicalAttack()+Opponent.magicalAttack())
                /(float)(base.magicalDefense()+base.physicalDefense());
            DMG=Mrate*(rand()%30);
            if(base.evasion() > Opponent.accuracy()){
                if(rand()%3 == 0){
                    miss = true;
                }
                else{
                    miss = false;
                }
            }
            else if(base.evasion() == Opponent.accuracy()){
                if(rand()%5 == 0){
                    miss = true;
                }
                else{
                    miss = false;
                }
            }
            else{
                if(rand()%7 == 0){
                    miss = true;
                }
                else{
                    miss = false;
                }   
            }
            if ((rand()%Opponent.criticalChance() > 40) ||
                    (rand()%Opponent.criticalChance() < 2)) {
                DMG=DMG*1.5;
                cout<<"Oh no.. A critical hit strikes."<< endl;
            }
            if(miss==true){
                cout<<"Lucky, they missed."<< endl;
            }
            else{
                base.changeDamage(DMG);
                cout<<"Your Opponent just dealt " << DMG <<" damage"<<endl;
            }

        }

        void MoveCheck(CharacterBehavior &base, CharacterBehavior &Opponent) {
            for(int i=0;Move[i].GetExistence()!=0;i++){
                Move[i].MoveDisplay(i+1);
            }
            int choice = 0;
            cin >> choice;
            //read '\n'
            cin.get();
            bool ownAttackFirst = false;
            if(base.speed() > Opponent.speed()){
                cout<<"Faster than your opponent, you strike first."<< endl;
                setOpponentDamage(choice, base, Opponent);
                ownAttackFirst = true;
            } else {
                setOwnDamage(base, Opponent);
            }
            if(ownAttackFirst && Opponent.playeralive()) {
                setOwnDamage(base, Opponent);
            } else if (!ownAttackFirst && base.playeralive()) {
                setOpponentDamage(choice, base, Opponent);
            }
        }
};

class HumanPlayer : public Player {
    private:
        HumanPlayerBehavior humanBehavior;
        int _chapter;

    public:
        HumanPlayer() {
            _chapter = 1;
        }

        void setHumanBehavior(int playerRole) {
            humanBehavior.Charchange(playerRole, base);
        }
        void display(bool wait) {
            TERMINAL_CLEAR;
            cout<<this->name()<<endl;
            humanBehavior.CharDisplay();
            cout<<"Stats"<<endl;
            base.CharDisplay();
            if (wait) {
                getInputCharFromConsole();
            }
        }

        void updateStats() {
            humanBehavior.StatUp(base);
        }

        void battle(Player& opponent) {
            humanBehavior.MoveCheck(base, opponent.characterBehavior());
        }

        void moveToNextChapter() {
            _chapter++;
        }

        int chapter() {
            return _chapter;
        }
};

class Opponent : public Player {
    private:
        ComputerPlayerBehavior compBehavior;

    public:
        void init(int x) {
            char fileName[MAX_NAME_LEN];
            memset(fileName, 0, MAX_NAME_LEN);
            strcat(fileName, "opp_chapter_");
            ostringstream oss;
            oss << x;
            strcat(fileName, oss.str().c_str());
            strcat(fileName, ".dat");
            compBehavior.init(x, fileName, base);
        }
};

class Battle {
    public:
        static bool run(HumanPlayer &protagonist){
            bool continueGame;
            Opponent antagonist;
            antagonist.init(protagonist.chapter());
            antagonist.setName((char*)"bot");
            cout << protagonist.remainingHealth() << " "
                << antagonist.remainingHealth() << "\n";
            while(protagonist.alive() && antagonist.alive()) {
                TERMINAL_CLEAR;
                cout<<antagonist.name()<<"\t\t\t"<<protagonist.name()<<endl;
                cout<<antagonist.remainingHealth()<<"\t\t\t"
                    <<protagonist.remainingHealth()<<endl;
                cout<<"Enter the move you would like to use"<<endl;
                protagonist.battle(antagonist);
                cout<<antagonist.remainingHealth()<<"\t\t\t"
                    <<protagonist.remainingHealth()<<endl;
                if(!antagonist.alive()) {
                    cout<<"\nYou Win\n";
                    continueGame = true;
                    break;
                }
                if(!protagonist.alive()) {
                    continueGame = false;
                    cout<<"\nYou Lose\n";
                    break;
                }
                //Moves need to be done in unlockable Sequence
                getInputCharFromConsole();
            }
            protagonist.revive();
            getInputCharFromConsole();
            return continueGame;
        }
};

class FF8Game {
    public:
        static void FileSave(HumanPlayer& X, bool oldAccount=true){
            ofstream Save("Savefile.dat",ios::app);
            if(!oldAccount){
                Save.write((char*)&X,sizeof(X));
                Save.close();
            }
            else{
                ofstream tempSave("tempSavefile.dat",ios::app);
                tempSave.write((char*)&X,sizeof(X));
                HumanPlayer tempPlayer;
                ifstream SavedFile("Savefile.dat",ios::beg);
                while(SavedFile.read((char*)&tempPlayer,sizeof(tempPlayer))){
                    if(strcmp(X.name(),tempPlayer.name())){
                        tempSave.write((char*)&tempPlayer,sizeof(tempPlayer));
                    }
                }
                SavedFile.close();
                tempSave.close();
                Save.close();
                if (remove("Savefile.dat") == 0) {
                    rename("tempSavefile.dat","Savefile.dat");
                } else {
                    cout << "Error: could not save the game." << endl;
                    getInputCharFromConsole();
                }
            }
        }
        static bool FileRead(HumanPlayer &X){
            bool newAccount = true;
            HumanPlayer M;
            char x;
            ifstream SavedFile("Savefile.dat",ios::out);
            while(SavedFile.read((char*)&M,sizeof(M))){
                M.display(false);
                cout<<"Is this your savefile?(y/n)"<<endl;
                cin>>x;
                if(x=='y'){
                    X=M;
                    newAccount = false;
                    break;      
                }
            }
            SavedFile.close();
            return newAccount;
        }

        static void StoryCheck(HumanPlayer& X){
            ifstream Story("Story.txt");
            char Text[300];
            int chapter = 0;
            while(Story.getline(Text,300)){
                TERMINAL_CLEAR;
                if(chapter == X.chapter()){
                    if(Text[0]=='#') {
                        X.moveToNextChapter();
                        FileSave(X);
                    }
                    else if (Text[0]=='$') {
                        cout<<"Saving the game"<<endl;
                        getInputCharFromConsole();
                        for(;;){
                            if(Battle::run(X)){
                                break;
                            }
                            else{
                                TERMINAL_CLEAR;
                                cout<<"Everything fades to black";
                                getInputCharFromConsole();
                                char choice;
                                for(;;){
                                    TERMINAL_CLEAR;
                                    cout<<"Do you want to replay from the ";
                                    cout<<"previous savefile? (y/n)"<<endl;
                                    cin>>choice;
                                    if(choice=='n'||choice=='N'){
                                        exit(0);
                                    }
                                    else if(choice=='y'||choice=='Y'){
                                        break;
                                    }
                                    else{
                                        cout<<"Enter a valid choice. (y/n)"
                                            << endl;
                                        getInputCharFromConsole();
                                    }
                                }
                            }
                        }
                    }
                    else {
                        cout<<Text;
                        getInputCharFromConsole();  
                    }           
                }
                if(Text[0] == '+'){
                    chapter++;
                }
            }            
        }

        static void start() {
            HumanPlayer LeadChar;
            if (FileRead(LeadChar)) {
                //new account
                TERMINAL_CLEAR;
                cout<<"Welcome to the Game. ";
                cout<<"What character would you like to play?"<<endl;
                cout<<"1.Archer \n ";
                cout<<"A fast and evasive class with an eye for details."<<endl;
                cout<<"2.Brawler \n Get ready to pack a punch with this class."
                    <<endl;
                cout<<"3.Wizard \n ";
                cout<<"A magical class of humans who took learning too far"
                    <<endl;
                int choice;
                cin>>choice;
                cout<<"Name of Character?"<<endl;
                char temp_name[MAX_NAME_LEN];
                memset(temp_name, 0, MAX_NAME_LEN);
                //read \n
                cin.get();
                cin.getline(temp_name, MAX_NAME_LEN);
                LeadChar.setName(temp_name);
                LeadChar.setHumanBehavior(choice);
                LeadChar.display(true);
                LeadChar.updateStats();
                FileSave(LeadChar,false);
            }
            LeadChar.display(true);
            TERMINAL_CLEAR;
            StoryCheck(LeadChar);
        }
};

int main() {
    FF8Game::start();
    return 0;
}
