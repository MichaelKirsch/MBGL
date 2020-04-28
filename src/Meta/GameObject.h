

#pragma once

class GameObject {
public:
    virtual void init()=0;
    virtual void update()=0;
    virtual void render()=0;
    virtual ~GameObject() = default;
};



