#include "main/agent2_types.h"

extern Agent2GlobalState *D_089D5EC0;
extern void func_08866010(Agent2GlobalState *, unsigned char, unsigned char,
                          unsigned char);

void func_08866AA8(unsigned char red, unsigned char green, unsigned char blue) {
    func_08866010(D_089D5EC0, red, green, blue);
}
