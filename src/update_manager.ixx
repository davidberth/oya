module;

#include <algorithm>
#include <vector>

export module updatable_manager;

import updatable;

export class UpdatableManager {
public:

	UpdatableManager()
	{
		printf("UpdatableManager created\n");
	}

	void add_updatable(Updatable* updatable) {
		updatables.push_back(updatable);
	}

	void remove_updatable(Updatable* updatable) {
		updatables.erase(std::remove(updatables.begin(), updatables.end(), updatable), updatables.end());
	}

	void update_all() {
		for (auto* updatable : updatables) {
			if (updatable->is_enabled()) {
				updatable->update();
			}
		}
	}

private:
	std::vector<Updatable*> updatables;
};

UpdatableManager* updatable_manager;

export inline UpdatableManager* get_updatable_manager() {
	return updatable_manager;
}

export inline void init_updatable_manager() {
	updatable_manager = new UpdatableManager();
}

export inline void delete_updatable_manager() {
	delete updatable_manager;
}
