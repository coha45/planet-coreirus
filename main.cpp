#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <unordered_map>
#include <windows.h>
using namespace std;

void set_cursor_pos(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

string display_bool(bool v) {
    if (v) return "True";
    else return "False";
}

struct Position {
    int x;
    int y;

    friend ostream& operator<<(ostream& os, Position pos) {
        os << "(" << pos.x << ", " << pos.y << ")";
        return os;
    }
};

enum class Diet : uint8_t {
    HERBIVORE,
    CARNIVORE,
    OMNIVORE
};

inline ostream& operator<<(ostream& os, Diet diet) {
    switch(diet) {
        case Diet::HERBIVORE: os << "Herbivore"; break; 
        case Diet::CARNIVORE: os << "Carnivore"; break; 
        case Diet::OMNIVORE: os << "Omnivore"; break;
        default: os << "This shouldn't happen.";
    }
    return os;
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

struct Edible {
    string name;
    int hunger_value;
    int thirst_value = 0;
};

// Note: Omnivores will share the same item pool as 
// carnivores and herbivores for each biome.
// ------------------------------------------------
//  Edible Items for Carnivores
Edible bony_fish { "Bony Fish", 3 };
Edible carcass { "Carcass", 2 };
Edible small_mammal { "Small Mammal", 5 }; // Placeholders for creatures that may fill similar niches.
Edible small_reptile { "Small Reptile", 4 };
Edible medium_reptile { "Medium Reptile", 8 };
Edible dragon_fly { "Dragon Fly", 1 };
Edible grasshopper { "Grasshopper", 1 };
Edible scorpion { "Scorpion", 1 };
Edible creature { "Creature", 0 }; // Special placeholder for when player encounters an NPC while hunting.

//  Edible Items for Herbivores
Edible cactus { "Cactus", 1, 2 };
Edible shrub { "Shrub", 2 };
Edible fern { "Fern", 2 };
Edible conifer { "Conifer", 2 };
Edible grass { "Grass", 1 };
Edible fruit { "Fruit", 4 };
Edible seed { "Seed", 3 };
Edible twig { "Twig", 1 };
Edible berry { "Berry", 3 };

enum class Biome : uint8_t {
    RAINFOREST,
    TUNDRA,
    DESERT,
    SWAMP,
    TEMPERATE_FOREST,
    PLAINS
};

inline ostream& operator<<(ostream& os, Biome biome) {
    switch(biome) {
        case Biome::RAINFOREST: os << "Rainforest"; break;
        case Biome::TUNDRA: os << "Tundra"; break;
        case Biome::DESERT: os << "Desert"; break;
        case Biome::SWAMP: os << "Swamp"; break;
        case Biome::TEMPERATE_FOREST: os << "Temperate Forest"; break;
        case Biome::PLAINS: os << "Plains"; break;
        default: os << "This shouldn't happen."; break;
    }
    return os;
}

unordered_map<Biome, vector<Edible>> biome_edibles = {
    { Biome::RAINFOREST, { small_reptile, small_mammal, scorpion, dragon_fly, conifer } },
    { Biome::TUNDRA, { twig, berry, grass, small_mammal, carcass } },
    { Biome::DESERT, { shrub, cactus, scorpion, grasshopper, medium_reptile, small_reptile, carcass} },
    { Biome::SWAMP, { dragon_fly, shrub, conifer, small_reptile, medium_reptile, small_mammal } },
    { Biome::TEMPERATE_FOREST, { conifer, twig, berry, small_reptile, small_mammal, carcass } },
    { Biome::PLAINS, { medium_reptile, small_mammal, carcass, dragon_fly, grasshopper, shrub, fern, grass, fruit, seed, twig, berry } },
};




// Temporary class to handle single-player biomes/location.
// Handles player position + biome specific attributes.
class BiomeHandler {
    private:
        Biome current_biome {};
        int seed {};
        Position player_loc {};

    public:
        BiomeHandler() = default;
        BiomeHandler(Biome initial_biome) {
            player_loc = { 0, 0 };
            current_biome = initial_biome;
        }

        Biome current() {
            return current_biome;
        }
        
        void display_biome_drops() {
            const vector<Edible>& edible_pool = biome_edibles.at(current_biome);
            for(const Edible& e : edible_pool) {
                cout << e.name << ": " << e.hunger_value << "\n";
            }
        }

        Position get_player_location() {
            return player_loc;
        }

        void move(char dir) {
            switch(dir) {
                case 'n': player_loc.y++; break;
                case 's': player_loc.y--; break;
                case 'e': player_loc.x++; break;
                case 'w': player_loc.x--; break;
                default: cout << "Invalid Option.\n"; break;
            }
            // TODO: Add logic to change biome based on position.
            // ...
        }

        void display_location_stats() {
            cout << "LOCATION DATA:\n";
            cout << "\tCurrent Position: " << player_loc << "\n";
            cout << "\tCurrent Biome: " << current_biome << "\n";
        }
};

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
        Tier tier {};
        int base_health {};
        int base_atk {};
        int adult_weight {};
        int round {};
        int mtbl_rate {};
        Biome native_biome {};

        Creature() = default;

        Creature(string n, Diet dt, int bh, int ba, int w, int mtbl_r, Biome nb) {
            name = n;
            diet_type = dt;
            base_health = bh;
            base_atk = ba;
            adult_weight = w;
            mtbl_rate = mtbl_r;
            tier = get_tier(adult_weight);
            native_biome = nb;

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
// Currently unused, as there is not really any need to encapsulate these.
class Age {
    private: 
        int age_freq {};
        int age {};
        int age_ctr {};
        int weight {};
        int weight_gain_rate {};

    public:
        Age() = default;
        Age(Tier tier, int w, int wgr) {
            weight = w;
            weight_gain_rate = wgr;
            switch (tier) {
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
                weight += weight_gain_rate;
                age_ctr = 0;
            }
        }

        int get_age() {
            return age;
        }
        int get_weight() {
            return weight;
        }
};

int rate(Tier tier) {
    switch (tier) {
                case Tier::ENORMOUS:
                    return 5;
                case Tier::LARGE:
                    return 4;
                case Tier::MEDIUM:
                    return 3;
                case Tier::SMALL:
                    return 2;
                case Tier::TINY:
                    return 1;
                default:
                    throw runtime_error("Invalid age tier.");
    }
}

class Player {
    private:
        static inline int base_hunger = 100, base_thirst = 100, base_stamina = 100;
        Creature cur_creature {};
        int age {};
        int age_rate {};
        int weight {};
        int weight_gain_rate {};
        int health {};
        int health_gain_rate {};
        int hunger {};
        int thirst {};
        int stamina {};
        bool is_alive {};
        int round {};

    public: 
        Player() = default;

        Player(Creature c) {
            cur_creature = c;
            hunger = base_hunger;
            thirst = base_thirst;
            stamina = 100;
            is_alive = true;
            health = cur_creature.base_health / 100;
            weight = cur_creature.adult_weight / 100;

            // Rates
            age_rate = rate(cur_creature.tier);
            weight_gain_rate = cur_creature.adult_weight / 100;
            health_gain_rate = cur_creature.base_health / 100;
        }

        void hunt() {
            stamina -= 2;
        }

        Biome get_native_biome() {
            return cur_creature.native_biome;
        }
        
        void advance_round() {
            if (!is_alive) return;
            round++;

            if(round % age_rate == 0) {
                age++;
                weight += weight_gain_rate;
                health += health_gain_rate;
            }

            // Food and hunger loss
            hunger -= cur_creature.mtbl_rate;
            thirst -= cur_creature.mtbl_rate*1.5;
            if (hunger < 0) hunger = 0;
            if (thirst < 0) thirst = 0;

            if(hunger == 0 || thirst == 0) {
                health -= cur_creature.base_health*0.05;
            }

            if (health <= 0) {
                health = 0;  
                is_alive = false; 
                cout << "dead\n";
            }
        }

        void display_stats() {
            cout << "SESSION STATS\n";
            cout << "\tRound: " << round << "\n";

            cout << "SURVIVAL STATS\n";
            cout << "\tAlive: " << display_bool(is_alive) << "\n";
            cout << "\tHealth: " << health << "\n";
            cout << "\tHunger: " << hunger << "\n";
            cout <<"\tThirst: " << thirst << "\n";
            cout << "\tStamina: "  << stamina << "\n";
            
            
            cout << "CREATURE STATS\n";
            cout << "\tAge: " << age << "\n";
            cout << "\tCurrent Weight: " << weight << "\n";
            // cur_creature.display_stats();
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
    private:
        Player* player {};
        BiomeHandler biome_h {};
    public:
        Game() = default;
        Game(bool r, Player* plr) : Loop(r) {
            player = plr;
            biome_h = BiomeHandler(player->get_native_biome());
        }
        void display_options() override {
            biome_h.display_location_stats();
            player->display_stats();
            cout << "-----------------------------\n";
            cout << "1) Roam (Default)\n"; 
            cout << "2) Hunt/Forage\n"; 
            cout << "3) Rest\n"; 
            cout << "4) Sleep\n"; 
            cout << "5) Quit\n"; 
        }

        void hunt() {
            biome_h.display_biome_drops();
            player->hunt();
        }

        void process_options(int choice) override {
            switch(choice) {
                case 1:
                    char dir;
                    cout << "Move (N, S, E, W): ";
                    cin >> dir;
                    biome_h.move(dir);
                    break;
                case 2:
                    hunt();
                    break;
                case 3:
                    cout << "Unimplemented\n";
                    break;
                case 4:
                    cout << "Unimplemented\n";
                    break;
                case 5:
                    cout << "Unimplemented\n";
                    break;
                default: cout << "Invalid Option.\n";

                player->advance_round();
            }
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
    Creature("Draconos", Diet::CARNIVORE, 4305, 395, 2500, 3, Biome::PLAINS), // Megalonyx, Glyptodon body + Entelodont facial features
    Creature("Harlikir", Diet::HERBIVORE, 3800, 200, 1500, 2, Biome::TUNDRA), // Gaur, Barrel body + Bison-like Face
    Creature("Grandis", Diet::CARNIVORE, 2435, 230, 1100, 4, Biome::DESERT), // Lythronax, Lythronax body plan + Garzapelta osteoderms
    Creature("Carnagal", Diet::CARNIVORE, 4420, 500, 3000, 4, Biome::RAINFOREST), // Simbakubwa, more furry Simbakubwa
    Creature("Flikch", Diet::HERBIVORE, 2310, 175, 700, 2, Biome::RAINFOREST), // Large Pig, Binturong + lemur tail.
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
            *playerPtr = Player(creatures[choice-1]);
            running = false;
        }
};

/*
class SaveSelection : public Menu {
};
*/

class MainMenu : public Menu {

};

int main() {    
    Player player {};
    CreatureSelection c(true, avail_creatures, &player);
    c.start();
    Game g(false, &player);
    g.start();
    return 0;
}