#pragma once

class State {
public:
    virtual void update()=0;
    virtual void render()=0;
    virtual void processInputs()=0;
    ~State() = default;
};



