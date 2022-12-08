#ifndef __ACCUMULATOR_H__
#define __ACCUMULATOR_H__

class Accumulator {

public:
    Accumulator();
    ~Accumulator();
    void eval();
    void attach (uint32_t input);

private:
    // PIMPL: Pointer to Implementation - C++ programming technique
    // Remove the implementation details of a class from
    // its object representation by placing them in a separate class
    class Impl;
    Impl *impl_;
};

#endif // COMPARATOR_SS_H