/* After the guard call, runs the handler for the object's state 0..3; -1 when the guard fails. */
typedef struct DSelf { char pad[4]; int state; } DSelf;
extern int func_08B20118(void);
extern void func_08B20458(DSelf *self, unsigned char flag);
extern void func_08B205C4(DSelf *self, unsigned char flag);
extern void func_08B20834(DSelf *self, unsigned char flag);
extern void func_08B205BC(DSelf *self);
int func_08B20050(DSelf *self, unsigned char flag)
{
    if (func_08B20118() == 0) {
        return -1;
    }
    switch (self->state) {
    case 0:
        func_08B20458(self, flag);
        break;
    case 1:
        func_08B205C4(self, flag);
        break;
    case 2:
        func_08B20834(self, flag);
        break;
    case 3:
        func_08B205BC(self);
        break;
    }
    return 0;
}
