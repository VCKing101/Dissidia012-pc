#include "main/state_prefixes.h"

/* Called by main 0x08889694, extra 0x089DEB84, and menu 0x089DE910.
 * Store the three callback addresses using their shared block declaration. */
void func_0886503C(MainStatePrefix *self, MainRegisteredCallback a,
                   MainRegisteredCallback b, MainRegisteredCallback c) {
    self->field_0018 = a;
    self->field_001C = b;
    self->field_0020 = c;
}
