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
#include "rtl_header.h"
#include "rtlevent.h"

using namespace SST;
using namespace SST::RtlComponent;

void RTLEvent::UpdateRtlSignals(void *update_data, Rtlheader* cmodel, uint64_t& cycles, mm_magic_t* mem) {
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

    output.verbose(CALL_INFO, 1, 0, "sim_cycles: %\n" PRIu64, sim_cycles);
    output.verbose(CALL_INFO, 1, 0, "update_inp: %d\n", update_inp);
    output.verbose(CALL_INFO, 1, 0, "update_ctrl: %d\n", update_ctrl);
    if(update_inp) {
        inp_ptr =  (void*)cycles_ptr; 
        input_sigs(cmodel, mem);
    }

    /*if(update_ctrl) {
        UInt<1>* rtl_inp_ptr = (UInt<1>*)inp_ptr;
        ctrl_ptr = (void*)(&rtl_inp_ptr);
        control_sigs(cmodel);
    }*/
}

void RTLEvent::input_sigs(Rtlheader* cmodel, mm_magic_t* mem) {

    uint32_t i = 0;
    //load_mem(mem->get_data(), (const char*)inp_ptr); 
    char* inp_pt = (char*)inp_ptr;
    while(i < 9536) {
        mem->get_data()[i++] = *inp_pt;
        //printf("\nHex char is: %c", *inp_pt);
        //output.verbose(CALL_INFO, 1, 0, "\nhex char is: %s", *inp_pt);
        inp_pt++;
    }
    inp_ptr = (void*)&inp_pt;
    return;
}

/*void RTLEvent::control_sigs(Rtlheader* cmodel, void* ctrl_ptr) {
     output.verbose(CALL_INFO, 1, 0, "\nctrl_sigs called"); 
     UInt<1>* rtl_ctrl_ptr = (UInt<1>*)ctrl_ptr;
     cmodel->reset = rtl_ctrl_ptr[0];
     rtl_ctrl_ptr++;
     UInt<32>* ctrl_pt = (UInt<32>*)rtl_ctrl_ptr;
     cmodel->io_host_fromhost_bits = ctrl_pt[0];
     ctrl_pt++;
     UInt<1>* t = (UInt<1>*)ctrl_pt;
     cmodel->io_host_fromhost_valid = *t;
     printf("\n ctrl_sig: %" PRIu64, cmodel->io_host_fromhost_bits.as_single_word());
     printf("\n ctrl_sig: %" PRIu64, cmodel->io_host_fromhost_valid.as_single_word());
     return;
 }*/


