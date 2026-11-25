/* Sets word 0x50 to 1 and float 0x54 to 0. */
typedef struct F50 { char pad[0x50]; int a; float b; } F50;
void func_08B55ED0(F50 *self) { int one = 1; self->a = one; self->b = 0.0f; }
