/* Polls the online session; 0 when offline. */
#include "main/agent17_types.h"
extern "C" {
int func_088FF908(void);
int func_088C9AA4(Session *s)
{
    if (Session::online(s)) {
        return func_088FF908();
    }
    return 0;
}
}
