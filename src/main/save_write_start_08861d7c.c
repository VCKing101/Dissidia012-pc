typedef struct Save08861D7C {
    int state;                         /* +0x00 2 = started, 3 = failed */
    int file;                          /* +0x04 -1 = none */
    int unk_08;
    void *buffer;                      /* +0x0C */
    int size;                          /* +0x10 */
    void *heap;                        /* +0x14 */
} Save08861D7C;

extern int func_089C5D50(int file);
extern int func_08862F2C(int file, void *buffer, int size, void (*done)(void), Save08861D7C *self, int unk);
extern void func_08861EC4(void);
extern void func_0885C538(void *heap, void *block, int align);   /* heap_free */

void func_08861D7C(Save08861D7C *self, int ok)
{
    int failed = 1;

    if (ok >= 0 && func_089C5D50(self->file) >= 0 &&
        func_08862F2C(self->file, self->buffer, self->size, func_08861EC4, self, 1) == 1) {
        self->state = 2;
        failed = 0;
    }
    if (failed == 1) {
        func_0885C538(self->heap, self->buffer, 0);
        self->buffer = 0;
        self->size = 0;
        self->file = -1;
        self->state = 3;
    }
}
