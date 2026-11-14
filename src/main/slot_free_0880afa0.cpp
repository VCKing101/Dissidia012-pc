#include "main/agent30_types.h"

/* Returns a task slot to the global slot set. */
extern "C" {

extern BitSet256 D_089D4750;

void func_0880AFA0(unsigned int slot)
{
    BitSet256::remove(&D_089D4750, slot);
}

}
