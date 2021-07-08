#pragma once

#include "lib.h"

struct FatalError : std::exception {
};

struct FatalGuard
{
    FatalGuard() {
        set_fatal_handler([](void* arg, const char* msg) { static_cast<FatalGuard*>(arg)->Fatal(msg); }, this);
    }

    ~FatalGuard() {
        set_fatal_handler(NULL, NULL);
    }

    void Fatal(const char* msg)
    {
        fatal_msg = msg;
        throw FatalError();
    }

    std::string fatal_msg;
};

