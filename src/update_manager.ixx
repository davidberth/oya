module;

#include <vector>
#include <algorithm>

export module updatable_manager;

import updatable;

export class UpdatableManager {
public:
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

export inline UpdatableManager& get_updatable_manager() {
	static UpdatableManager updatable_manager;
	return updatable_manager;
}
