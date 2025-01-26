module;

#include <fstream>
#include <iostream>
#include <string>

export module persistent_data;

export class PersistentData {
public:
	int window_size;
	bool gui_on;
	int font_size;

	PersistentData() : window_size(3), gui_on(true), font_size(0) {
		load();
	}

	~PersistentData() {
		save();
	}

	void load() {
		std::ifstream infile("preferences.txt");
		if (infile.is_open()) {
			infile >> window_size >> gui_on >> font_size;
			infile.close();
		}
		else {
			std::cerr << "Could not open preferences file for reading.\n";
		}
	}

	void save() const {
		std::ofstream outfile("preferences.txt");
		if (outfile.is_open()) {
			outfile << window_size << " " << gui_on << " " << font_size;
			outfile.close();
		}
		else {
			std::cerr << "Could not open preferences file for writing.\n";
		}
	}
};

PersistentData* persistent_data;

export inline PersistentData* get_persistent_data() {
	return persistent_data;
}

export inline void init_persistent_data() {
	persistent_data = new PersistentData();
}

export inline void delete_persistent_data() {
	delete persistent_data;
}