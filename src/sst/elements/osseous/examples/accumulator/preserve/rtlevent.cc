// Copyright 2009-2020 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2020, NTESS
// All rights reserved.
//
// Portions are copyright of other developers:
// See the file CONTRIBUTORS.TXT in the top level directory
// the distribution for more information.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#include "uint.h"
#include "sint.h"
#include "rtlevent.h"

using namespace SST;
using namespace SST::RtlComponent;

void RTLEvent::UpdateRtlSignals(void *update_data, Accumulator* cmodel, uint64_t& cycles) {
    bool* update_rtl_params = (bool*)update_data; 
    update_inp = update_rtl_params[0];
    update_ctrl = update_rtl_params[1];
    update_eval_args = update_rtl_params[2];
    update_registers = update_rtl_params[3];
    verbose = update_rtl_params[4];
    done_reset = update_rtl_params[5];
    sim_done = update_rtl_params[6];
    uint64_t* cycles_ptr = (uint64_t*)(&update_rtl_params[7]);
    sim_cycles = *cycles_ptr;
    cycles = sim_cycles;
    cycles_ptr++;
    if(update_inp) {
        inp_ptr =  (void*)cycles_ptr; 
        input_sigs(cmodel);
    }

    if(update_ctrl) {
        UInt<8>* rtl_inp_ptr = (UInt<8>*)inp_ptr;
        ctrl_ptr = ++rtl_inp_ptr;
        control_sigs(cmodel);
    }
}

void RTLEvent::input_sigs(Accumulator* cmodel) {

    cmodel->reset = UInt<1>(1);

    UInt<1>* rtl_inp_ptr = (UInt<1>*)inp_ptr;
    cmodel->io_in = rtl_inp_ptr[0];
    fprintf(stderr, "\ninput_sigs: %" PRIu8, cmodel->io_in);
    ++rtl_inp_ptr;

    cmodel->reset = UInt<1>(0);

    return;
}

void RTLEvent::control_sigs(Accumulator* cmodel) {

    cmodel->reset = UInt<1>(1);

    cmodel->reset = UInt<1>(0);

    return;
}

