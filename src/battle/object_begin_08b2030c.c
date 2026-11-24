/* Begins with a start index; in mode 1 also binds the side's +0x5A0 source. -1 bad index, -2 no object, -3 no source. */
typedef struct BObj { char pad[0x5A0]; void *src; } BObj;
typedef struct BRoot { char pad[0x38]; BObj *obj[2]; } BRoot;
typedef struct BSelf { char pad[4]; int state; void *src; char pad_0c[4]; int side; } BSelf;
extern BRoot *D_089D6A10;
extern void *func_08872B48(int side);
extern int func_08B20260(BSelf *self, void *p);
extern void func_08B20160(BSelf *self, int index);
int func_08B2030C(BSelf *self, int index, unsigned char mode)
{
    int one;
    if (index < 0) {
        return -1;
    }
    one = 1;
    if (mode == one) {
        BObj *o = D_089D6A10->obj[self->side];
        if (o == 0) {
            return -2;
        }
        self->src = o->src;
        if (self->src == 0) {
            return -3;
        }
        func_08B20260(self, func_08872B48(self->side));
    }
    func_08B20160(self, index);
    self->state = one;
    return 0;
}
