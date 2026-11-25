/* Stores a halfword at offset 0x2430. */
typedef struct S2430 { char pad[0x2430]; short v; } S2430;
void func_08B42EC4(S2430 *self, int v) { self->v = v; }
