import itertools
import math
import time
import csv

start_time=time.time()

class Person:
    def __init__(self, name, skill_level, modifiers):
        self.name = name
        self.skill_level = skill_level
        self.modifiers = modifiers
        self.position = None
        self.team = None

class Team:
    def __init__(self, name, TPR=0, ISL=0):
        self.name = name
        self.TPR = TPR
        self.ISL = ISL
        self.positions = {
            1: None,
            2: None,
            3: None,
            4: None,
            5: None,
        }

combination_info = []       # Create a list to store information about each combination

def calculate_tpr(team):
    tpr = 0
    for position, player in team.positions.items():
        if player is not None:
            tpr += player.modifiers[position - 1]
    return tpr

def store_combination_info(mmr_value, combination):
    combination_data = {
        "Combination Number": len(combination_info) + 1,
        "Team A Players": [player.name for player in combination[:5]],
        "Team B Players": [player.name for player in combination[5:]],
        "Team A Positions": [player.position for player in combination[:5]],
        "Team B Positions": [player.position for player in combination[5:]],
        "Team A ISL": calculate_tpr(team_a),
        "Team B ISL": calculate_tpr(team_b),
        "Team A TPR": sum(player.skill_level for player in combination[:5]),
        "Team B TPR": sum(player.skill_level for player in combination[5:]),
        "Total TPR": calculate_tpr(team_a) + calculate_tpr(team_b),
        "ISLV": calculate_tpr(team_a) - calculate_tpr(team_b),
        "MMR": mmr_value,
    }
    combination_info.append(combination_data)

def SuperAlgo(team_a, team_b):
                                                                      # Create a list of all players
    all_players = [player for position, player in team_a.positions.items() if player is not None]
    all_players += [player for position, player in team_b.positions.items() if player is not None]
    
    player_permutations = list(itertools.permutations(all_players))   # Generate all permutations of the 10 players
    mmr_combinations = []                                             # Initialize a list to store MMR values and their corresponding combinations
    total_combinations = math.factorial(len(all_players))             # Total number of combinations
    last_printed_percentage = 0                                       # Initialize the last printed percentage

    # Iterate through all permutations and assign players to teams
    for index, permutation in enumerate(player_permutations, start=1):
        team_a_positions = permutation[:5]  # First 5 players to Team A
        team_b_positions = permutation[5:]  # Last 5 players to Team B

        # Assign players to teams
        for i, player in enumerate(team_a_positions):
            team_a.positions[i + 1] = player
            player.position = i + 1
            player.team = team_a

        for i, player in enumerate(team_b_positions):
            team_b.positions[i + 1] = player
            player.position = i + 1
            player.team = team_b


        isl_a = sum(player.skill_level for player in team_a_positions)        # Calculate the sum of skill levels for each team in this combination
        isl_b = sum(player.skill_level for player in team_b_positions)
        tpr_a = calculate_tpr(team_a)                                         # Calculate the TPR for each team in this combination
        tpr_b = calculate_tpr(team_b)
        
        ISLV = isl_a - isl_b                                                  # Calculate the ISLV (Individual Skill Level Variation)
        if ISLV == 0:
            ISLV = 0.1                                            # Set ISLV to a small non-zero value        
        
        total_tpr = tpr_a + tpr_b                                             # Calculate the Total TPR (Team A TPR + Team B TPR)
        mmr = total_tpr / ISLV                                                # Calculate the MMR (Matchmaking Rating) as TTPR/ISLV
        mmr = abs(mmr)                                                        # Ensure MMR is positive by taking the absolute value
        mmr_combinations.append((mmr, permutation))                           # Add the MMR value and combination to the list
        store_combination_info(mmr, permutation)                              # Call the function to store combination information

        current_percentage = (index / total_combinations) * 100               # Calculate the current percentage completion
        if current_percentage - last_printed_percentage >= 5:                 # Check if there's a 5% difference in progress since the last print
            print(f"Progress: {current_percentage:.2f}% completed")
            last_printed_percentage = current_percentage
            
    mmr_combinations.sort(key=lambda x: x[0], reverse=True)                   # Sort the MMR combinations in descending order of MMR values
   

team_a = Team("Team A")
team_b = Team("Team B")

# Define the data for people as tuples and create Person objects
people_data = [
    ("Brunatto", 3, [5, 4, 2, 1, 1]),
    ("Fonti di Oro", 5, [1, 2, 3, 4, 5]),
    ("Colo", 4, [1, 2, 3, 5, 4]),
    ("Mati", 5, [1, 1, 3, 4, 5]),
    ("Remi", 2, [1, 2, 5, 4, 3]),
    ("Kags", 1, [5, 4, 2, 1, 0.1]),
    ("Laucha", 3, [4, 5, 3, 2, 1]),
    ("Juan", 4, [3, 5, 4, 2, 1]),
    ("Stefa", 4, [4, 5, 3, 2, 1]),
    ("Cucho", 4, [1, 2, 4, 5, 3]),
    #("Giaco", 3, [5, 4, 3, 2, 1]),
]

# Create Person objects from the data and assign them to teams
for index, data in enumerate(people_data, start=1):
    name, skill_level, modifiers = data
    person = Person(name, skill_level, modifiers)
    if index % 2 == 1:
        team_a.positions[index // 2 + 1] = person
        person.position = index // 2 + 1
        person.team = team_a
    else:
        team_b.positions[index // 2] = person
        person.position = index // 2
        person.team = team_b


SuperAlgo(team_a, team_b)                       # Call SuperAlgo to perform calculations and print the top three combinations with the biggest MMR values


csv_file = "combination_info.csv"               # Save information to a CSV file

with open(csv_file, mode="w", newline="") as file:
    fieldnames = ["Combination Number", "Team A Players", "Team B Players", "Team A Positions", "Team B Positions", 
                  "Team A ISL", "Team B ISL", "Team A TPR", "Team B TPR", "Total TPR", "ISLV", "MMR"]
    writer = csv.DictWriter(file, fieldnames=fieldnames)
    
    writer.writeheader()
    writer.writerows(combination_info)













print("CSV file saved:", csv_file)

# Sort combination_info by MMR in descending order
combination_info.sort(key=lambda x: x["MMR"], reverse=True)

# Print the top three combinations with the highest MMR
print("\nTop 3 Combinations by MMR:")
for i in range(min(3, len(combination_info))):
    combination = combination_info[i]
    print(f"Combination {i + 1}:")
    print("Team A Players:", combination["Team A Players"])
    print("Team B Players:", combination["Team B Players"])
    print("Team A Positions:", combination["Team A Positions"])
    print("Team B Positions:", combination["Team B Positions"])
    print("Team A ISL:", combination["Team A ISL"])
    print("Team B ISL:", combination["Team B ISL"])
    print("Team A TPR:", combination["Team A TPR"])
    print("Team B TPR:", combination["Team B TPR"])
    print("Total TPR:", combination["Total TPR"])
    print("ISLV:", combination["ISLV"])
    print("MMR:", combination["MMR"])
    print()

print("--- %s seconds ---" % (time.time() - start_time))