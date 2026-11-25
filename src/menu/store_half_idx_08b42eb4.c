/* Stores a halfword in a table at 0x2424 indexed by a1. */
typedef struct S2424 { char pad[0x2424]; short t[1]; } S2424;
void func_08B42EB4(S2424 *self, int i, int v) { self->t[i] = v; }
