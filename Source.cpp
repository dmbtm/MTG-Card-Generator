#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_set>
#include <random>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

//FILE PATHS HERE
//The program may give errors when trying to open these files, you may
//need to copy/past the full path to these files
string path1 = "default-cards.json";
string path2 = "cardlist.txt";
string path3 = "randomcardlist.txt";

bool IsLand(string s) {
    auto foundLand = s.find("Land");
    if (foundLand == string::npos) {
        return false;
    }
    else {
        return true;
    }   
}

void MakeList() {
    
    ifstream inputFile(path1);
    ofstream outputFile(path2);

    if (!inputFile.is_open()) {
        cout << "Failed to open " << path1 << endl;
    }
    else {
        cout << "File opened successfully" << endl;
    }

    json card;
    string line;
    unordered_set<string> seen;

    int i = 0;
    while (getline(inputFile, line)) {
        i++;
        if (line == "[" or line == "]") {
            continue;
        }
        else {
            //delete comma at end of line
            line.erase(line.end());
            line.erase(line.end() - 1);

            try {
                card = json::parse(line);
            }
            catch (const json::parse_error& e) {
                cerr << e.what() << endl;
            }
            //cout << card.dump(4) << endl;

            cout << "Checking line " << i << "..." << endl;

            if (card.contains("name") and
                seen.find(card["name"]) == seen.end() and
                card.contains("type_line") and
                card.contains("legalities") and
                card["legalities"]["commander"] == "legal" and
                IsLand(card["type_line"]) == false) {

                seen.insert(card["name"]);
                string name = card["name"];
                name = name.substr(0, name.size());
                outputFile << name << endl;
                cout << "Writing " << name << " to file" << endl;
                
            }
            else {
                cout << "Skipping card..." << endl;
            }
        }

    }

    inputFile.close();
    outputFile.close();
}

void MakeRandomList(int num) {
    ifstream inputFile(path2);
    ofstream outputFile(path3);

    vector<string> cards;

    if (!inputFile.is_open()) {
        cout << "Failed to open " << path2 << endl;
    }
    else {
        cout << "File opened successfully" << endl;
    }

    string line;

    while (getline(inputFile, line)) {
        cards.push_back(line);
    }    

    for (int i = 0; i < num; i++) {
        if (cards.size() == 0) {
            cerr << "Error: invalid entries (did you generate cardlist.txt properly?)" << endl;
        }
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, cards.size() - 1);
        int index = distrib(gen);
        outputFile << cards[index] << endl;
        swap(cards[index], cards.back());
        cards.pop_back();
    }

    inputFile.close();
    outputFile.close();
}

int main() {
    cout << "Welcome to the MTG Card Generator!" << endl;
    int in;
    input:
    cout << "What would you like to do?" << endl;
    cout << "1. Generate programmed card list" << endl;
    cout << "2. Generate specified number of cards from card list" << endl;
    cin >> in;
    if (cin.fail()) {
        cout << "Invalid input" << endl;
        goto input;
    }
    else if(in == 1){
        MakeList();
    }
    else if (in == 2) {
        int num;
        inputQuantity:
        cout << "How many random cards would you like?" << endl;
        cin >> num;
        if (cin.fail() or num <= 0) {
            cout << "Invalid input" << endl;
            goto inputQuantity;
        }
        else {
            MakeRandomList(num);
        }

    }
    return 0;
}