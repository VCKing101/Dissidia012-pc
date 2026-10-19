#include "battle/runtime_helpers.h"

/* Called subobject dispatch wrappers retain their observed ABI and offsets. */
extern int func_089EA388(void *);
extern int func_089EA40C(void *);

int func_089E6CAC(unsigned char *object) { return func_089EA388(object + 0x124); }

int func_089E6CC8(unsigned char *object) { return func_089EA40C(object + 0x124); }
