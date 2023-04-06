#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Warrior{
    friend ostream &operator<<(ostream &os, const Warrior &warrior) {
        os <<  warrior.name << ": " << warrior.strength;
        return os;
    }

public:
        Warrior(const string& name, double strength) :
                name(name), strength(strength){}

        const string &getName() const {
            return name;
        }

        double getStrength() const {
            return strength;
        }

        void setStrength(double strength) {
            Warrior::strength = strength;
        }

        void setIsHired(bool isHired) { //changes between hired and not hired
            is_hired = isHired;
        }

        bool isHired() const { //to show if a warrior has been hired or not
            return is_hired;
        }

private:
        string name;
        double strength;
        bool is_hired = false; //to show if a warrior has been hired or not
};



class Noble{
    friend ostream &operator<<(ostream &os, const Noble &noble){
        os << noble.name + " has an army of ";
        os << noble.army.size() << endl;
        for(const Warrior* pointer: noble.army){
            os << "\t" + pointer -> getName() + ": ";
            os << pointer -> getStrength() << endl;
        }
        return os;
    }
public:
    Noble(const string &name) : name(name) {}

    bool hire(Warrior& fighter){
        if((is_alive) && !(fighter.isHired())){//noble has to be alive
            army.push_back(&fighter);
            Noble::strength += fighter.getStrength();
            fighter.setIsHired(true);
            return true;
        }
        if(fighter.isHired()){
            cout << fighter.getName() << " is already hired" << endl; //when i put cerr sometimes it was outputted at the top of the output,
            // like it should for the warrior Spock, but sometimes it gave the error message right before the main finsihed running...
        }
        if(!(is_alive)){
            cout << "Dead nobles cannot hire warriors" << endl;
        }
        return false;
    }

    const vector<Warrior*> &getArmy() const {
        return army;
    }

    const string &getName() const {
        return name;
    }

    double getStrength() const {
        return strength;
    }

    void setIsAlive(bool isAlive) {
        is_alive = isAlive;
    }

    bool fire(Warrior& fighter){
        if ((is_alive)){//noble has to be alive
            for (size_t ndx = 0; ndx < (army.size() - 1); ++ndx){
                if (army[ndx] -> getName() == fighter.getName()){//puts fired warrior at the end and then pops
                    Warrior* swap_1 = army[ndx];
                    Warrior* swap_2 = army[ndx + 1];
                    army[ndx + 1] = swap_1;
                    army[ndx] = swap_2;
                }
            }
            if(find_warrior(fighter)){ //warrior has to be available to fire
                army.pop_back();
                Noble::strength -= fighter.getStrength();
                fighter.setIsHired(false);
                cout << "You don't have to work for me anymore " + fighter.getName()
                        + "! -- " + name << endl;
                return true;
            }
            else{
                cout << "Warrior not found!" << endl;
            }
        }
        cout << "Dead nobles cannot fire warriors" << endl;
        return false;
    }

    void set_warrior_strengths(const double& ratio){ //updates the strengths of warriors after battle
        for(Warrior* val: army){
            double war_strength = val -> getStrength();
            val -> setStrength(war_strength - (war_strength * ratio));
        }
    }

    void setStrength(double strength) {
        Noble::strength = strength;
    }

    void battle(Noble& noble){
        cout << name << " battles " << noble.getName() << endl;
        if(!(is_alive) && !(noble.is_alive)){
            cout << "Oh, NO! They're both dead! Yuck!" << endl;
        }
        else if(!(is_alive)){
            cout << "He's dead, " << noble.getName() << endl;
        }
        else if(!(noble.is_alive)){
            cout << "He's dead, " << name << endl;
        }
        else if (strength == noble.getStrength()){
            cout << "Mutual Annihilation: " << name <<" and "
                 << noble.getName() << " die at each other's hands" << endl;
            set_warrior_strengths(1);
            noble.set_warrior_strengths(1);
            strength = 0;
            is_alive = false;
            noble.setStrength(0);
            noble.setIsAlive(false);
        }
        else if (strength > noble.getStrength()){
            cout << name << " defeats " << noble.getName() << endl;
            set_warrior_strengths((noble.getStrength()/strength));
            noble.set_warrior_strengths(1);
            strength = find_strength();
            noble.setStrength(0);
            noble.setIsAlive(false);
        }
        else{
            cout << noble.getName() << " defeats " << name << endl;
            noble.set_warrior_strengths((strength/noble.getStrength())); //lower strength/higher strength
            set_warrior_strengths(1);//by putting 1 it makes the strength of all the warriors zero..
            noble.setStrength(find_strength());//puts updated strength after battle
            strength = 0;
            is_alive = false;
        }
    }

    bool find_warrior(const Warrior& fighter){ //tries to find a warrior inside a noble's army
        for(const Warrior* pointer: army){
            if(pointer -> getName() == fighter.getName() &&
               pointer -> getStrength() == fighter.getStrength()){
                return true;
            }
        }
        return false;
    }

    int find_strength(){ //gets the strength of a noble by adding his warriors' strengths
        int strength_num = 0;
        for(const Warrior* pointer: army){
            strength_num += pointer -> getStrength();
        }
        return strength_num;
    }
private:
    vector <Warrior*> army;
    string name;
    double strength = 0;
    bool is_alive = true; //when the noble is initialized is he considered dead? -QUESTIONNNNN
};

void read_file_vector(ifstream& file, vector <Warrior*>& warriors,
                      vector <Noble*>& nobles);
size_t find_noble(const vector <Noble*>& nobles, const string& name);
size_t find_warrior(const vector <Warrior*>& warriors, const string& name);
void display_army(const vector <Noble*>& nobles,const vector <Warrior*>& warriors);
void clear_all(vector <Noble*>& nobles,vector <Warrior*>& warriors);


int main() {
    vector <Warrior*> warriors;
    vector <Noble*> nobles;
    ifstream file("nobleWarriors.txt");
    if(!file){
        cerr << "file does not open";
        exit(1);
    }
    read_file_vector(file,warriors,nobles);
    file.close();
    return 0;
}

void read_file_vector(ifstream& file, vector <Warrior*>& warriors,
                      vector <Noble*>& nobles){ //CAN U MAKE THESE CONSTS???
    string token;
    string noble_name;
    string noble_2;
    string warrior_name;
    double strength;
    while(file >> token){
        if(token == "Noble"){
            file >> noble_name;
            if(find_noble(nobles, noble_name) == nobles.size()){
                Noble* pointer = new Noble(noble_name);
                nobles.push_back(pointer);
            }
            else{
               cerr << "The Noble " << noble_name << " already exists!" << endl;
            }
        }
        else if(token == "Warrior"){
            file >> warrior_name >> strength;
            if(find_warrior(warriors, warrior_name) == warriors.size()){
                Warrior* pointer = new Warrior(warrior_name,strength);
                warriors.push_back(pointer);
            }
            else{
                cerr << "The Warrior " << warrior_name << "already exists" << endl;
            }
        }
        else if(token == "Hire"){
            file >> noble_name >> warrior_name;
            size_t ndx_1 = find_noble(nobles,noble_name);
            size_t ndx_2 = find_warrior(warriors,warrior_name);
            if(ndx_1 != nobles.size() && ndx_2 != warriors.size()){
                nobles[ndx_1] -> hire(*warriors[ndx_2]);
            }
            if(ndx_1 == nobles.size()){
                cout << noble_name << " does not exist!" << endl;
            }
            if(ndx_2 == warriors.size()){
                cout << warrior_name << " does not exist! " << endl;
            }
        }
        else if(token == "Status"){
            display_army(nobles,warriors);
        }
        else if(token == "Fire"){
            file >> noble_name >> warrior_name;
            size_t ndx_1 = find_noble(nobles,noble_name);
            size_t ndx_2 = find_warrior(warriors,warrior_name);
            if(ndx_1 != nobles.size() && ndx_2 != warriors.size()){
                nobles[ndx_1] -> fire(*warriors[ndx_2]);
            }
            if(ndx_1 == nobles.size()){
                cout << noble_name << " does not exist!" << endl;
            }
            if(ndx_2 == warriors.size()){
                cout << warrior_name << " does not exist! " << endl;
            }
        }
        else if(token == "Battle"){
            file >> noble_name >> noble_2;
            size_t ndx_1 = find_noble(nobles,noble_name);
            size_t ndx_2 = find_noble(nobles,noble_2);
            if(ndx_1 != nobles.size() && ndx_2 != nobles.size()){
                nobles[ndx_1] -> battle(*nobles[ndx_2]);
            }
            if(ndx_1 == nobles.size()){
                cout << noble_name << " does not exist!" << endl;
            }
            if(ndx_2 == nobles.size()){
                cout << noble_2 << " does not exist! " << endl;
            }
        }
        else{
            clear_all(nobles,warriors);
        }
    }
}



size_t find_noble(const vector <Noble*>& nobles, const string& name){
    for(size_t ndx = 0; ndx < nobles.size(); ++ndx){
        if(nobles[ndx] -> getName() == name){
            return ndx;
        }
    }
    return nobles.size();
}

size_t find_warrior(const vector <Warrior*>& warriors, const string& name){
    for(size_t ndx = 0; ndx < warriors.size(); ++ndx){
        if(warriors[ndx] -> getName() == name){
            return ndx;
        }
    }
    return warriors.size();
}

void display_army(const vector <Noble*>& nobles,const vector <Warrior*>& warriors){
    cout << "Status" << endl;
    cout << "======" << endl << "Nobles:" << endl;
    if(nobles.empty()){
        cout << "NONE" << endl;
    }
    else{
        for(const Noble* pointer: nobles){
            cout << *pointer << endl;
        }
    }
    cout << "Unemployed Warriors:" << endl;
    if(warriors.empty()){
        cout << "NONE" << endl;
    }
    else{
        for(const Warrior* pointer: warriors){
            if(!(pointer -> isHired())){
                cout << *pointer << endl;
            }
        }
    }
}

void clear_all(vector <Noble*>& nobles,vector <Warrior*>& warriors){ //remember to pass by reference or it creates a copy....
    for(size_t ndx = 0; ndx < nobles.size(); ++ndx){
        delete nobles[ndx];
    }
    for(size_t ndx = 0; ndx < warriors.size(); ++ndx){
        delete warriors[ndx];
    }
    nobles.clear();
    warriors.clear();
}



