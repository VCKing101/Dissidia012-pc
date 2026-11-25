/* For each of four entries gets the pose colour of the block's part, halves its alpha unless the entry's flag byte is set (then uses -1), and applies it to the entry's handle. */
typedef struct Blk08AE6948 {
    char pad0[0x48C];
    int part;
    char pad1[0x880 - 0x490];
    int h[0x80];
} Blk08AE6948;
typedef struct S08AE6948 { char pad[0x10000]; } S08AE6948;
extern unsigned short D_08AEBD6C[];
extern unsigned short D_08AEBD74[];
extern void func_088DA0A8(int part, int pose, float *matrix, unsigned char *color);
extern void func_0887B8A8(int h, int alpha);
#define BLK08AE6948(s) (*(Blk08AE6948 **)((char *)(s) + 0x10000 - 0x5970))
#define FLAG08AE6948(s, i) (*((unsigned char *)(s) + 0x10000 - 0x5978 + (i)))
void func_08AE6948(S08AE6948 *self)
{
    float matrix[16];
    unsigned char color[4];
    int i;
    for (i = 0; i < 4; i++) {
        int alpha;
        Blk08AE6948 *b;
        func_088DA0A8(BLK08AE6948(self)->part, D_08AEBD6C[i], matrix, color);
        b = BLK08AE6948(self);
        alpha = -1;
        if (FLAG08AE6948(b, i) == 0) {
            alpha = color[3] = color[3] >> 1;
        }
        func_0887B8A8(b->h[D_08AEBD74[i]], alpha);
    }
}
