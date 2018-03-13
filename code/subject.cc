#include "subject.h"
#include "observer.h"

#include <vector>

void Subject::attachObserver(Observer & who) {
    observers.emplace_back(&who); // add observer pointer to the vector
}

void Subject::notifyObservers() {
    for (auto & obs : observers) {
        obs->notify(*this); // notify observers that I've changed
    }
}

Subject::~Subject() {}

