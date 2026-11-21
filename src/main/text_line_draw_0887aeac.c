typedef struct Text0887AEAC {
    int count;                         /* +0x00 */
    char *data;                        /* +0x04 */
} Text0887AEAC;

extern void *D_089D6998;               /* the text renderer */
extern char D_09B49088[];              /* the empty string */
extern int func_0887A800(const char *data, int index, int count);
extern int func_0887A8F0(const char *data);
extern int func_0887D868(void *renderer, int slot, const char *line, int a, int b, int c, int d, int e);

/* Draw line `index` of a text block into `slot` (an empty line when it is missing). */
int func_0887AEAC(Text0887AEAC *self, int slot, int index, int c, int d)
{
    int offset;
    int skip;

    if (self->data == 0) {
        return 0;
    }
    offset = func_0887A800(self->data, index, self->count);
    if (offset == 0) {
        return func_0887D868(D_089D6998, 0, D_09B49088, -1, 0, 0, 0, 0);
    }
    skip = 2;
    if (func_0887A8F0(self->data) != 0) {
        skip = 4;
    }
    return func_0887D868(D_089D6998, slot, self->data + offset + skip, 0, 1, c, d, 0);
}
