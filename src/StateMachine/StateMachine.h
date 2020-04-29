

#pragma once
#include "State.h"
#include <memory>
namespace MBGL
{
    class StateMachine {
    public:
        StateMachine();

        ~StateMachine() = default;

    private:
        std::unique_ptr<State> currentState;

    protected:
    };
}




