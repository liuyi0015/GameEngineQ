//
// Created by XL0002 on 2026/9/17.
//

#ifndef GAMEENGINEQ_APPLICATION_H
#define GAMEENGINEQ_APPLICATION_H


class Application {
public:
    virtual void init()=0;
    virtual void start()=0;
    virtual void fixed_update(double deltaTime)=0;
    virtual void update(double deltaTime)=0;
    virtual void draw()=0;
};


#endif //GAMEENGINEQ_APPLICATION_H
