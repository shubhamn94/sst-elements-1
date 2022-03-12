#ifndef ACCUMULATOR_H_
#define ACCUMULATOR_H_

#include <iostream>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <uint.h>
#include <sint.h>
#define UNLIKELY(condition) __builtin_expect(static_cast<bool>(condition), 0)

typedef struct Accumulator {
  UInt<8> accumulator;
  UInt<1> clock;
  UInt<1> reset;
  UInt<1> io_in;
  UInt<8> io_out;

  Accumulator() {
    accumulator.rand_init();
    reset.rand_init();
    io_in.rand_init();
    io_out.rand_init();
  }

  void eval(bool update_registers, bool verbose, bool done_reset) {
    UInt<8> _GEN_0 = io_in.pad<8>();
    cout << "New val is: " << _GEN_0 << endl;
    UInt<9> _T = accumulator + _GEN_0;
    cout << "Accumulator was: " << accumulator << endl;
    cout << "Accumulator is:  " << _T << endl;
    UInt<8> _T_1 = _T.tail<1>();
    io_out = accumulator;
    cout << "update regs is: " << update_registers << endl;
    cout << "reset is : " << reset << endl;
    cout << "tail is: " << _T_1 << endl;
    if (update_registers) accumulator = reset ? UInt<8>(0x0) : _T_1;
  }
} Accumulator;

#endif  // ACCUMULATOR_H_
