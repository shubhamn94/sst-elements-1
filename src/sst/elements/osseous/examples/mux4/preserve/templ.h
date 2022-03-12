// Copyright 2009-2020 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2020, NTESS
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#ifndef _SIMPLE_VECTORSHIFTREG_H
#define _SIMPLE_VECTORSHIFTREG_H

#include <sst/core/component.h>
#include <sst/core/interfaces/simpleMem.h>
#include <sst/core/timeConverter.h>

#include <sst/core/link.h>
#include <sst/core/clock.h>
#include <sst/core/eli/elementinfo.h>
#include "rtlevent.h"
#include "arielrtlev.h"
#include "rtlreadev.h"
#include "rtlwriteev.h"
#include "rtlmemmgr.h"

namespace SST {
    namespace RtlComponent {

class dut : public SST::Component {

public:
	dut( SST::ComponentId_t id, SST::Params& params );
    ~dut();

	void setup();
    void init(unsigned int);
	void finish();

	bool clockTick( SST::Cycle_t currentCycle );

    void generateReadRequest(RtlReadEvent* rEv);
    void generateWriteRequest(RtlWriteEvent* wEv);
    void setDataAddress(uint8_t*);
    uint8_t* getDataAddress();
    void setBaseDataAddress(uint8_t*);
    uint8_t* getBaseDataAddress();

private:
	SST::Output output;
    
    //RTL Clock frequency of execution and maximum Cycles/clockTicks for which RTL simulation will run.
    std::string RTLClk;
	SST::Cycle_t maxCycles;

    void handleArielEvent(SST::Event *ev);
    void handleMemEvent(Interfaces::SimpleMem::Request* event);
    SST::Link* ArielRtlLink;
    Interfaces::SimpleMem* cacheLink;
    void commitReadEvent(const uint64_t address, const uint64_t virtAddr, const uint32_t length);
    void commitWriteEvent(const uint64_t address, const uint64_t virtAddr, const uint32_t length, const uint8_t* payload);
    void sendArielEvent();
    
    TimeConverter* timeConverter;
    Clock::HandlerBase* clock_handler;
    bool writePayloads;
    RTLEvent ev;
    TYPE *cmodel;
    SST::ArielComponent::ArielRtlEvent* RtlAckEv;
    size_t inp_size, ctrl_size, updated_rtl_params_size;
    void* inp_ptr = nullptr;
    void* updated_rtl_params = nullptr;
    RtlMemoryManager* memmgr;
    bool mem_allocated = false;
    uint64_t sim_cycle;

    std::unordered_map<Interfaces::SimpleMem::Request::id_t, Interfaces::SimpleMem::Request*>* pendingTransactions;
    std::unordered_map<uint64_t, uint64_t> VA_VA_map;
    uint32_t pending_transaction_count;

    bool isStalled;
    uint64_t cacheLineSize;
    uint8_t *dataAddress, *baseDataAddress;

    uint64_t tickCount;
    uint64_t dynCycles; 
};

 } //namespace RtlComponent 
} //namespace SST
#endif //_SIMPLE_VECTORSHIFTREG_H
