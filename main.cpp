#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <windows.h>

using namespace std;

string map_name;
int map_width;
int map_height;
unique_ptr<vector<vector<int>>> map;

const int colors[] = {
    0, // Black
	FOREGROUND_BLUE, // Dark Blue
    FOREGROUND_RED, // Dark Red
    FOREGROUND_RED | FOREGROUND_GREEN, // Dark Yellow
    FOREGROUND_RED | FOREGROUND_BLUE, // Dark Magenta
	FOREGROUND_GREEN, // Dark Green
    FOREGROUND_GREEN | FOREGROUND_BLUE, // Dark Cyan
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // Gray
    FOREGROUND_RED | FOREGROUND_INTENSITY, // Light Red
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY // White
};

void setColor(int index) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (index < 0 || index >= sizeof(colors) / sizeof(colors[0])) {
        index = 0;
    }
    SetConsoleTextAttribute(hConsole, colors[index]);
}

void printMap() {
    for (const auto& row : *map) {
        for (int cell : row) {
            setColor(cell % (sizeof(colors) / sizeof(colors[0])));
            cout << cell << " ";
        }
        cout << endl;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void loadConfig(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        throw runtime_error("Failed to open config file.");
    }

    string line;
    while (getline(infile, line)) {
        size_t pos = line.find('=');
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            if (key == "map_file") {
                map_name = value;
            } else if (key == "map_width") {
                map_width = stoi(value);
            } else if (key == "map_height") {
                map_height = stoi(value);
            }
        }
    }

    cout << "Loaded config:\n"
         << "map_file: " << map_name << "\n"
         << "map_width: " << map_width << "\n"
         << "map_height: " << map_height << endl;
}

void loadMap(const string& filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        throw runtime_error("Failed to open map file for reading.");
    }

    map = make_unique<vector<vector<int>>>(map_height, vector<int>(map_width, 0));
    string line;
    for (int y = 0; y < map_height && getline(infile, line); ++y) {
        transform(line.begin(), line.begin() + min(map_width, static_cast<int>(line.size())), (*map)[y].begin(), [](char c) {
            return isdigit(c) ? c - '0' : 0;
        });
    }

    cout << "Loaded map:\n";
    printMap();
}

void saveMap(const string& filename) {
    ofstream outfile(filename);
    if (!outfile.is_open()) {
        throw runtime_error("Failed to open map file for writing.");
    }

    for (const auto& row : *map) {
        for (int cell : row) {
            outfile << cell;
        }
        outfile << endl;
    }
}

int main() {
    try {
        loadConfig("config.txt");
        loadMap(map_name);
		
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
