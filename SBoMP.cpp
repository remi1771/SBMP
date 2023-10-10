#include <iostream>
#include <string>
#include <array>
#include <vector> // Include the vector header
#include <algorithm>
#include <numeric>
#include <list>
#include <conio.h>
#include <chrono>
#include <thread>


struct Player {
    std::string key;             // 2-digit alphanumeric key
    std::string name;            // Name (string)
    float skillLevel;            // Skill level (float)
    std::array<float, 5> modifiers;  // Modifiers (Array of 5 floats)
    bool isSelectable;              // IsOnTeam (bool)
    int number;  // New variable to store a number (0 to 10)

    // Declara la clase struct de lo que implica ser un jugador
    Player(std::string k, std::string n, float skill, std::array<float, 5> mods, bool teamStatus, int num)
        : key(k), name(n), skillLevel(skill), modifiers(mods), isSelectable(teamStatus), number(num) {} // Constructor to initialize the struct members

};
struct Combination {
    int combinationNumber;
    std::list<Player> teamAPlayers;
    std::list<Player> teamBPlayers;
    float LevelTeamA;
    float LevelTeamB;
    float TruePositionRatingA;
    float TruePositionRatingB;
    float skillLevelDisparity;
    float truePositionRating;
    float mmRating;
    int combinationRanking;
};
std::list<Combination> AllCombinations;

void calculateMetrics(Combination& combination) {
    // Calculate LevelTeamA
    float levelTeamA = 0.0f;
    for (const Player& player : combination.teamAPlayers) {
        levelTeamA += player.skillLevel;
    }
    combination.LevelTeamA = levelTeamA;

    // Calculate LevelTeamB
    float levelTeamB = 0.0f;
    for (const Player& player : combination.teamBPlayers) {
        levelTeamB += player.skillLevel;
    }
    combination.LevelTeamB = levelTeamB;

    float truePositionRatingA = 0.0f;
    int positionA = 0;  // Start with position 1 for the first player in Team A

    for (const Player& player : combination.teamAPlayers) {
        truePositionRatingA += player.modifiers[positionA];
        positionA++;  // Increment the position for the next player
    }
    float truePositionRatingB = 0.0f;
    int positionB = 0;  // Start with position 1 for the first player in Team A

    for (const Player& player : combination.teamBPlayers) {
        truePositionRatingB += player.modifiers[positionB];
        positionB++;  // Increment the position for the next player
    }
    combination.skillLevelDisparity = std::abs(combination.LevelTeamA - combination.LevelTeamB);
    combination.truePositionRating = combination.TruePositionRatingA + combination.TruePositionRatingB;

    // Calculate mmRating
    if (combination.skillLevelDisparity == 0.0f) {
        combination.skillLevelDisparity += 0.05f;  // Add 0.05 to skillLevelDisparity when it's zero
    }
    combination.mmRating = combination.truePositionRating / combination.skillLevelDisparity;
}

std::vector<Player> playerPool = {
        {"FB", "Brunatto", 3.75f, {5.0f, 4.0f, 3.0f, 2.0f, 1.0f}, true, 0},
        {"MT", "Mati", 4.75f, {1.0f, 2.0f, 3.0f, 5.0f, 5.0f}, false, 0},
        {"CT", "Colo", 4.25f, {1.0f, 2.0f, 3.0f, 5.0f, 4.0f}, false, 0},
        {"MR", "Remi", 2.1875f, {1.0f, 2.0f, 4.0f, 5.0f, 3.0f}, true, 0},
        {"JP", "Juan", 3.14125f, {2.0f, 4.0f, 5.0f, 3.0f, 1.0f}, true, 0},
        {"SF", "Stefa", 4.625f, {4.0f, 5.0f, 5.0f, 2.0f, 1.0f}, true, 0},
        {"GF", "Giaco", 3.75f, {5.0f, 4.0f, 3.0f, 2.0f, 1.0f}, true, 0},
        {"FF", "Fontinovo", 4.5f, {1.0f, 2.0f, 3.0f, 4.0f, 5.0f}, true, 0},
        {"KX", "Kags", 1.875f, {5.0f, 4.0f, 2.0f, 1.0f, 1.0f}, true, 0},
        {"CX", "Cucho", 3.25f, {1.0f, 2.0f, 4.0f, 5.0f, 3.0f}, false, 0},  //hasta aca son 10
        {"TH", "Thomy", 3.125f, {2.0f, 3.0f, 4.0f, 5.0f, 2.0f}, true, 0 },
        {"PB", "Pablo", 2.75f, { 2.0f, 5.0f, 4.0f, 3.0f, 1.0f }, true, 0},
        {"NL", "Nico Ila", 3.0f, { 1.0f, 2.0f, 3.0f, 5.0f, 4.0f }, true, 0},
        {"Z1", "Empty", 0.5f, {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, false, 0},
        {"Z2", "Empy", 0.5f, {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, false, 0},
        {"Z3", "Emty", 0.5f, {1.0f, 1.0f, 1.0f, 1.0f, 1.0f}, false, 0},
}; // Inicializa lista de jugadores (aca van los players)
std::vector<Player> selectedPlayers;            // Create a variable for selectable players

bool Is_Odd_Ok = false;                      // Initialize as false by default

bool ErrorInPlayerPool(int totalSelectablePlayers, std::vector<Player>& selectedPlayers) {
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
            return true;
        }
    }

    return false;  // Added return statement
}


std::vector<Player> selectPlayers(const std::vector<Player>& playerPool) {
    std::vector<Player> Selectables;
    for (const Player& player : playerPool) {
        if (player.isSelectable) {
            Selectables.push_back(player);
        }
    }
    return Selectables;
}
// Funcion para que separe los players de los players elegibles

void assignPlayerNumbers(std::vector<Player>& selectedPlayers) {
    int numberCounter = 0;
    for (Player& player : selectedPlayers) {
        player.number = numberCounter++;
        if (numberCounter > 9) {
            numberCounter = 0; // Reset to 0 when it reaches 9
        }
    }
}

bool customComparison(const Player& player1, const Player& player2) {
    return player1.number < player2.number;
}

void generatePlayerPermutations(std::vector<Player>& players) {
    int combinationCount = 0;
    int totalCombinations = 3628800; // Total number of combinations
    int progressInterval = totalCombinations / 20; // 5% intervals



    do {
        std::list<Player> teamA(players.begin(), players.begin() + 5);
        std::list<Player> teamB(players.begin() + 5, players.end());

        Combination combination; //lol
        combination.combinationNumber = combinationCount++;
        combination.teamAPlayers = teamA;
        combination.teamBPlayers = teamB;

        // Calculate metrics
        calculateMetrics(combination);
        float rating = combination.mmRating;

        // Print progress every 5%
        if (combinationCount % progressInterval == 0) {
            int progress = (combinationCount * 100) / totalCombinations;
            std::cout << "Progress: " << progress << "%" << std::endl;
        }

        // Clear the memory associated with teamA and teamB
        teamA.clear();
        teamB.clear();
    } while (std::next_permutation(players.begin(), players.end(), customComparison));

}

void printCombination(const Combination& combination) {
    std::cout << "Combination Number: " << combination.combinationNumber << "\n";
    std::cout << "Team A Players:\n";
    for (const auto& player : combination.teamAPlayers) {
        std::cout << player.name << " ";
    }
    std::cout << "\nTeam B Players:\n";
    for (const auto& player : combination.teamBPlayers) {
        std::cout << player.name << " ";
    }
    std::cout << "\nLevel Team A: " << combination.LevelTeamA << "\n";
    std::cout << "Level Team B: " << combination.LevelTeamB << "\n";
    std::cout << "True Position Rating Team A: " << combination.TruePositionRatingA << "\n";
    std::cout << "True Position Rating Team B: " << combination.TruePositionRatingB << "\n";
    std::cout << "Skill Level Disparity: " << combination.skillLevelDisparity << "\n";
    std::cout << "True Position Rating: " << combination.truePositionRating << "\n";
    std::cout << "MM Rating: " << combination.mmRating << "\n";
    std::cout << "Combination Ranking: " << combination.combinationRanking << "\n";
    std::cout << "\n\n";
}

void printAllCombinations(const std::list<Combination>& AllCombinations) {
    for (const auto& combination : AllCombinations) {
        printCombination(combination);
    }
}

void generatePlayerPermutationsParallel(std::vector<Player>& players, int start, int end) {
    for (int i = start; i < end; ++i) {
        std::list<Player> teamA(players.begin(), players.begin() + 5);
        std::list<Player> teamB(players.begin() + 5, players.end());

        Combination combination;
        combination.combinationNumber = i;
        combination.teamAPlayers = teamA;
        combination.teamBPlayers = teamB;

        // Calculate metrics
        calculateMetrics(combination);
        float rating = combination.mmRating;

        // Process the combination (e.g., add it to a shared container)
        // ...

        teamA.clear();
        teamB.clear();
    }
}

int numberCounter = 0;

int main() {
    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    /* 
    // Number of threads you want to use
    int numThreads = std::thread::hardware_concurrency();

    // Split the work into equal parts for each thread
    int combinationsPerThread = 3628800 / numThreads;

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        int start = i * combinationsPerThread;
        int end = (i == numThreads - 1) ? 3628800 : (i + 1) * combinationsPerThread;
        threads.emplace_back(generatePlayerPermutationsParallel, std::ref(selectedPlayers), start, end);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    } */

    std::vector<Player> selectedPlayers = selectPlayers(playerPool); //Corre la funcion selectPlayers

    /*
    std::cout << "Corrimos la función selectPlayers" << std::endl;
    std::cout << "Selected Players:\n";
    for (const Player& player : selectedPlayers) {
        std::cout << "Name: " << player.name << ", Skill Level: " << player.skillLevel << "\n";
    }
    _getch(); */ 

    int totalSelectablePlayers = selectedPlayers.size();     // Calculate the number of selectable players
;
    for (Player& player : selectedPlayers) {
        player.number = numberCounter;
        numberCounter++;
        if (numberCounter > 10) {
            numberCounter = 0;  // Reset the number to 0 when it reaches 10
        }
    }


    bool Is_Odd_Ok = ErrorInPlayerPool(totalSelectablePlayers, playerPool); //corre la funcion ErrorInPlayerPool

    if (Is_Odd_Ok) {
        // Set IsSelectable to true for Z1, Z2, and Z3
        for (Player& player : playerPool) {
            if (player.key == "Z1" || player.key == "Z2" || player.key == "Z3") {
                player.isSelectable = true;
            }
        }
    }

    selectedPlayers = selectPlayers(playerPool);
    assignPlayerNumbers(selectedPlayers);

    std::cout << "Generating Permutations, please wait..."  << std::endl;

    generatePlayerPermutations(selectedPlayers);

   printAllCombinations(AllCombinations);

   // Stop measuring time
   auto stop = std::chrono::high_resolution_clock::now();

   // Calculate the duration in minutes and seconds
   auto duration = std::chrono::duration_cast<std::chrono::minutes>(stop - start);
   auto remainingSeconds = std::chrono::duration_cast<std::chrono::seconds>(stop - start) % std::chrono::minutes(1);

   // Print the execution time in minutes and seconds
   std::cout << "Execution Time: " << duration.count() << " minutes and " << remainingSeconds.count() << " seconds" << std::endl;

    return 0;
}
