/* If the three checks on the global object pass, runs one of two handlers and stores a state id. */
extern int func_088ACAC4(void *g);
extern int func_088AC9A8(void *g);
extern int func_088ACAEC(void *g);
extern void func_08B39834(void *self);
extern void func_08B39C9C(void *self, int a);
extern void func_08B398CC(void *self);
extern void *D_089D712C;

typedef struct S37828 { char pad[0xC4]; int *state; } S37828;

void func_08B37828(S37828 *self)
{
    if (func_088ACAC4(D_089D712C) && func_088AC9A8(D_089D712C)) {
        if (func_088ACAEC(D_089D712C)) {
            func_08B39834(self);
            func_08B39C9C(self, 1);
            *self->state = 7;
        } else {
            func_08B398CC(self);
            *self->state = 0x1D;
        }
    }
}
