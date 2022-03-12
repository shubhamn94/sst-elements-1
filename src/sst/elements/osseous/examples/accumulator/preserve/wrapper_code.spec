
// Start header
    SST_ELI_REGISTER_COMPONENT(
	dut,
	"dut",
	"dut",
	SST_ELI_ELEMENT_VERSION( 1, 0, 0 ),
	"Demonstration of an External Element for SST",
	COMPONENT_CATEGORY_PROCESSOR
    )    
    SST_ELI_DOCUMENT_STATISTICS(
        { "read_requests",        "Statistic counts number of read requests", "requests", 1},   // Name, Desc, Enable Level
        { "write_requests",       "Statistic counts number of write requests", "requests", 1},
        { "read_request_sizes",   "Statistic for size of read requests", "bytes", 1},   // Name, Desc, Enable Level
        { "write_request_sizes",  "Statistic for size of write requests", "bytes", 1},
        { "split_read_requests",  "Statistic counts number of split read requests (requests which come from multiple lines)", "requests", 1},
        { "split_write_requests", "Statistic counts number of split write requests (requests which are split over multiple lines)", "requests", 1},
	    { "flush_requests",       "Statistic counts instructions which perform flushes", "requests", 1},
        { "fence_requests",       "Statistic counts instructions which perform fences", "requests", 1}
    )
    //Parameters will mostly be just frequency/clock in the design. User will mention specifically if there could be other parameters for the RTL design which needs to be configured before runtime.  Don't mix RTL input/control signals with SST parameters. SST parameters of RTL design will make the RTL design/C++ model synchronous with rest of the SST full system.   
	SST_ELI_DOCUMENT_PARAMS(
		{ "ExecFreq", "Clock frequency of RTL design in GHz", "1GHz" },
		{ "maxCycles", "Number of Clock ticks the simulation must atleast execute before halting", "1000" },
        {"memoryinterface", "Interface to memory", "memHierarchy.memInterface"}
	)

    //Default will be single port for communicating with Ariel CPU. Need to see the requirement/use-case of multi-port design and how to incorporate it in our parser tool.  
    SST_ELI_DOCUMENT_PORTS(
        {"ArielRtllink", "Link to the Rtlmodel", { "Rtlmodel.RTLEvent", "" } },
        {"RtlCacheLink", "Link to Cache", {"memHierarchy.memInterface" , ""} }
    )
    
    SST_ELI_DOCUMENT_SUBCOMPONENT_SLOTS(
            {"memmgr", "Memory manager to translate virtual addresses to physical, handle malloc/free, etc.", "SST::RtlComponent::RtlMemoryManager"},
            {"memory", "Interface to the memoryHierarchy (e.g., caches)", "SST::Interfaces::SimpleMem" }
    )

//Placeholder for Boiler Plate code

void init() {
    //Any initialization code or binary/hex file to be loaded to memHierarchy
}

//void setup() {
//    dut->reset = UInt<1>(1);
//    axiport->reset = UInt<1>(1);
//	output.verbose(CALL_INFO, 1, 0, "Component is being setup.\n");
//}

//void finish() {
//	output.verbose(CALL_INFO, 1, 0, "Component is being finished.\n");
//    free(getBaseDataAddress());
//}

void ClockTick() {
    //Code or piece of logic to be executed other than eval() call at every SST Clock 
}

void handleRTLEvent() {
    //Piece of logic other than updating input signals at stimulus based on input_port spec file
}


// End header


//For event

//input_sigs() {
    //Update necessary input signals, at stimulus
//}

//ctrl_sigs() {
    //Update necessary ctrl signals, at stimulus
//}

