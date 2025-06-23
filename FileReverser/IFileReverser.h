#pragma once

class IFileReverser {
public:
    virtual ~IFileReverser() = default;

    virtual bool process() = 0;
};