#include "main/auto_region2.h"

/* Builds a 0x90-byte object on the stack (func_08849B50) and runs it (func_08849700). */
void func_0884A294(void)
{
    char object[144];

    func_08849B50(object);
    func_08849700(object);
}
