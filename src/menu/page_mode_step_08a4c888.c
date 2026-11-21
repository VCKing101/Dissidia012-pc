/* Steps the page once it is ready: by mode, 2..3 run func_08A4D824 and 4
 * runs func_08A4E72C, both going on to state 2. */
typedef struct Work4C888 {
    char pad_0000[0x4];
    int state;                /* +0x0004 */
    char pad_0008[0x6D30];
    int mode;                 /* +0x6D38 */
} Work4C888;

typedef struct Screen4C888 {
    char pad_00[0x5C];
    Work4C888 *work;          /* +0x5C */
} Screen4C888;

int func_08A4D350(Screen4C888 *self);
int func_08A4D370(Screen4C888 *self);
void func_08A4CFC4(Screen4C888 *self);
void func_08A4D824(Screen4C888 *self, int arg);
void func_08A4E72C(Screen4C888 *self);

void func_08A4C888(Screen4C888 *self)
{
    if (func_08A4D350(self) != 0) {
        if (func_08A4D370(self) == 3) {
            func_08A4CFC4(self);
        }
        switch (self->work->mode) {
        case 2:
        case 3:
            func_08A4D824(self, 1);
            self->work->state = 2;
            break;
        case 4:
            func_08A4E72C(self);
            self->work->state = 2;
            break;
        }
    }
}
