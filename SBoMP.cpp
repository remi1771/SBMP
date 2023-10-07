#include <iostream>
#include <string>
#include <array>
#include <vector> // Include the vector header

struct Player {
    // Struct definition for the Player
    std::string key;             // 2-digit alphanumeric key
    std::string name;            // Name (string)
    float skillLevel;            // Skill level (float)
    std::array<float, 5> modifiers;  // Modifiers (Array of 5 floats)
    bool isSelectable;              // IsOnTeam (bool)

    // Constructor to initialize the struct members
    Player(std::string k, std::string n, float skill, std::array<float, 5> mods, bool teamStatus)
        : key(k), name(n), skillLevel(skill), modifiers(mods), isSelectable(teamStatus) {}
};

int main() {
    // Create a pool of selectable players
    std::vector<Player> playerPool = {
        {"AB", "John Doe", 4.5f, {1.1f, 0.9f, 1.2f, 1.0f, 0.8f}, true},
        {"CD", "Jane Smith", 4.2f, {1.0f, 1.1f, 1.3f, 0.9f, 1.2f}, true},
        {"EF", "Jane Smitty", 4.2f, {1.0f, 1.1f, 1.3f, 0.9f, 1.2f}, true},
        {"Z1", "Empty", 0.5f, {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, false},
        {"Z2", "Empty", 0.5f, {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, false},
        {"Z3", "Empty", 0.5f, {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, false},
        // Add more players here
    };

    
    std::vector<Player> Selectables;            // Create a variable for selectable players
    bool Is_Odd_Ok = false;                      // Initialize as false by default

    // Loop through playerPool to select the players
    for (const Player& player : playerPool) {
        if (player.isSelectable) {
            Selectables.push_back(player);
        }
    }

   int totalSelectablePlayers = Selectables.size();     // Calculate the number of selectable players

    // Check and raise errors for special cases
    if (totalSelectablePlayers == 2) {
        std::cerr << "Error: Only one matchup possible!" << std::endl;
        // You can choose to exit the program or handle this case as needed.
    }
    else if (totalSelectablePlayers < 2) {
        std::cerr << "Error: No matchups possible!" << std::endl;
        // You can choose to exit the program or handle this case as needed.
    }
    else if (totalSelectablePlayers % 2 != 0) {
        std::cerr << "Warning: Odd number of players. Are you sure you want to proceed? (y/n): ";

        std::string userResponse;
        std::cin >> userResponse;

        if (userResponse == "yes" || userResponse == "Yes" || userResponse == "YES" || userResponse == "Y" || userResponse == "1" || userResponse == "y") {  //THIS NEEDS TO BE CHANGED SO IT CHECKS THE FIRST LETTER OF THE ANSWER AND IF IT IS Y(Yeses), S(Si), A(ccept), E(nable), OR 1 THEN ACCEPT
            Is_Odd_Ok = true;
        }
    }

    if (Is_Odd_Ok) {
        // Set IsSelectable to true for Z1, Z2, and Z3
        for (Player& player : playerPool) {
            if (player.key == "Z1" || player.key == "Z2" || player.key == "Z3") {
                player.isSelectable = true;
            }
        }
    }

    /*
    std::cout << "List of Players:" << std::endl;
    for (const Player& player : playerPool) {
        std::cout << "Key: " << player.key << ", Name: " << player.name
            << ", Skill Level: " << player.skillLevel << ", IsSelectable: " << (player.isSelectable ? "true" : "false")
            << ", Modifiers: [";
        for (float modifier : player.modifiers) {
            std::cout << modifier << " ";
        }
        std::cout << "]" << std::endl;
    }
    */                                                 // Print the list of players for debugging

    // Create all possible player combinations
    std::vector<std::vector<Player>> playerCombinations;

    // Loop through the selectable players to create combinations
    for (size_t i = 0; i < Selectables.size(); ++i) {
        for (size_t j = i + 1; j < Selectables.size(); ++j) {
            for (size_t k = j + 1; k < Selectables.size(); ++k) {
                for (size_t l = k + 1; l < Selectables.size(); ++l) {
                    for (size_t m = l + 1; m < Selectables.size(); ++m) {
                        // Create a combination with 5 players for Team A and 5 players for Team B
                        std::vector<Player> combinationA = { Selectables[i], Selectables[j], Selectables[k], Selectables[l], Selectables[m] };
                        std::vector<Player> combinationB;

                        for (size_t n = 0; n < Selectables.size(); ++n) {
                            if (n != i && n != j && n != k && n != l && n != m) {
                                combinationB.push_back(Selectables[n]);
                            }
                        }

                        playerCombinations.push_back(combinationA);
                        playerCombinations.push_back(combinationB);
                    }
                }
            }
        }
    }

    // Rest of your code for generating combinations and calculations goes here

    return 0;
}