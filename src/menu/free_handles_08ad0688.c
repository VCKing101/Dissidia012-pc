/* Frees the handles at indices 0xA4..0xC0 of the screen block's handle list (from +0x880), resets the block's text, and releases the owner's resource. */
typedef struct Blk08AD0688 {
    char pad0[0x400];
    char text[0x480];
    int h[0xC1];
} Blk08AD0688;
typedef struct S08AD0688 { char pad[0x10000]; } S08AD0688;
typedef struct Own08AD0688 { char pad[0x18]; int w18; } Own08AD0688;
typedef struct Glob08AD0688 { char pad[0x140]; Own08AD0688 *own; } Glob08AD0688;
extern Glob08AD0688 *D_08B01CF8;
extern int D_089D6998;
extern const char D_08AD91C4[];
extern void func_0887E798(int a, int b);
extern void func_0887BA5C(int h);
extern void func_088D3C24(Blk08AD0688 *b, char *text, const char *name, int n);
extern void func_0887E0DC(int a, int b);
extern void func_0887E7B4(int a);
#define BLK08AD0688(s) (*(Blk08AD0688 **)((char *)(s) + 0x10000 - 0x24D0))
void func_08AD0688(S08AD0688 *self)
{
    int i;
    func_0887E798(D_089D6998, D_08B01CF8->own->w18);
    for (i = 0xA4; i < 0xC1; i++) {
        if (BLK08AD0688(self)->h[i] != 0) {
            func_0887BA5C(BLK08AD0688(self)->h[i]);
            BLK08AD0688(self)->h[i] = 0;
        }
    }
    func_088D3C24(BLK08AD0688(self), BLK08AD0688(self)->text, D_08AD91C4, 0x120);
    func_0887E0DC(D_089D6998, 1);
    func_0887E7B4(D_089D6998);
}
