/* Stops the online session and resets it when online. */
#include "main/agent17_types.h"
extern "C" {
void func_088FF8C8(void);
void func_088C9938(Session *s);
void func_088C9A54(Session *s)
{
    if (Session::online(s)) {
        func_088FF8C8();
        func_088C9938(s);
    }
}
}
