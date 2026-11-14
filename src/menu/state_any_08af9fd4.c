/* Whether the page is in state 4 or func_08AF9F68 reports set. */
extern int func_08AF9F48(void *self);
extern int func_08AF9F68(void *self);

int func_08AF9FD4(void *self)
{
    int result = 0;
    if (func_08AF9F48(self) != 0) {
        result = 1;
    } else if (func_08AF9F68(self) != 0) {
        result = 1;
    }
    return result;
}
