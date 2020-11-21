#include "Subject.hpp"

void Subject::attach(Observer* observer) {
    this->observers.push_back(observer);
}

void Subject::attach(vector<Observer*>& observers) {
    for (auto observer : observers) {
        this->attach(observer);
    }
}

void Subject::detach(Observer* observer) {
    auto p = find(this->observers.begin(), this->observers.end(), observer);

    if (p != this->observers.end()) this->observers.erase(p);
}

void Subject::notify() {
    for (auto observer : this->observers) {
        observer->update();
    }
}
