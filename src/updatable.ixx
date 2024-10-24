module;

export module updatable;

export class Updatable {
public:
    virtual ~Updatable() = default;
    virtual void update() = 0;

    void set_enabled(bool enabled) {
        this->enabled = enabled;
    }

    bool is_enabled() const {
        return enabled;
    }

private:
    bool enabled = true;
};