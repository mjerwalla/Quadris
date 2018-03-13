#ifndef _OBSERVER_H_
#define _OBSERVER_H_

class Subject;

class Observer {
  public:
    virtual void notify(Subject & whoNotified) = 0;
};

#endif /* _OBSERVER_H_ */
