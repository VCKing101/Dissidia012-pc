/* Leaves the online session if one is open and sets state 6; 1 when nothing was open. */
#include "main/agent17_types.h"
extern "C" {
void func_088FF894(void);
void func_088FF84C(void);
int func_088C99E4(Session *s)
{
    int result = 1;
    if (Session::online(s)) {
        s->flags.word &= ~2;
        func_088FF894();
        func_088FF84C();
        result = 0;
    }
    s->state = 6;
    return result;
}
}
