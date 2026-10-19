#include "battle/runtime_helpers.h"

/* Called subobject dispatch wrappers retain their observed ABI and offsets. */
extern int func_089EDCD4(void *);
extern int func_089E5CAC(void *);

int func_089E5644(unsigned char *object) { return func_089EDCD4(object + 0x84); }

int func_089E5660(unsigned char *object) { return func_089E5CAC(object + 0x40); }

int func_089E567C(unsigned char *object) { return func_089E5BDC((BattleControlState *)(object + 0x40)); }

int func_089E5698(unsigned char *object) { return func_089E5C28((BattleControlState *)(object + 0x40)); }

int func_089E56B4(unsigned char *object) { return func_089E5C60((BattleControlState *)(object + 0x40)); }

int func_089E56D0(unsigned char *object) { return func_089E5C88((BattleControlState *)(object + 0x40)); }
