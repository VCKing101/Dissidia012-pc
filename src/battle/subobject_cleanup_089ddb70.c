#include "battle/runtime_helpers.h"

/* Called subobject dispatch wrappers retain their observed ABI and offsets. */
extern int func_089E8028(void *);

int func_089DDB70(unsigned char *object) { return func_089E8028(object + 0x1DC); }
