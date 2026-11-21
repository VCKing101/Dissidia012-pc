/* Opens the confirmation box (text 0x1A) unless one is open, starts the
 * page part and goes to state 3 or 6 by mode. */
typedef struct Work50FD8 {
    int field_00;
    int state;                /* +0x0004 */
    char pad_0008[0x6B0C];
    void *field_6B14;
    char pad_6B18[0x4];
    void *field_6B1C;
    char pad_6B20[0xE80];
    int mode;                 /* +0x79A0 */
} Work50FD8;

typedef struct Screen50FD8 {
    char pad_00[0x58];
    Work50FD8 *work;          /* +0x58 */
} Screen50FD8;

extern void *D_089D69E8;
extern char D_08A51CA8[];
int func_08885544(void *box);
void func_088850EC(void *box, int kind, void *name);
void func_08885340(void *box, int arg);
void func_088854C4(void *box, int kind, int id);
void func_08885388(void *box);
void func_088852C4(void *box, int a, int b);
void func_08A69714(void *object, void *arg, int c);

void func_08A50FD8(Screen50FD8 *self)
{
    if (func_08885544(D_089D69E8) == 0) {
        int kind = 0x1A;
        func_088850EC(D_089D69E8, kind, D_08A51CA8);
        {
            int none = 0;
            func_08885340(D_089D69E8, none);
        }
        {
            int id = 0x64B2;
            func_088854C4(D_089D69E8, kind, id);
        }
        func_08885388(D_089D69E8);
    } else {
        int one = 1;
        int none = 0;
        func_088852C4(D_089D69E8, one, none);
    }
    func_08A69714(self->work->field_6B1C, self->work->field_6B14, 8);
    switch (self->work->mode) {
    case 0:
        self->work->state = 3;
        break;
    case 1:
        self->work->state = 6;
        break;
    }
}
