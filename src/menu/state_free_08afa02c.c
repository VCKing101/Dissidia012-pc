/* Whether neither func_08AF9F1C nor func_08AF9F68 reports set. */
extern int func_08AF9F1C(void *self);
extern int func_08AF9F68(void *self);

int func_08AFA02C(void *self)
{
    if (func_08AF9F1C(self) != 0 || func_08AF9F68(self) != 0) {
        return 0;
    }
    return 1;
}
