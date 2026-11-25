/* Hides the three handles at +0x894, shows the one for the index and poses it and the matching one at +0x888 with the two pose pairs from D_08A88A4A. */
typedef struct Blk08A88478 {
    char pad0[0x408];
    int h408[0x120];
    int h888[3];
    int h894[3];
} Blk08A88478;
typedef struct S08A88478 { Blk08A88478 *blk; } S08A88478;
extern unsigned short D_08A88A4A[];
extern void func_0887B944(int h);
extern void func_0887B924(int h);
extern void func_088DA0A8(int h, int pose, float *matrix, int *color);
extern void func_0887B778(int h, float *matrix);
extern void func_0887B828(int h, int *color);
void func_08A88478(S08A88478 *self, int idx)
{
    float matrix[16];
    int color;
    int i;
    for (i = 0; i < 3; i++) {
        func_0887B944(self->blk->h894[i]);
    }
    func_0887B924(self->blk->h894[idx]);
    func_088DA0A8(self->blk->h408[idx], D_08A88A4A[0], matrix, &color);
    func_0887B778(self->blk->h888[idx], matrix);
    func_0887B828(self->blk->h888[idx], &color);
    func_088DA0A8(self->blk->h408[idx], D_08A88A4A[1], matrix, &color);
    func_0887B778(self->blk->h894[idx], matrix);
    func_0887B828(self->blk->h894[idx], &color);
}
