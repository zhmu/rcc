#pragma once

#include "scan.h"

struct ScannerFeeder
{
    ScannerFeeder(std::string input)
        : input(std::move(input))
    {
        scan_set_fn([](void* arg) { return static_cast<ScannerFeeder*>(arg)->FetchChar(); }, this);
        scan_set_pending_char(0);
    }

    ~ScannerFeeder()
    {
        scan_set_fn(NULL, NULL);
    }

    int FetchChar()
    {
        if (pos >= input.size())
            return EOF;
        auto ch = input[pos];
        ++pos;
        return ch;
    }

    size_t pos{};
    std::string input;
};

