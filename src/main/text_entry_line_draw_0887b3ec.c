typedef struct Text0887B3EC {
    void *data;                        /* +0x00 */
} Text0887B3EC;

extern void *D_089D6998;               /* the text renderer */
extern char D_09B49088[];              /* the empty string */
extern char *func_0887B1B4(Text0887B3EC *self, int index);
extern int func_0887A898(const char *s);
extern int func_0887A800(const char *s, int a, int b);
extern int func_0887A8F0(const char *s);
extern int func_0887D868(void *renderer, int slot, const char *line, int a, int b, int c, int d, int e);

/* Draw entry `index`, line `number`, of a text into `slot` (an empty line when it is missing). */
int func_0887B3EC(Text0887B3EC *self, int index, int slot, int number, int c, int d)
{
    char *line;
    int offset;
    int skip;

    if (self->data == 0) {
        return 0;
    }
    line = func_0887B1B4(self, index);
    offset = func_0887A800(line, number, func_0887A898(line));
    if (offset == 0) {
        return func_0887D868(D_089D6998, 0, D_09B49088, -1, 0, 0, 0, 0);
    }
    skip = 2;
    if (func_0887A8F0(line) != 0) {
        skip = 4;
    }
    return func_0887D868(D_089D6998, slot, line + offset + skip, 0, 0, c, d, 0);
}
