/* Reset a sprite to its default state: range, scale 1.0, flags cleared. */
typedef struct Sprite0887B6A0 {
    unsigned char unk_00[6];
    unsigned char a;
    unsigned char b;
    short c;
    short d;
    unsigned int unk_0C;
    unsigned int flags;
} Sprite0887B6A0;
extern void func_0887B748(Sprite0887B6A0 *self, int a, int b);
extern void func_0887B7D0(Sprite0887B6A0 *self, float x, float y);
extern void func_0887B7DC(Sprite0887B6A0 *self, float x, float y);
extern void func_0887B924(Sprite0887B6A0 *self);
extern void func_0887B970(Sprite0887B6A0 *self);
extern void func_0887B9E4(Sprite0887B6A0 *self);
extern void func_0887BBC8(Sprite0887B6A0 *self);
void func_0887B6A0(Sprite0887B6A0 *self)
{
    float one;
    func_0887B748(self, 0, 0);
    one = 1.0f;
    func_0887B7D0(self, one, one);
    func_0887B7DC(self, one, one);
    self->a = 0;
    self->b = 0;
    self->c = 0;
    self->flags = 0;
    func_0887B924(self);
    func_0887B970(self);
    func_0887B9E4(self);
    self->flags &= 0xFFFFF7FF;
    self->unk_0C = 0;
    self->d = -1;
    func_0887BBC8(self);
}
