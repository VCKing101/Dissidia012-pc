/* Stores a float at offset 0x28 of the second argument. */
typedef struct S28 { char pad[0x28]; float v; } S28;
void func_08B43F1C(float v, S28 *self) { self->v = v; }
