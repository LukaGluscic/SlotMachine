#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

struct GameData {
    int balance;
    int jackpot;
    vector<int> betHistory;
    vector<string> achievements;
    int highScore;
};

// Function declarations
void displayMenu();
void displayRules();
void playSlotMachine(GameData& gameData);
void addFunds(GameData& gameData);
void viewAchievements(const GameData& gameData);
void viewBettingHistory(const GameData& gameData);
void updateHighScoreAndQuit(GameData& gameData);
int getStartingBalance();
void displayIntro();
void displayDifficultyOptions();
int getDifficulty();
int getBetAmount(const GameData& gameData);
int getRandomNumber(int min, int max);
void displaySymbols(int delay);
bool playAgain();
bool isLuckySpin();
void displayJackpot(int jackpot);
void unlockAchievement(vector<string>& achievements, const string& achievement);
void saveGameData(const GameData& gameData);
GameData loadGameData();
void deleteGameDataFile();

int main() {
    srand(time(0)); // Initialize random seed

    displayIntro();

    char viewRulesChoice;
    cout << "Would you like to view the rules? (Y/N): ";
    cin >> viewRulesChoice;

    if (viewRulesChoice == 'Y' || viewRulesChoice == 'y') {
        displayRules();
    }

    GameData gameData;
    bool isNewGame = true;

    // Load game data if it exists
    if (ifstream("game_data.bin")) {
        gameData = loadGameData();
        isNewGame = false;
    }
    else {
        gameData.balance = getStartingBalance();
        gameData.jackpot = 100;
        gameData.highScore = 0;
    }

    bool continuePlaying = true;

    while (continuePlaying) {
        displayMenu();

        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            displayRules();
            break;
        case 2:
            playSlotMachine(gameData);
            break;
        case 3:
            addFunds(gameData);
            break;
        case 4:
            viewAchievements(gameData);
            break;
        case 5:
            viewBettingHistory(gameData);
            break;
        case 6:
            viewAchievements(gameData.achievements);
            break;
        case 7:
            updateHighScoreAndQuit(gameData);
            continuePlaying = false;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }

    cout << "Thank you for playing!" << endl;

    return 0;
}

void displayMenu() {
    cout << "===================" << endl;
    cout << "       MENU        " << endl;
    cout << "===================" << endl;
    cout << "1. View Rules" << endl;
    cout << "2. Play Slot Machine" << endl;
    cout << "3. Add Funds" << endl;
    cout << "4. View Achievements" << endl;
    cout << "5. View Betting History" << endl;
    cout << "6. View All Achievements" << endl;
    cout << "7. Update High Score and Quit" << endl;
    cout << "===================" << endl;
    cout << "Enter your choice (1-7): ";
}

void displayRules() {
    cout << "----- SLOT MACHINE RULES -----" << endl;
    cout << "Match three symbols to win." << endl;
    cout << "The jackpot starts at 100 and increases by 10 with each play." << endl;
    cout << "If you match three dollar signs ($$$), you win the jackpot." << endl;
    cout << "Good luck!" << endl;
}

void playSlotMachine(GameData& gameData) {
    cout << "----- SLOT MACHINE -----" << endl;

    int betAmount = getBetAmount(gameData);

    if (gameData.balance < betAmount) {
        cout << "Insufficient balance. Please add funds." << endl;
        return;
    }

    gameData.balance -= betAmount;

    displaySymbols(500);

    if (isLuckySpin()) {
        cout << "Congratulations! You won " << betAmount * 3 << "." << endl;
        gameData.balance += betAmount * 3;

        if (gameData.balance > gameData.highScore) {
            gameData.highScore = gameData.balance;
        }

        if (gameData.balance >= gameData.jackpot) {
            cout << "Congratulations! You won the jackpot of " << gameData.jackpot << "!" << endl;
            unlockAchievement(gameData.achievements, "Jackpot Winner");
            gameData.jackpot += 10;
        }
    }
    else {
        cout << "Better luck next time!" << endl;
    }

    gameData.betHistory.push_back(betAmount);

    saveGameData(gameData);
}

void addFunds(GameData& gameData) {
    int amount;
    cout << "Enter the amount to add: ";
    cin >> amount;

    gameData.balance += amount;

    cout << "Funds added successfully." << endl;

    saveGameData(gameData);
}

void viewAchievements(const GameData& gameData) {
    cout << "----- ACHIEVEMENTS -----" << endl;

    if (gameData.achievements.empty()) {
        cout << "No achievements unlocked." << endl;
    }
    else {
        for (const string& achievement : gameData.achievements) {
            cout << "- " << achievement << endl;
        }
    }

    cout << "------------------------" << endl;
}

void viewBettingHistory(const GameData& gameData) {
    cout << "----- BETTING HISTORY -----" << endl;

    if (gameData.betHistory.empty()) {
        cout << "No betting history." << endl;
    }
    else {
        for (int betAmount : gameData.betHistory) {
            cout << "- " << betAmount << endl;
        }
    }

    cout << "---------------------------" << endl;
}

void updateHighScoreAndQuit(GameData& gameData) {
    if (gameData.balance > gameData.highScore) {
        gameData.highScore = gameData.balance;
        cout << "New high score: " << gameData.highScore << endl;
    }

    saveGameData(gameData);
}

int getStartingBalance() {
    int balance;
    cout << "Enter your starting balance: ";
    cin >> balance;
    return balance;
}

void displayIntro() {
    cout << "Welcome to the Slot Machine Game!" << endl;
    cout << "------------------------------" << endl;
}

void displayDifficultyOptions() {
    cout << "Select a difficulty level:" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cout << "Enter your choice (1-3): ";
}

int getDifficulty() {
    int difficulty;
    displayDifficultyOptions();
    cin >> difficulty;

    while (difficulty < 1 || difficulty > 3) {
        cout << "Invalid choice. Please try again." << endl;
        displayDifficultyOptions();
        cin >> difficulty;
    }

    return difficulty;
}

int getBetAmount(const GameData& gameData) {
    int betAmount;

    cout << "Current balance: " << gameData.balance << endl;

    if (gameData.balance < gameData.jackpot) {
        cout << "Minimum bet amount is " << gameData.balance << "." << endl;
        cout << "Enter your bet amount: ";
        cin >> betAmount;

        while (betAmount < 1 || betAmount > gameData.balance) {
            cout << "Invalid bet amount. Please try again." << endl;
            cout << "Enter your bet amount: ";
            cin >> betAmount;
        }
    }
    else {
        cout << "Enter your bet amount (1-" << gameData.balance << "): ";
        cin >> betAmount;

        while (betAmount < 1 || betAmount > gameData.balance) {
            cout << "Invalid bet amount. Please try again." << endl;
            cout << "Enter your bet amount (1-" << gameData.balance << "): ";
            cin >> betAmount;
        }
    }

    return betAmount;
}

int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void displaySymbols(int delay) {
    vector<string> symbols = { "cherry", "lemon", "orange", "plum", "bell", "bar", "seven", "$$$" };

    cout << "Spinning..." << endl;
    this_thread::sleep_for(chrono::milliseconds(delay));

    vector<int> selectedSymbols;

    for (int i = 0; i < 3; i++) {
        int symbolIndex = getRandomNumber(0, symbols.size() - 1);
        selectedSymbols.push_back(symbolIndex);
        cout << symbols[symbolIndex] << " ";
        this_thread::sleep_for(chrono::milliseconds(delay));
    }

    cout << endl;
}

bool playAgain() {
    char choice;
    cout << "Would you like to play again? (Y/N): ";
    cin >> choice;
    return (choice == 'Y' || choice == 'y');
}

bool isLuckySpin() {
    int difficulty = getDifficulty();

    switch (difficulty) {
    case 1:
        return getRandomNumber(1, 100) <= 50;
    case 2:
        return getRandomNumber(1, 100) <= 30;
    case 3:
        return getRandomNumber(1, 100) <= 10;
    default:
        return false;
    }
}

void displayJackpot(int jackpot) {
    cout << "Current Jackpot: " << jackpot << endl;
}

void unlockAchievement(vector<string>& achievements, const string& achievement) {
    if (find(achievements.begin(), achievements.end(), achievement) == achievements.end()) {
        achievements.push_back(achievement);
        cout << "Achievement unlocked: " << achievement << endl;
    }
}

void saveGameData(const GameData& gameData) {
    ofstream file("game_data.bin", ios::binary);

    if (file) {
        file.write(reinterpret_cast<const char*>(&gameData), sizeof(gameData));
        file.close();
    }
    else {
        cout << "Error: Failed to save game data." << endl;
    }
}

GameData loadGameData() {
    ifstream file("game_data.bin", ios::binary);

    GameData gameData;

    if (file) {
        file.read(reinterpret_cast<char*>(&gameData), sizeof(gameData));
        file.close();
    }
    else {
        cout << "Error: Failed to load game data. Starting a new game." << endl;
        gameData.balance = getStartingBalance();
        gameData.jackpot = 100;
        gameData.highScore = 0;
    }

    return gameData;
}

void deleteGameDataFile() {
    remove("game_data.bin");
}

