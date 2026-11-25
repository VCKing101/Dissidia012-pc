/* Polls the selection: a changed index swaps the two entries (0x5C bytes each) and flags 1; keys A/B set 2 when flag 0x298 allows, key C sets 3 (with a cue) when flag 0x299 allows. */
struct Child089316E0 { char d[0x34]; virtual int v0(unsigned long long k); virtual int v1(unsigned long long k); };
struct Obj089316E0 {
    char pad0[0x264]; Child089316E0 *child; int h268; int sel;
    char pad1[0x294 - 0x270]; int flag; unsigned char f298; unsigned char f299;
};
extern "C" {
extern unsigned long long D_089CF1E8;
extern unsigned long long D_089CF1F0;
extern unsigned long long D_089CF1F8;
void func_088D688C(int *h, int a);
void func_08930700(void *p);
void func_089306E4(void *p);
void func_088D4BF4(int a);
void func_089316E0(Obj089316E0 *self)
{
    int old = self->sel;
    self->flag = 0;
    func_088D688C(&self->h268, 0);
    if (old != self->sel) {
        func_08930700((char *)self + old * 0x5C + 0x38);
        func_089306E4((char *)self + self->sel * 0x5C + 0x38);
        self->flag = 1;
    } else if (self->child->v1(D_089CF1E8) != 0 || self->child->v1(D_089CF1F0) != 0) {
        if (self->f298 != 0) {
            self->flag = 2;
        }
    } else if (self->child->v1(D_089CF1F8) != 0) {
        if (self->f299 != 0) {
            func_088D4BF4(1);
            self->flag = 3;
        }
    }
}
}
