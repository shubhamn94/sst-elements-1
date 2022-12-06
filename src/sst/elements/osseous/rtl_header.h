#ifndef ACCUMULATOR_H_
#define ACCUMULATOR_H_

#include <array>
#include <cstdint>
#include <cstdlib>
#include <uint.h>
#include <sint.h>
#define UNLIKELY(condition) __builtin_expect(static_cast<bool>(condition), 0)

typedef struct Rtlheader {
  UInt<8> accumulator;
  UInt<1> clock;
  UInt<1> reset;
  UInt<8> io_in;
  UInt<8> io_out;

  Accumulator() {
    accumulator.rand_init();
    reset.rand_init();
    io_in.rand_init();
    io_out.rand_init();
  }

  UInt<1> reset$old;
  UInt<8> io_in$old;
  std::array<bool,1> PARTflags;
  bool sim_cached = false;
  bool regs_set = false;
  bool update_registers;
  bool done_reset;
  bool verbose;

  void EVAL_0() {
    PARTflags[0] = false;
    io_out = accumulator;
    printf("\nio_out is: %"PRIu8, io_out);
    printf("\nio_in is: %" PRIu8, io_in);
    UInt<8> accumulator$next;
    if (UNLIKELY(UNLIKELY(reset))) {
      accumulator$next = UInt<8>(0x0);
    } else {
      UInt<8> _GEN_0 = io_in.pad<8>();
      UInt<9> _T = accumulator + _GEN_0;
      UInt<8> _T_1 = _T.tail<1>();
      accumulator$next = _T_1;
    }
    PARTflags[0] |= accumulator != accumulator$next;
    if (update_registers) accumulator = accumulator$next;
  }

  void eval(bool update_registers, bool verbose, bool done_reset) {
    if (reset || !done_reset) {
      sim_cached = false;
      regs_set = false;
    }
    if (!sim_cached) {
      PARTflags.fill(true);
    }
    sim_cached = regs_set;
    this->update_registers = update_registers;
    this->done_reset = done_reset;
    this->verbose = verbose;
    PARTflags[0] |= reset != reset$old;
    PARTflags[0] |= io_in != io_in$old;
    reset$old = reset;
    io_in$old = io_in;
    if (UNLIKELY(PARTflags[0])) EVAL_0();
    regs_set = true;
  }
} Rtlheader;

#endif  // ACCUMULATOR_H_
