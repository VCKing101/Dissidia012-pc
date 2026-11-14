#include "main/agent25_card.h"
extern int func_0892615C(CardUnit *unit);
extern char *func_0892614C(CardUnit *unit);
extern int func_08926154(CardUnit *unit);
char *func_08926164(CardUnit *unit)
{
    int tail = func_0892615C(unit);
    char *head = func_0892614C(unit);
    return head + 0x48C + func_08926154(unit) + tail;
}
