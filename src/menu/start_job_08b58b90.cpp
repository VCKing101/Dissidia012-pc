// Looks up the resource for the object's index, starts a job with a callback, and flags the object if it started.
struct Inner { unsigned flags; int index; };
struct Owner { Inner *inner; };
struct Res { char pad[0xEE0]; int h; };
extern Res *D_089D712C;
extern int D_089D727C;
extern unsigned short D_08B59200[];
extern "C" {
extern int func_0887AFBC(int, int, int);
extern int func_088B1230(int, int, void *, int, int, int);
extern void func_08B58F64(void);
void func_08B58B90(Owner *self) {
    int h = func_0887AFBC(D_089D712C->h, D_08B59200[self->inner->index], 0);
    if (func_088B1230(D_089D727C, h, (void *)func_08B58F64, 0, 0xA3, 0)) {
        self->inner->flags |= 1;
    }
}
}
