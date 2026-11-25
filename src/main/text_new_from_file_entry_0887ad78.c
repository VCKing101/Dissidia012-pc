/* Draw line index of a text block into slot with two extra arguments; an empty line is drawn when the line is missing unless quiet. */
typedef struct Text0887AD78 {
    int count;
    char *data;
} Text0887AD78;
extern void *D_089D6998;
extern char D_09B49088[];
extern int func_0887A800(const char *data, int index, int count);
extern int func_0887A8F0(const char *data);
extern int func_0887A918(const char *data);
extern char *func_0887A940(char *line);
extern int func_0887D868(void *renderer, int slot, const char *line, int a, int b, int c, int d, int e);
int func_0887AD78(Text0887AD78 *self, int slot, int index, int c, int d, unsigned char quiet)
{
    int offset;
    int skip;
    char *line;

    if (self->data == 0) {
        return 0;
    }
    offset = func_0887A800(self->data, index, self->count);
    if (offset == 0) {
        if (quiet != 0) {
            return 0;
        }
        return func_0887D868(D_089D6998, 0, D_09B49088, -1, 0, 0, 0, 0);
    }
    skip = 2;
    if (func_0887A8F0(self->data) != 0) {
        skip = 4;
    }
    line = self->data + offset + skip;
    if (func_0887A918(self->data) != 0) {
        line = func_0887A940(line);
    }
    return func_0887D868(D_089D6998, slot, line, 0, 0, c, d, 0);
}
