#ifndef MUX4_H_
#define MUX4_H_

#include <iostream>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <uint.h>
#include <sint.h>
#define UNLIKELY(condition) __builtin_expect(static_cast<bool>(condition), 0)
using namespace std;
typedef struct Mux2 {

  Mux2() {
  }
} Mux2;

typedef struct Mux4 {
  UInt<1> clock;
  UInt<1> reset;
  UInt<1> io_in0;
  UInt<1> io_in1;
  UInt<1> io_in2;
  UInt<1> io_in3;
  UInt<2> io_sel;
  UInt<1> io_out;
  Mux2 m0;
  Mux2 m1;
  Mux2 m2;

  Mux4() {
    reset.rand_init();
    io_in0.rand_init();
    io_in1.rand_init();
    io_in2.rand_init();
    io_in3.rand_init();
    io_sel.rand_init();
    io_out.rand_init();
  }

  void eval(bool update_registers, bool verbose, bool done_reset) {
    cout << "io_in0: " << io_in0 << endl;
    cout << "io_in1: " << io_in1 << endl;
    cout << "io_in2: " << io_in2 << endl;
    cout << "io_in3: " << io_in3 << endl;
    cout << "io_sel: " << io_sel << endl;
    UInt<1> m2$io_sel = io_sel.bits<1,1>();
    UInt<1> m1$io_sel = io_sel.bits<0,0>();
    UInt<1> m1$_T = m1$io_sel & io_in3;
    UInt<1> m1$_T_1 = ~m1$io_sel;
    UInt<1> m1$_T_2 = m1$_T_1 & io_in2;
    UInt<1> m1$io_out = m1$_T | m1$_T_2;
    UInt<1> m2$_T = m2$io_sel & m1$io_out;
    UInt<1> m2$_T_1 = ~m2$io_sel;
    UInt<1> m0$io_sel = io_sel.bits<0,0>();
    UInt<1> m0$_T = m0$io_sel & io_in1;
    UInt<1> m0$_T_1 = ~m0$io_sel;
    UInt<1> m0$_T_2 = m0$_T_1 & io_in0;
    UInt<1> m0$io_out = m0$_T | m0$_T_2;
    UInt<1> m2$_T_2 = m2$_T_1 & m0$io_out;
    UInt<1> m2$io_out = m2$_T | m2$_T_2;
    io_out = m2$io_out;
    cout << "out: " << io_out << endl;
  }
} Mux4;

#endif  // MUX4_H_
