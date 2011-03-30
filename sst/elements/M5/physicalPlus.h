#ifndef _physicalPlus_h
#define _physicalPlus_h

#include <sst_config.h>
#include <sst/core/component.h>
#include <mem/physical.hh>

class M5;
class MemLink;

struct PhysicalMemoryPlusParams : public PhysicalMemoryParams
{
    M5*         m5Comp;
    SST::Params params;
};

class MemLink;

class PhysicalMemoryPlus : public PhysicalMemory 
{
  public:
    PhysicalMemoryPlus( const PhysicalMemoryPlusParams* p );

  private:
    MemLink* m_link;
};

#endif

