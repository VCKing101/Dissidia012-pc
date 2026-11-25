/* Phase step on the field at 0xBDB8: phase 1 runs func_088CA564 for both slots and moves to 3; phase 2 moves to 4 and clears bit 0 of the shared flags. */
typedef struct O088C9938 { char pad[0xBDB8]; int phase; } O088C9938;
typedef struct G088C9938 { char pad[0x264]; unsigned int flags; } G088C9938;
extern G088C9938 *D_089D6008;
void func_088CA564(O088C9938 *o, int i);
void func_088C9938(O088C9938 *self)
{
    int i;
    switch (self->phase) {
    case 2:
        self->phase = 4;
        D_089D6008->flags &= 0xFFFFFFFE;
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            func_088CA564(self, i);
        }
        self->phase = 3;
        break;
    default:
        break;
    }
}
