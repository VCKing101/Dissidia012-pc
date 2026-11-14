/* Stops an effect when bit 0 of the flags is set. */
extern void func_08992F04(void *effect);

void func_08922898(void *effect, int flags)
{
    if (effect != 0 && (flags & 1) != 0) {
        func_08992F04(effect);
    }
}
