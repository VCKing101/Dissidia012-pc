/* Handles a ready prompt: left/right keys (choice 0 or 1, with a cue unless muted) and the confirm key (state 2) read from the input child. */
struct Child088DF15C { char d[0x34]; virtual int v0(unsigned long long k); virtual int v1(unsigned long long k); };
struct Obj088DF15C {
    char pad0[1]; unsigned char muted; char pad1[2]; int state; int confirm; int choice; Child088DF15C *child; unsigned char ready;
};
extern "C" {
extern unsigned long long D_089CC318;
extern unsigned long long D_089CC320;
extern unsigned long long D_089CC328;
int func_088DF2B8(Obj088DF15C *o);
void func_088D3508(int a);
int func_088DF15C(Obj088DF15C *self)
{
    if (func_088DF2B8(self) == 0) {
        return -1;
    }
    if (self->state == 1 && self->ready == 1) {
        if (self->child == 0) {
            return -2;
        }
        if (self->child->v1(D_089CC318) == 1) {
            self->choice = 0;
            if (self->muted == 0) {
                func_088D3508(0);
            }
        }
        if (self->child->v1(D_089CC320) == 1) {
            self->choice = 1;
            if (self->muted == 0) {
                func_088D3508(0);
            }
        }
        if (self->child->v1(D_089CC328) == 1) {
            self->confirm = 1;
            self->state = 2;
        }
    }
    return 0;
}
}
