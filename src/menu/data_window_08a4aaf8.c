/* Opens the message window for the work block's data (+0xBE8), when there
 * is any. */
typedef struct Work4AAF8 {
    char pad_000[0x88C];
    void *messages;           /* +0x88C */
    char pad_890[0x358];
    void *data;               /* +0xBE8 */
} Work4AAF8;

typedef struct Screen4AAF8 {
    char pad_000[0x138];
    void *heap;               /* +0x138 */
    char pad_13C[0x10];
    Work4AAF8 *work;          /* +0x14C */
} Screen4AAF8;

extern void *D_089D7F90;
extern char D_08A4BE06[];
void func_088FD928(void *object, void *heap, int a, int b, void *name, int c, void *messages, int d,
                   int e, int f, int g, void *data);

void func_08A4AAF8(Screen4AAF8 *self)
{
    void *data = self->work->data;

    if (data == 0) {
        return;
    }
    func_088FD928(D_089D7F90, self->heap, 0x12, 0x20, D_08A4BE06, -1, self->work->messages, 0xD, 0, 0, 0, data);
}
