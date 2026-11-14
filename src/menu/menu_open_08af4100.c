/* Whether the menu may open: nothing pending and not locked by the global flag. */
extern unsigned char D_089D5F20;
extern int func_08AF9F1C(void *self);
extern int func_08AF9F94(void *self);
extern int func_08AF9FB4(void *self);
extern int func_08AF9870(void *self);

int func_08AF4100(void *self)
{
    int ok = 1;
    if (func_08AF9F1C(self) != 0 || func_08AF9F94(self) != 0 || func_08AF9FB4(self) != 0
        || func_08AF9870(self) != 0) {
        ok = 0;
    }
    if (D_089D5F20 == 1) {
        return 0;
    }
    return ok;
}
