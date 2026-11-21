typedef struct Text0887AFBC {
    int count;                         /* +0x00 */
    char *data;                        /* +0x04 */
} Text0887AFBC;

extern char D_09B49088[];              /* the empty string */
extern int func_0887A800(const char *data, int index, int count);
extern int func_0887A8F0(const char *data);
extern int func_0887A918(const char *data);
extern char *func_0887A940(char *line);

/* Line `index` of a text block (after its 2- or 4-byte header); an empty string for a missing
 * line, or 0 when `strict`. */
char *func_0887AFBC(Text0887AFBC *self, int index, unsigned char strict)
{
    int offset;
    int skip;
    char *line;

    if (self->data == 0) {
        return 0;
    }
    offset = func_0887A800(self->data, index, self->count);
    if (offset == 0) {
        if (strict) {
            return 0;
        }
        return D_09B49088;
    }
    skip = 2;
    if (func_0887A8F0(self->data) != 0) {
        skip = 4;
    }
    line = self->data + offset + skip;
    if (func_0887A918(self->data) != 0) {
        line = func_0887A940(line);
    }
    return line;
}
