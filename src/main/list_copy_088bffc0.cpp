/* Copies an id and its value list into an inline list. */
struct Source088BFFC0 {
    short id;
    short count;
    short *values;
};

struct List088BFFC0 {
    short id;
    short count;
    short values[1];
};

extern "C" {
void func_088BFFC0(List088BFFC0 *dst, Source088BFFC0 *src)
{
    dst->id = src->id;
    dst->count = src->count;
    for (int i = 0; i < dst->count; i++) {
        dst->values[i] = src->values[i];
    }
}
}
