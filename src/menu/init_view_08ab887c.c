/* Sets up the view: draws the screen's texts into the block's text area (+0x400), sets the title's halfword (+0xE of the object at +0x46C) to 4, recreates the handle at +0x95C, binds the second text, enables flag 1 at +0x880 and sets the counters. */
typedef struct Title08AB887C { char pad[0xE]; short h; } Title08AB887C;
typedef struct Blk08AB887C {
    char pad0[0x400];
    char text[0x6C];
    Title08AB887C *title;
    char pad1[0x880 - 0x470];
    int w880;
    char pad2[0x8FC - 0x884];
    int h8FC;
    char pad3[0x900 - 0x900];
    char at900[0x5C];
    int h95C;
    char pad4[0xD30 - 0x960];
    int wD30;
    char pad5[4];
    int wD38;
    char pad6[0xD56 - 0xD3C];
    unsigned char bD56;
} Blk08AB887C;
typedef struct S08AB887C { char pad[0x138]; Blk08AB887C *blk; } S08AB887C;
extern const char D_08ABB832[];
extern const char D_08ABB84E[];
extern const char D_08ABB886[];
extern void func_088D3B88(Blk08AB887C *b, char *text, const char *name, int a, int n);
extern void func_088D3CF0(Blk08AB887C *b, char *text, const char *name);
extern void func_0887BA5C(int h);
extern int func_0887AD78(int h, int a, int b, int c, int d, int e);
extern void func_088D428C(char *text, char *at, const char *name, int n);
extern void func_088D46E4(int *flags, int a, int b, int c, int d, int e);
extern void func_08AB89A8(S08AB887C *self);
void func_08AB887C(S08AB887C *self)
{
    char *text = self->blk->text;
    Blk08AB887C *b;
    int h;
    func_088D3B88(self->blk, text, D_08ABB832, 0, 0x120);
    func_088D3CF0(self->blk, text, D_08ABB84E);
    self->blk->title->h = 4;
    b = self->blk;
    h = b->h95C;
    if (h != 0) {
        func_0887BA5C(h);
        self->blk->h95C = 0;
        b = self->blk;
    }
    self->blk->h95C = func_0887AD78(b->h8FC, 0, 6, 0, 0, 0);
    func_088D428C(text, self->blk->at900, D_08ABB886, 1);
    func_088D46E4(&self->blk->w880, 1, 0, 1, 0, -1);
    {
        int *fl = &self->blk->w880;
        *fl |= 1;
        func_08AB89A8(self);
    }
    self->blk->wD38 = 0;
    self->blk->bD56 = 0;
    self->blk->wD30 |= 0xA;
}
