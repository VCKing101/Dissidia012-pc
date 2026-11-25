/* Menu confirm step: looks up the selected entry and, for result 1 or 2, starts the transition and sets the page state; result 2 plays sound 4 when it fails. */
typedef struct S { char p[4]; short f4; } S;
typedef struct A { int state; char p[0x20]; int *h; } A;
typedef struct B { char p[0x24]; int h24; } B;
typedef struct O { char p[0xC4]; int *c4; B *c8; } O;
extern S D_09B6DAC0;
extern int func_08B45200(int);
extern int func_08B47FE4(int, int);
extern void func_08B4E794(int);
extern void func_08B46698(int, int);
extern void func_08B3A3D8(O *, int);
extern int func_08B3A9F4(O *, int, int);
extern void func_088D4BF4(int);
void func_08B36C40(O *self) {
    int id = D_09B6DAC0.f4;
    int obj = func_08B45200(id);
    switch (func_08B47FE4(((int *)self->c4)[4], obj)) {
    case 0:
        break;
    case 1:
        func_08B4E794(self->c8->h24);
        func_08B46698(((int *)self->c4)[4], 1);
        func_08B3A3D8(self, id);
        *self->c4 = 0xE;
        break;
    case 2:
        if (func_08B3A9F4(self, obj, -1) != 0) {
            func_08B4E794(self->c8->h24);
            *self->c4 = 0x12;
        } else {
            func_088D4BF4(4);
            break;
        }
        break;
    case 3:
        break;
    }
}
