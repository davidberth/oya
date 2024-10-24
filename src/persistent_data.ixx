module;

#include <fstream>
#include <iostream>
#include <string>

export module persistent_data;

export class PersistentData {
public:
    int window_size;
    bool gui_on;

    PersistentData() : window_size(2), gui_on(true) {
        load();
    }

    ~PersistentData() {
        save();
    }

    void load() {
        std::ifstream infile("preferences.txt");
        if (infile.is_open()) {
            infile >> window_size >> gui_on;
            infile.close();
        }
        else {
            std::cerr << "Could not open preferences file for reading.\n";
        }
    }

    void save() const {
        std::ofstream outfile("preferences.txt");
        if (outfile.is_open()) {
            outfile << window_size << " " << gui_on;
            outfile.close();
        }
        else {
            std::cerr << "Could not open preferences file for writing.\n";
        }
    }
};

export PersistentData persistent_data;