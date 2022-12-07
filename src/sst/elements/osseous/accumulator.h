#ifndef __ACCUMULATOR_H__
#define __ACCUMULATOR_H__

class Accumulator {

public:
    Accumulator();
    ~Accumulator();

    void eval();
    void attach(uint32_t input);
};

#endif // COMPARATOR_SS_H
