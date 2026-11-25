/* Frees the screen block's resources: runs func_088D3A04 on it, frees the 27 handles from +0x884 and the one at +0x880, releases the screen sound/effect and sets the state word +0xA04 to 3. */
typedef struct Blk08AC5F28 {
    char pad0[0x880];
    int h880;
    int h884[27];
    char pad[0xA04 - 0x8F0];
    int wA04;
} Blk08AC5F28;
typedef struct S08AC5F28 { char pad[0x138]; Blk08AC5F28 *blk; } S08AC5F28;
extern int D_089D69E8;
extern void func_088D3A04(Blk08AC5F28 *b);
extern void func_0887BA5C(int h);
extern void func_0887B1F4(int h, int v);
extern void func_08885220(int v);
extern void func_088F0A78(int v);
void func_08AC5F28(S08AC5F28 *self)
{
    int i;
    func_088D3A04(self->blk);
    for (i = 0; i < 27; i++) {
        if (self->blk->h884[i] != 0) {
            func_0887BA5C(self->blk->h884[i]);
            self->blk->h884[i] = 0;
        }
    }
    if (self->blk->h880 != 0) {
        func_0887B1F4(self->blk->h880, 3);
        self->blk->h880 = 0;
    }
    func_08885220(D_089D69E8);
    func_088F0A78(0);
    self->blk->wA04 = 3;
}
