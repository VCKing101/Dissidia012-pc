/* Resets the reader, attaches a source and starts it: -1 without a source,
   -2 when starting fails. */
typedef struct Reader088206E8 {
    unsigned char open;
    char pad_01[3];
    void *source;
} Reader088206E8;

void func_088206CC(Reader088206E8 *r);
int func_08820750(Reader088206E8 *r);

int func_088206E8(Reader088206E8 *r, void *source)
{
    func_088206CC(r);
    if (source == 0) {
        return -1;
    }
    r->source = source;
    r->open = 1;
    if (func_08820750(r) != 0) {
        return -2;
    }
    return 0;
}
