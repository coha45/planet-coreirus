#include <iostream>
#include <cstdint>
#include <vector>
#include <windows.h>
using namespace std;

void set_cursor_pos(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

string display_bool(bool v) {
    if (v) return "True";
    else return "False";
}

enum class Diet : uint8_t {
    HERBIVORE,
    CARNIVORE,
    OMNIVORE
};

inline ostream& operator<<(ostream& os, Diet d) {
    switch(d) {
        case Diet::HERBIVORE: os << "Herbivore"; break; 
        case Diet::CARNIVORE: os << "Carnivore"; break; 
        case Diet::OMNIVORE: os << "Omnivore"; break;
        default: os << "This shouldn't happen.";
    }
}

enum class Tier : uint8_t {
    TINY = 1,
    SMALL,
    MEDIUM,
    LARGE,
    ENORMOUS,
};

inline ostream& operator<<(ostream& os, Tier tier) {
    switch(tier) {
        case Tier::TINY: os << "Tiny"; break;
        case Tier::SMALL: os << "Small"; break;
        case Tier::MEDIUM: os << "Medium"; break;
        case Tier::LARGE: os << "Large"; break;
        case Tier::ENORMOUS: os << "Enormous"; break;
        default: "This shouldn't happen";
    }
    return os;
}

class Creature {
    private: 
        static inline int creature_count = 0;

        Tier get_tier(int weight) {
            if (weight >= 25000) {
                return Tier::ENORMOUS;
            } else if(weight >= 4500) {
                return Tier::LARGE;
            } else if(weight >= 500) {
                return Tier::MEDIUM;
            } else if(weight >= 100) {
                return Tier::SMALL;
            } else if(weight >= 50) {
                return Tier::TINY;
            } else {
                throw runtime_error("Weight must be greater than 0.");
            }
        }
    public:
        string name {};
        Diet diet_type {};
        int base_health {};
        int base_atk {};
        int adult_weight {};
        int mtbl_rate {};
        Tier tier {};

        Creature() = default;

        Creature(string n, Diet dt, int bh, int ba, int w, int mtbl_r) {
            name = n;
            diet_type = dt;
            base_health = bh;
            base_atk = ba;
            adult_weight = w;
            mtbl_rate = mtbl_r;
            tier = get_tier(adult_weight);

            creature_count++;
        }

        void display_stats() {
            cout << "SURVIVAL\n";
            cout << "\tName: " << name << "\n";
            cout << "\tHealth: " << base_health << "\n";
            cout << "\tAttack: " << base_atk << "\n";
            cout << "\tDiet: " << diet_type << "\n";
            cout << "MULTIPLIERS\n";
            cout << "\tWeight (lbs): " << adult_weight << "\n";
            cout << "\tTier: " << tier << "\n";
            cout << "\tMetabolic Rate: " << mtbl_rate << "\n";
        }
        
    };

// Class to handle age-related attributes.
// The larger the creature, the slower it'll age.
class Age {
    private: 
        int age_freq {};
        int age {};
        int age_ctr {};

    public:
        Age() = default;
        Age(Tier tier) {
            switch (tier){
                case Tier::ENORMOUS:
                    age_freq = 5;
                    break;
                case Tier::LARGE:
                    age_freq = 4;
                    break;
                case Tier::MEDIUM:
                    age_freq = 3;
                    break;
                case Tier::SMALL:
                    age_freq = 2;
                    break;
                case Tier::TINY:
                    age_freq = 1;
                    break;
                default:
                    throw runtime_error("Invalid age tier.");
            }
        }

        void advance_age() {
            age_ctr++;
            if(age_ctr >= age_freq) {
                age++;
                age_ctr = 0;
            }
        }

        int get_age() {
            return age;
        }
};

class Player {
    private:
        static inline int base_hunger = 100, base_thirst = 100, base_stamina = 100;
        Creature cur_creature {};

    public:
        Age age {};
        int weight {};
        int health {};
        int hunger {};
        int thirst {};
        bool is_alive {};
        
        Player() = default;

        Player(Creature c) {
            cur_creature = c;
            health = cur_creature.base_health;
            hunger = base_hunger;
            thirst = base_thirst;
            weight = cur_creature.adult_weight / 100;

            age = Age(c.tier);
        }
        
        void advance_round() {
            if (!is_alive) return;
            age.advance_age();

            // Food and hunger loss
            hunger -= cur_creature.mtbl_rate;
            thirst -= cur_creature.mtbl_rate*1.5;
            if(hunger <= 0 || thirst <= 0) {
                health -= 1;
            }

            if (health <= 0) {
                is_alive = false;   
                cout << "dead\n";
            }
        }

        void display_stats() {
            cout << "SURVIVAL STATS\n";
            cout << "\tStatus: " << display_bool(is_alive) << "\n";
            cout << "\tHealth: " << health << "\n";
            cout << "\tHunger: " << hunger << "\n";
            cout <<"\tThirst: " << thirst << "\n";
            
            cout << "CREATURE STATS\n";
            cout << "\tAge: " << age.get_age() << "\n";
            cout << "\tCurrent Weight: " << weight << "\n";
            cur_creature.display_stats();
        }

        void set_creature(Creature c) {
            cur_creature = c;
            cur_creature = c;
            health = cur_creature.base_health;
            weight = cur_creature.adult_weight / 100;

            age = Age(c.tier);
        }
};

class Loop {
    protected:
        bool running {};
    public:
        Loop() = default;
        Loop(bool r) {
            running = r;
        }
        virtual void display_options() = 0;
        virtual void process_options(int choice) = 0;
        void start() {
            int choice;
            if (!running) running = true;
            while(running) {
                display_options();
                cout << ">> ";
                cin >> choice;
                process_options(choice);
            }
        }
};
    
class Game : public Loop {
    public:
        Game() = default;
        Game(bool r) : Loop(r) {}
        void display_options() override {
            
        }
        void process_options(int choice) override {
            /*
            switch (choice) {
                case 1:
            }
            */
        }
};

class Menu : public Loop {
    private:
        vector<string> options;
    public:
        Menu(bool r, vector<string> opt) : Loop(r) {
            options = opt;
        }
        void display_options() override {
            for (size_t i { 0 }; i < options.size(); ++i) {
                cout << "(" << i+1 << ") " << options[i] << "\n";
            }
        }
};

// For each creature:
// Size based on, Appearance based on
vector<Creature> avail_creatures = {
    Creature("Draconos", Diet::CARNIVORE, 4305, 395, 2500, 3), // Megalonyx, Glyptodon body + Entelodont facial features
    Creature("Harlikir", Diet::HERBIVORE, 3800, 200, 1500, 2), // Gaur, Barrel body + Bison-like Face
    Creature("Grandis", Diet::CARNIVORE, 2435, 230, 1100, 4), // Lythronax, Lythronax body plan + Garzapelta osteoderms
    Creature("Carnagal", Diet::CARNIVORE, 4420, 500, 3000, 4), // Simbakubwa, more furry Simbakubwa
    Creature("Flikch", Diet::HERBIVORE, 2310, 175, 700, 2), // Large Pig, Binturong + lemur tail.
};

class CreatureSelection : public Menu {
    private:
        vector<string> get_names(const vector<Creature>& c) {
            vector<string> a;
            for(Creature creature : c) {
                a.push_back(creature.name);
            }
            return a;
        }

        vector<Creature> creatures;
        Player* playerPtr {};
    public:
        CreatureSelection(bool r, vector<Creature> c, Player* plr) : Menu(r, get_names(avail_creatures)) {
            creatures = c;
            playerPtr = plr;
        }

        void process_options(int choice) override {
            if(choice > creatures.size() || choice <= 0) {
                cout << "Invalid option.\n";
                return;
            }
            playerPtr->set_creature(creatures[choice-1]);
            running = false;
        }
};

/*
class SaveSelection : public Menu {
};
*/

/*
class MainMenu : public Menu {
};
*/

int main() {    
    Player player {};
    CreatureSelection c(true, avail_creatures, &player);
    c.start();
    player.display_stats();
    return 0;
}