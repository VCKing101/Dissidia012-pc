typedef struct Text0887B500 {
    void *data;                        /* +0x00 */
} Text0887B500;

extern char D_09B49088[];              /* the empty string */
extern char *func_0887B1B4(Text0887B500 *self, int index);
extern int func_0887A898(const char *line);
extern int func_0887A800(const char *line, int a, int b);
extern int func_0887A8F0(const char *line);

char *func_0887B500(Text0887B500 *self, int index, int a)
{
    char *line;
    int offset;
    int skip;

    if (self->data == 0) {
        return 0;
    }
    line = func_0887B1B4(self, index);
    offset = func_0887A800(line, a, func_0887A898(line));
    if (offset == 0) {
        return D_09B49088;
    }
    skip = 2;
    if (func_0887A8F0(line) != 0) {
        skip = 4;
    }
    return line + offset + skip;
}
