/* The part of a seq (layout) object this reader fills (see seq_load, FUN_088DCBA8). */
typedef struct SeqBlockReader {
    void *data;                        /* +0x00 destination of the copied block (0 = skip) */
    unsigned char unk_04[0x1C - 0x4];
    int mode;                          /* +0x1C 1: a table of `count` 32-byte records */
    int count;                         /* +0x20 */
    unsigned char unk_24[0x3C - 0x24];
    int size;                          /* +0x3C block size in the other mode */
} SeqBlockReader;

extern int func_088DC9DC(SeqBlockReader *r, const char *src, void *data);   /* read the record table */
extern void *func_08935524(void *dst, const void *src, unsigned int n);      /* memcpy */

/* seq_read_block: read the next block of a seq layout file at `src`; returns where the
 * following block starts and, when asked, the size read. */
const char *func_088DC944(SeqBlockReader *r, const char *src, int *out_size)
{
    int size;

    if (r->mode == 1) {
        size = func_088DC9DC(r, src, r->data);
        src += r->count * 32;
    } else {
        if (r->data != 0) {
            func_08935524(r->data, src, r->size);
        }
        size = r->size;
        src += size;
    }
    if (out_size != 0) {
        *out_size = size;
    }
    return src;
}
