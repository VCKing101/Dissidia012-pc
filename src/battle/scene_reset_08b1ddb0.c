// Resets the scene: restores the handles in globals 089D6A10/089D6AE8/089D6C88, zeroes +4/+8/+0xC of the owner and returns 0.
extern void func_08889944(int, int);
extern void func_088A28E8(int, int);
extern void func_088A3F2C(int);
extern void func_088965F4(int, int);
extern void func_088A18B0(int, int, int);
extern int D_089D6A10;
extern int D_089D6AE8;
extern int D_089D6C88;
extern float D_089C9F54;
typedef struct Owner57 {
    int f0;
    int f4;
    int f8;
    int fC;
} Owner57;
int func_08B1DDB0(Owner57 *self)
{
    func_08889944(D_089D6A10, self->f8);
    func_088A28E8(0, 0);
    func_088A3F2C(0);
    func_088965F4(D_089D6AE8, 0);
    func_088A18B0(D_089D6C88, 100, (int)D_089C9F54);
    self->f8 = 0;
    self->fC = 0;
    self->f4 = 0;
    return 0;
}
