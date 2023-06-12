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
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed

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
            viewAchievements(gameData); // Changed to correct function name
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
    cout << "1. The objective of the game is to match 3 symbols in a row." << endl;
    cout << "2. Each symbol has a specific payout associated with it." << endl;
    cout << "3. You can place bets using your available balance." << endl;
    cout << "4. If you run out of funds, you can add more." << endl;
    cout << "5. The jackpot increases with each spin and can be won randomly." << endl;
    cout << "6. Good luck and have fun!" << endl;
    cout << "------------------------------" << endl;
}

void playSlotMachine(GameData& gameData) {
    int betAmount = getBetAmount(gameData);

    if (betAmount <= gameData.balance) {
        gameData.balance -= betAmount;
        gameData.betHistory.push_back(betAmount);

        int delay = getDifficulty();

        displaySymbols(delay);

        int random1 = getRandomNumber(1, 9);
        int random2 = getRandomNumber(1, 9);
        int random3 = getRandomNumber(1, 9);

        cout << random1 << " - " << random2 << " - " << random3 << endl;

        if (random1 == random2 && random2 == random3) {
            cout << "Congratulations! You won!" << endl;
            int payout = betAmount * random1;
            gameData.balance += payout;
            cout << "Payout: " << payout << endl;

            if (isLuckySpin()) {
                displayJackpot(gameData.jackpot);
                gameData.balance += gameData.jackpot;
                gameData.jackpot = 100;
            }
        }
        else {
            cout << "Better luck next time!" << endl;
        }

        if (gameData.balance > gameData.highScore) {
            gameData.highScore = gameData.balance;
        }

        saveGameData(gameData);
    }
    else {
        cout << "Insufficient funds. Please add more funds." << endl;
    }
}

void addFunds(GameData& gameData) {
    int amount;
    cout << "Enter the amount to add: ";
    cin >> amount;

    if (amount > 0) {
        gameData.balance += amount;
        cout << "Funds added successfully!" << endl;
        cout << "New Balance: " << gameData.balance << endl;
        saveGameData(gameData);
    }
    else {
        cout << "Invalid amount. Please try again." << endl;
    }
}

void viewAchievements(const GameData& gameData) {
    cout << "----- ACHIEVEMENTS -----" << endl;

    if (gameData.achievements.empty()) {
        cout << "No achievements unlocked yet." << endl;
    }
    else {
        for (const string& achievement : gameData.achievements) {
            cout << achievement << endl;
        }
    }

    cout << "------------------------" << endl;
}

void viewBettingHistory(const GameData& gameData) {
    cout << "----- BETTING HISTORY -----" << endl;

    if (gameData.betHistory.empty()) {
        cout << "No betting history available." << endl;
    }
    else {
        for (int bet : gameData.betHistory) {
            cout << "Bet Amount: " << bet << endl;
        }
    }

    cout << "----------------------------" << endl;
}

void updateHighScoreAndQuit(GameData& gameData) {
    if (gameData.balance > gameData.highScore) {
        gameData.highScore = gameData.balance;
        saveGameData(gameData);
        cout << "New high score recorded!" << endl;
    }
    else {
        cout << "High score not beaten. Goodbye!" << endl;
    }
}

int getStartingBalance() {
    int balance;
    cout << "Enter your starting balance: ";
    cin >> balance;
    return balance;
}

void displayIntro() {
    cout << "Welcome to the Slot Machine Game!" << endl;
}

void displayDifficultyOptions() {
    cout << "Select the difficulty level:" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cout << "Enter your choice (1-3): ";
}

int getDifficulty() {
    int choice;
    displayDifficultyOptions();
    cin >> choice;

    while (choice < 1 || choice > 3) {
        cout << "Invalid choice. Please try again." << endl;
        displayDifficultyOptions();
        cin >> choice;
    }

    int delay;
    switch (choice) {
    case 1:
        delay = 3000;
        break;
    case 2:
        delay = 2000;
        break;
    case 3:
        delay = 1000;
        break;
    default:
        delay = 3000;
        break;
    }

    return delay;
}

int getBetAmount(const GameData& gameData) {
    int amount;
    cout << "Current Balance: " << gameData.balance << endl;
    cout << "Enter the bet amount: ";
    cin >> amount;
    return amount;
}

int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

void displaySymbols(int delay) {
    cout << "Spinning..." << endl;
    this_thread::sleep_for(chrono::milliseconds(delay));

    cout << "*-*-*" << endl;
    this_thread::sleep_for(chrono::milliseconds(delay));

    cout << "-*-*-" << endl;
    this_thread::sleep_for(chrono::milliseconds(delay));

    cout << "*-*-*" << endl;
    this_thread::sleep_for(chrono::milliseconds(delay));
}

bool playAgain() {
    char choice;
    cout << "Play again? (Y/N): ";
    cin >> choice;
    return (choice == 'Y' || choice == 'y');
}

bool isLuckySpin() {
    int random = getRandomNumber(1, 10);
    return (random == 1);
}

void displayJackpot(int jackpot) {
    cout << "Congratulations! You hit the jackpot!" << endl;
    cout << "Jackpot Amount: " << jackpot << endl;
}

void unlockAchievement(vector<string>& achievements, const string& achievement) {
    if (find(achievements.begin(), achievements.end(), achievement) == achievements.end()) {
        achievements.push_back(achievement);
        cout << "Achievement Unlocked: " << achievement << endl;
    }
}

void saveGameData(const GameData& gameData) {
    ofstream file("game_data.bin", ios::binary);
    file.write(reinterpret_cast<const char*>(&gameData), sizeof(gameData));
    file.close();
}

GameData loadGameData() {
    GameData gameData;
    ifstream file("game_data.bin", ios::binary);
    file.read(reinterpret_cast<char*>(&gameData), sizeof(gameData));
    file.close();
    return gameData;
}

void deleteGameDataFile() {
    remove("game_data.bin");
}
