extern "C" {
/* The stage loader (the object at *(0x089D6AC8)), only the fields its state machine uses. */
typedef struct StageLoader {
    unsigned char unk_000[0x4E0];
    unsigned char sky_model[0x130];    /* +0x4E0 */
    unsigned char sky_loaded;          /* +0x610 */
    unsigned char unk_611[0x6B2 - 0x611];
    unsigned char stage_loaded;        /* +0x6B2 */
    unsigned char unk_6B3[0x714 - 0x6B3];
    int has_sky;                       /* +0x714 */
    void *buffer;                      /* +0x718 file buffer */
    int buffer_size;                   /* +0x71C capacity */
    int read_size;                     /* +0x720 bytes read (I: set when the read finishes) */
    int state;                         /* +0x724 */
    void *desc;                        /* +0x728 stage descriptor */
} StageLoader;

typedef struct GameState08893218 {
    unsigned char unk_000[0x100];
    int mode;                          /* +0x100 */
} GameState08893218;

extern StageLoader *D_089D6AC8;
extern char D_089CA798[];              /* "map/%s.gmo" */
extern char D_089CA7A8[];              /* "SKY" */

extern int func_08893CF8(StageLoader *self);
extern GameState08893218 *func_08899300(void);
extern const char *func_0889381C(StageLoader *self);                         /* stage map name */
extern const char *func_08893828(StageLoader *self);                         /* sky map name */
extern int func_0893476C(char *dst, const char *format, ...);                /* sprintf */
extern int func_088629DC(const char *path, void *dst, int size, void (*done)(void), int t0, unsigned char t1); /* pkg_read_sync */
extern void func_08892E84(void);                                             /* read callback (I: moves to the next state) */
extern void func_08893FF4(StageLoader *self, void *data, int size, void *desc, unsigned char super_skill); /* stage_setup2 */
extern void func_08894F38(void *slot, const char *name, void *data, int size); /* stage_model_setup */

}
struct StageWait {
    /* The game is busy unless its mode is 0 or 5. */
    static unsigned char busy() {
        int busy = 0;
        if (func_08899300()->mode != 0) {
            if (func_08899300()->mode != 5) {
                busy = 1;
            }
        }
        return busy;
    }
};
extern "C" {
/* stage_load_update: one step of the stage loader. States 2-5 read map/<stage>.gmo into the
 * buffer (asynchronously, through pkg_read_sync) and build the stage with stage_setup2;
 * states 6-9 read the sky the same way and set it up as the "SKY" model (stage_model_setup,
 * textures to VRAM). State 10 = done. */
void func_08893218(void)
{
    StageLoader *self = D_089D6AC8;

    switch (self->state) {
    case 0:
        break;
    case 1:
        if (!func_08893CF8(self) || StageWait::busy()) {
            break;
        }
        self->state = 2;
        break;
    case 2: {
        char path[0x100];

        func_0893476C(path, D_089CA798, func_0889381C(self));
        self->state = 3;
        func_088629DC(path, self->buffer, self->buffer_size, func_08892E84, 0, 1);
        break;
    }
    case 3:
        break;
    case 4:
        self->state = 5;
        break;
    case 5:
        func_08893FF4(self, self->buffer, self->read_size, self->desc, 0);
        self->stage_loaded = 1;
        self->state = 10;
        if (self->has_sky != 0) {
            self->state = 6;
        }
        break;
    case 6: {
        char path[0x100];

        func_0893476C(path, D_089CA798, func_08893828(self));
        self->state = 7;
        func_088629DC(path, self->buffer, self->buffer_size, func_08892E84, 0, 1);
        break;
    }
    case 7:
        break;
    case 8:
        self->state = 9;
        break;
    case 9:
        func_08894F38(self->sky_model, D_089CA7A8, self->buffer, self->buffer_size);
        self->sky_loaded = 1;
        self->state = 10;
        break;
    case 10:
        break;
    }
}
}
