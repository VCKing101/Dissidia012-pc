#include "battle/runtime_helpers.h"

/* Called subobject dispatch wrappers retain their observed ABI and offsets. */
extern int func_089EA22C(void *);
extern int func_089E2044(void *);

int func_089E6B30(unsigned char *object) { return func_089EA22C(object + 0x124); }

int func_089E6B4C(unsigned char *object) { return func_089E2044(object + 8); }
