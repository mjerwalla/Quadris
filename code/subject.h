#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include <vector>

class Observer;

using std::vector;

class Subject {

  vector<Observer*> observers;

  public:
    void attachObserver(Observer & who);
    void notifyObservers();
    virtual ~Subject();
};

#endif /* _SUBJECT_H_ */
