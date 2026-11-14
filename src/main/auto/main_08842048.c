#include "main/auto_region2.h"

/* The chara after prev (the first one when prev is 0). */
Chara *func_08842048(Chara *prev)
{
    return (prev == 0) ? D_089D4958->charas : prev->next;
}
