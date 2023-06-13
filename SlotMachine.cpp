#include<iostream>
#include<string>
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
void saveGameDataBinary(const GameData& gameData);
void loadGameDataBinary(GameData& gameData);
void saveGameDataText(const GameData& gameData);
void loadGameDataText(GameData& gameData);
void deleteGameDataFiles();

int main() {
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed

    displayIntro();

    char viewRulesChoice;
    cout << "Da li zelite vidjeti pravila prije igranja: (D/N): ";
    cin >> viewRulesChoice;

    if (viewRulesChoice == 'D' || viewRulesChoice == 'N') {
        displayRules();
    }

    GameData gameData;
    bool isNewGame = true;

    // Load game data if it exists
    if (ifstream("game_data_binary.bin") && ifstream("game_data_text.txt")) {
        loadGameDataBinary(gameData);
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
            cout << "Pogresan unos. Pokusajte ponovo!" << endl;
            break;
        }
    }

    cout << "Hvala na igranju!" << endl;

    return 0;
}

void displayMenu() {
    cout << "===================" << endl;
    cout << "       MENI        " << endl;
    cout << "===================" << endl;
    cout << "1. Pravila" << endl;
    cout << "2. Igraj Slot Machine" << endl;
    cout << "3. Dodavanje novca na racun" << endl;
    cout << "4. Vasa postignuca" << endl;
    cout << "5. Povijest kladnja" << endl;
    cout << "6. Sva vasa postignuca" << endl;
    cout << "7. Azurirajte najbolji rezultat te izadite iz igre" << endl;
    cout << "===================" << endl;
    cout << "Vas odabir je (1-7): ";
}

void displayRules() {
    cout << "----- SLOT MACHINE PRAVILA -----" << endl;
    cout << "1. Cilj igre je spojiti 3 simbola u nizu." << endl;
    cout << "2. Svaki simbol ima odredenu isplatu povezanu s njim" << endl;
    cout << "3. Mozete se kladiti koristeci svoj raspoloživi saldo." << endl;
    cout << "4. Ako vam ponestane novaca, možete dodati jos." << endl;
    cout << "5. Jackpot se povecava sa svakim okretanjem i može se osvojiti nasumicno." << endl;
    cout << "6. Sretno i zabavite se!" << endl;
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
            cout << "CESTITAMO! Pobjedili ste!" << endl;
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
            cout << "Vise srece drugi put!" << endl;
        }

        if (gameData.balance > gameData.highScore) {
            gameData.highScore = gameData.balance;
        }

        saveGameDataBinary(gameData);
        saveGameDataText(gameData);
    }
    else {
        cout << "Nedovoljno novaca. Dodajte još novca." << endl;
    }
}

void addFunds(GameData& gameData) {
    int amount;
    cout << "Dodajte svotu novaca: ";
    cin >> amount;

    if (amount > 0) {
        gameData.balance += amount;
        cout << "Novac je uspjesno dodan na vas racun!" << endl;
        cout << "Stanje vaseg racuna: " << gameData.balance << endl;
        saveGameDataBinary(gameData);
        saveGameDataText(gameData);
    }
    else {
        cout << "Nevazeci iznos. Molimo pokusajte ponovno." << endl;
    }
}

void viewAchievements(const GameData& gameData) {
    cout << "----- ACHIEVEMENTS -----" << endl;

    if (gameData.achievements.empty()) {
        cout << "Jos nema otkljucanih postignuca." << endl;
    }
    else {
        for (const string& achievement : gameData.achievements) {
            cout << achievement << endl;
        }
    }

    cout << "------------------------" << endl;
}

void viewBettingHistory(const GameData& gameData) {
    cout << "----- POVIJET KLADENJA -----" << endl;

    if (gameData.betHistory.empty()) {
        cout << "Nema dostupne povijesti kladenja." << endl;
    }
    else {
        for (int bet : gameData.betHistory) {
            cout << bet << endl;
        }
    }

    cout << "---------------------------" << endl;
}

void updateHighScoreAndQuit(GameData& gameData) {
    cout << "----- NAJBOLJI REZULTAT -----" << endl;
    cout << "Vas trenutni najbolji rezultat: " << gameData.highScore << endl;

    int newHighScore;
    cout << "Upisite vas novi najbolji rezulat: ";
    cin >> newHighScore;

    if (newHighScore > gameData.highScore) {
        gameData.highScore = newHighScore;
        cout << "Najbolji rezultat je uspjesno azuriran!" << endl;
        saveGameDataBinary(gameData);
        saveGameDataText(gameData);
    }
    else {
        cout << "Novi najbolji rezultat nije veci od Vaseg proslog najboljeg rezultata." << endl;
    }

    cout << "----------------------" << endl;
}

int getStartingBalance() {
    int startingBalance;
    cout << "Unesite Vas pocetni saldo: ";
    cin >> startingBalance;
    return startingBalance;
}

void displayIntro() {
    cout << "===================" << endl;
    cout << "   SLOT MACHINE" << endl;
    cout << "===================" << endl;
}

void displayDifficultyOptions() {
    cout << "----- RAZINA(TEZINA) IGRE -----" << endl;
    cout << "1. Lako (kasnjenje od 1 sekunde)" << endl;
    cout << "2. Srednje (kasnjenje od 2 sekunde)" << endl;
    cout << "3. Tesko (kasnjenje od 3 sekunde)" << endl;
    cout << "-----------------------------" << endl;
}

int getDifficulty() {
    displayDifficultyOptions();

    int choice;
    cout << "Unesite zeljenu tezinu(razinu) (1-3): ";
    cin >> choice;

    switch (choice) {
    case 1:
        return 1000; // 1 second delay
    case 2:
        return 2000; // 2 second delay
    case 3:
        return 3000; // 3 second delay
    default:
        return 1000; // Default to easy
    }
}

int getBetAmount(const GameData& gameData) {
    int betAmount;
    cout << "Unesite ulog: ";
    cin >> betAmount;

    if (betAmount > 0 && betAmount <= gameData.balance) {
        return betAmount;
    }
    else {
        cout << "Krivi unos.Pokusajte ponovo." << endl;
        return getBetAmount(gameData);
    }
}

int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void displaySymbols(int delay) {
    cout << "Okretanje kotaca..." << endl;
    this_thread::sleep_for(chrono::milliseconds(delay));
}

bool playAgain() {
    char choice;
    cout << "Da li zelite igrati ponovo? (D/N): ";
    cin >> choice;
    return (choice == 'D' || choice == 'y');
}

bool isLuckySpin() {
    int probability = getRandomNumber(1, 100);
    return (probability <= 5); // 5% chance
}

void displayJackpot(int jackpot) {
    cout << "Cestitamo! Osvojili ste jackpot!" << endl;
    cout << "Jackpot: " << jackpot << endl;
}

void unlockAchievement(vector<string>& achievements, const string& achievement) {
    cout << "Postugnuce otkljucano: " << achievement << endl;
    achievements.push_back(achievement);
}

void saveGameDataBinary(const GameData& gameData) {
    ofstream file("game_data_binary.bin", ios::binary);
    file.write(reinterpret_cast<const char*>(&gameData), sizeof(gameData));
    file.close();
}

void loadGameDataBinary(GameData& gameData) {
    ifstream file("game_data_binary.bin", ios::binary);
    file.read(reinterpret_cast<char*>(&gameData), sizeof(gameData));
    file.close();
}

void saveGameDataText(const GameData& gameData) {
    ofstream file("game_data_text.txt");

    file << gameData.balance << endl;
    file << gameData.jackpot << endl;

    file << gameData.betHistory.size() << endl;
    for (int bet : gameData.betHistory) {
        file << bet << endl;
    }

    file << gameData.achievements.size() << endl;
    for (const string& achievement : gameData.achievements) {
        file << achievement << endl;
    }

    file << gameData.highScore << endl;

    file.close();
}

void loadGameDataText(GameData& gameData) {
    ifstream file("game_data_text.txt");

    file >> gameData.balance;
    file >> gameData.jackpot;

    int betHistorySize;
    file >> betHistorySize;
    gameData.betHistory.resize(betHistorySize);
    for (int i = 0; i < betHistorySize; ++i) {
        file >> gameData.betHistory[i];
    }

    int achievementsSize;
    file >> achievementsSize;
    gameData.achievements.resize(achievementsSize);
    for (int i = 0; i < achievementsSize; ++i) {
        file.ignore();
        getline(file, gameData.achievements[i]);
    }

    file >> gameData.highScore;

    file.close();
}

void deleteGameDataFiles() {
    remove("game_data_binary.bin");
    remove("game_data_text.txt");
}
