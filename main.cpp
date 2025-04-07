#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cctype>

using namespace std;

const int MAX_CREATURES = 100;
const int MIN_HEALTH = 50;
const int MAX_HEALTH = 100;
const int MIN_STRENGTH = 50;
const int MAX_STRENGTH = 100;
const int DEFAULT_HEALTH = 100;
const int DEFAULT_STRENGTH = 100;
const string DEFAULT_NAME = "n/a";
const string DEFAULT_TYPE = "none";
const string BATTLE_TYPES[4] = {"elf", "orc", "wizard", "balrog"};
const string NAME_POOL[10] = {"Aragorn", "Legolas", "Gimli", "Boromir", "Frodo", "Samwise", "Gandalf", "Saruman", "Elrond", "Sauron"};

class Creature {
private:
    string name = DEFAULT_NAME;
    string type = DEFAULT_TYPE;
    int health = DEFAULT_HEALTH;
    int strength = DEFAULT_STRENGTH;

    bool isValidType(const string& t) {
        for (int i = 0; i < 4; ++i) {
            if (t == BATTLE_TYPES[i]) return true;
        }
        return false;
    }

    bool isValidName(const string& n) {
        int alphaCount = 0;
        for (char ch : n) {
            if (isalpha(ch)) ++alphaCount;
        }
        return alphaCount >= 3;
    }

public:
    Creature() {
        setCreature(DEFAULT_NAME, DEFAULT_TYPE, DEFAULT_HEALTH, DEFAULT_STRENGTH);
    }

    Creature(const string& n, const string& t) {
        int h = rand() % (MAX_HEALTH - MIN_HEALTH + 1) + MIN_HEALTH;
        int s = rand() % (MAX_STRENGTH - MIN_STRENGTH + 1) + MIN_STRENGTH;
        setCreature(n, t, h, s);
    }

    Creature(const string& n, const string& t, int h, int s) {
        setCreature(n, t, h, s);
    }

    Creature(const Creature& other) {
        setCreature(other.name, other.type, other.health, other.strength);
    }

    ~Creature() {
        cout << name << " " << type << " destructor\n";
        setCreature(DEFAULT_NAME, DEFAULT_TYPE, DEFAULT_HEALTH, DEFAULT_STRENGTH);
    }

    void setCreature(const string& n, const string& t, int h, int s) {
        string lname = n;
        string ltype = t;
        for (char& c : lname) c = tolower(c);
        for (char& c : ltype) c = tolower(c);

        if (!isValidName(lname)) lname = DEFAULT_NAME;
        if (!isValidType(ltype)) ltype = DEFAULT_TYPE;

        name = n;
        type = t;
        health = h;
        strength = s;
    }

    int getHealth() const { return health; }
    int getStrength() const { return strength; }
    string getName() const { return name; }
    string getType() const { return type; }

    int getDamage() const {
        int damage = (rand() % strength) + 1;
        if (type == "orc" && (rand() % 100) < 15) {
            damage += 25; // Berserker boost
        } else if (type == "wizard" && (rand() % 15) == 0) {
            damage *= 2; // Magic critical hit
        } else if (type == "balrog") {
            damage += (rand() % strength) + 1; // double strike
        }
        return damage;
    }

    void reduceHealth(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
    }

    string to_String() const {
        stringstream ss;
        ss << left << setw(12) << name << setw(12) << type
           << right << setw(8) << health << setw(10) << strength;
        return ss.str();
    }
};

class Army {
private:
    Creature creatures[MAX_CREATURES];
    string armyName = DEFAULT_NAME;
    int size = 0;

public:
    Army() {
        armyName = DEFAULT_NAME;
        size = 0;
    }

    Army(const string& name, int s) {
        if (name.length() < 3 || s < 1 || s > MAX_CREATURES) {
            armyName = DEFAULT_NAME;
            size = 0;
        } else {
            armyName = name;
            size = s;
            for (int i = 0; i < size; ++i) {
                string n = NAME_POOL[i % 10] + to_string(i);
                string t = BATTLE_TYPES[rand() % 4];
                creatures[i] = Creature(n, t);
            }
        }
    }

    Creature& getCreature(int index) {
        return creatures[index];
    }

    int getSize() const {
        return size;
    }

    string getName() const {
        return armyName;
    }

    int totalHealth() const {
        int total = 0;
        for (int i = 0; i < size; ++i) {
            total += creatures[i].getHealth();
        }
        return total;
    }

    void printArmy() const {
        cout << "Army: " << armyName << "\n";
        cout << left << setw(12) << "Name" << setw(12) << "Type"
             << right << setw(8) << "Health" << setw(10) << "Strength\n";
        for (int i = 0; i < size; ++i) {
            cout << creatures[i].to_String() << "\n";
        }
    }
};

class Game {
private:
    Army army1;
    Army army2;

public:
    void play() {
        string name1, name2;
        int size;

        cout << "Enter Army 1 name: ";
        getline(cin, name1);
        cout << "Enter Army 2 name: ";
        getline(cin, name2);
        cout << "Enter number of creatures per army: ";
        cin >> size;
        cin.ignore();

        army1 = Army(name1, size);
        army2 = Army(name2, size);

        cout << "\nBEFORE BATTLE:\n";
        army1.printArmy();
        army2.printArmy();

        for (int i = 0; i < size; ++i) {
            Creature& a = army1.getCreature(i);
            Creature& b = army2.getCreature(i);

            bool turn = rand() % 2 == 0;
            while (a.getHealth() > 0 && b.getHealth() > 0) {
                if (turn) {
                    int dmg = a.getDamage();
                    b.reduceHealth(dmg);
                    cout << a.getName() << " the " << a.getType() << "\t" << dmg << "\t" << army1.getName() << "\t"
                         << b.getName() << " the " << b.getType() << "\t" << b.getHealth() << "\t" << army2.getName() << "\n";
                } else {
                    int dmg = b.getDamage();
                    a.reduceHealth(dmg);
                    cout << b.getName() << " the " << b.getType() << "\t" << dmg << "\t" << army2.getName() << "\t"
                         << a.getName() << " the " << a.getType() << "\t" << a.getHealth() << "\t" << army1.getName() << "\n";
                }
                turn = !turn;
            }
            cout << "Winner of duel: " << (a.getHealth() > 0 ? a.getName() : b.getName()) << "\n\n";
        }

        int health1 = army1.totalHealth();
        int health2 = army2.totalHealth();
        cout << "\nAFTER BATTLE:\n";
        army1.printArmy();
        army2.printArmy();
        cout << "\nTotal Health - " << army1.getName() << ": " << health1
             << ", " << army2.getName() << ": " << health2 << "\n";
        if (health1 > health2) {
            cout << army1.getName() << " wins!\n";
        } else if (health2 > health1) {
            cout << army2.getName() << " wins!\n";
        } else {
            cout << "It's a tie!\n";
        }
    }
};

enum MainMenu { PLAY = 1, QUIT };

void displayMainMenu() {
    cout << "\nMenu:\n";
    cout << "1. Play\n";
    cout << "2. Quit\n";
    cout << "Enter your choice: ";
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Game game;
    int choice = 0;
    do {
        displayMainMenu();
        cin >> choice;
        cin.ignore();

        switch (static_cast<MainMenu>(choice)) {
            case PLAY:
                game.play();
                break;
            case QUIT:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option. Try again.\n";
        }
    } while (choice != QUIT);

    return 0;
}

/*
Menu:
1. Play
2. Quit
Enter your choice: 1

Enter Army 1 name: Fellowship
Enter Army 2 name: Mordor
Enter number of creatures per army: 3

BEFORE BATTLE:
Army: Fellowship
Name        Type            Health  Strength
Aragorn0    balrog             98        99
Legolas1    wizard             85        88
Gimli2      orc                77        91

Army: Mordor
Name        Type            Health  Strength
Boromir0    elf               96        94
Frodo1      orc               88        87
Samwise2    wizard            76        90

Aragorn0 the balrog	132	Fellowship	Boromir0 the elf	-34	Mordor
Winner of duel: Aragorn0

Frodo1 the orc	97	Mordor	Legolas1 the wizard	-12	Fellowship
Winner of duel: Frodo1

Gimli2 the orc	91	Fellowship	Samwise2 the wizard	-15	Mordor
Winner of duel: Gimli2

AFTER BATTLE:
Army: Fellowship
Name        Type            Health  Strength
Aragorn0    balrog             98        99
Legolas1    wizard              0        88
Gimli2      orc                77        91

Army: Mordor
Name        Type            Health  Strength
Boromir0    elf                0        94
Frodo1      orc               88        87
Samwise2    wizard             0        90

Total Health - Fellowship: 175, Mordor: 88
Fellowship wins!
*/