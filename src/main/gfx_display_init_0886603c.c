/* The graphics object, only the fields used here. */
typedef struct Gfx0886603C {
    unsigned char unk_0000[0x10];
    unsigned char clear_state[0x10];   /* +0x0010 */
    unsigned char unk_0020[0x2CCF - 0x20];
    unsigned char color32;             /* +0x2CCF 1: 32-bit colour framebuffers */
} Gfx0886603C;

extern unsigned char D_08B63500[];     /* the initial display list */
extern unsigned int D_09B26AB0;        /* bytes of EDRAM used by the frame and depth buffers */
extern unsigned int D_09AFEE54;

/* libGu */
extern void func_0899F468(void);                                         /* sceGuInit */
extern void func_0899F5A4(int context, void *list, int size);            /* sceGuStart */
extern void func_0899FCAC(int psm, void *fbp, int width);               /* sceGuDrawBuffer */
extern void func_089A026C(int width, int height, void *dbp, int stride); /* sceGuDispBuffer */
extern void func_089A0330(void *zbp, int width);                        /* sceGuDepthBuffer */
extern void func_089A0BE4(int x, int y);                                 /* sceGuOffset */
extern void func_089A00D8(void);                                         /* sceGuFinish */
extern void func_089A0190(int mode, int what);                           /* sceGuSync */
extern void func_089A2310(void *state, int a, int b, int c, int d);

/* gfx_display_init: set up the frame buffers. The EDRAM above them (from 0x04000000 +
 * D_09B26AB0) is what the GRAPHICS::EDRAM heap hands out for textures: 0x110000 bytes are
 * kept with 16-bit colour (draw 0, display 0x44000, depth 0x88000), 0x154000 with 32-bit. */
void func_0886603C(Gfx0886603C *gfx)
{
    func_0899F468();
    func_0899F5A4(0, D_08B63500, 0x100);
    if (gfx->color32 != 0) {
        D_09B26AB0 = 0x154000;
        D_09AFEE54 = 0;
        func_0899FCAC(3, (void *)0, 0x200);              /* GU_PSM_8888 */
        func_089A026C(480, 272, (void *)0x88000, 0x200);
        func_089A0330((void *)0x110000, 0x200);
    } else {
        D_09B26AB0 = 0x110000;
        D_09AFEE54 = 0xCC000;
        func_0899FCAC(1, (void *)0, 0x200);              /* GU_PSM_5551 */
        func_089A026C(480, 272, (void *)0x44000, 0x200);
        func_089A0330((void *)0x88000, 0x200);
    }
    func_089A0BE4(2048 - 480 / 2, 2048 - 272 / 2);
    func_089A00D8();
    func_089A0190(0, 0);
    func_089A2310(gfx->clear_state, 4, 4, 0x10, 4);
}
