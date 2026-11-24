/* Steps the stage sound loader: waits for the stage, starts reading the
   stage's sound file, then copies the loaded data into its own block. */
typedef struct SoundLoader {
    void *block;
    char pad_004[0xF0 - 4];
    int heap;
    void *buf;
    int read_size;
    int size;
    unsigned int state;
} SoundLoader;

extern int D_089D6AC8;
extern char D_089CAB04[];

extern SoundLoader *func_08899300(void);
extern int func_08893CF8(int stage);
extern const char *func_0889381C(int stage);
extern void func_0893476C(char *buf, const char *fmt, const char *name);
extern void func_088629DC(char *path, void *buf, int size, void (*done)(void), int a, int b);
extern void func_08899344(void);
extern void *func_0885C6CC(int heap, int size, int align, int a, int b, int c);
extern void func_08935524(void *dst, void *src, int size);
extern void func_0889A2F0(SoundLoader *self, void *block);

void func_08899380(void)
{
    int stage = D_089D6AC8;
    SoundLoader *self = func_08899300();

    switch (self->state) {
    case 0:
        break;
    case 1:
        if (func_08893CF8(stage) == 0) {
            break;
        }
        self->state = 2;
    case 2: {
        char path[256];
        func_0893476C(path, D_089CAB04, func_0889381C(stage));
        func_088629DC(path, self->buf, self->read_size, func_08899344, 0, 1);
        self->state = 3;
        break;
    }
    case 3:
        break;
    case 4:
        self->block = func_0885C6CC(self->heap, self->size, 0x40, 0, 1, 0);
        func_08935524(self->block, self->buf, self->size);
        func_0889A2F0(self, self->block);
        self->buf = 0;
        self->read_size = 0;
        self->size = 0;
        self->state = 5;
        break;
    case 5:
        break;
    }
}
