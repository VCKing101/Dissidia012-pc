/* Stores a halfword in a table at 0x2434 indexed by a1. */
typedef struct S2434 { char pad[0x2434]; short t[1]; } S2434;
void func_08B42F70(S2434 *self, int i, int v) { self->t[i] = v; }
