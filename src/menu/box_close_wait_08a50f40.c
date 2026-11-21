/* Once the message box has answered and the fonts are idle, closes the box
 * and goes to state 0xD. */
typedef struct Work50F40 {
    int field_00;
    int state;                /* +0x04 */
} Work50F40;

typedef struct Screen50F40 {
    char pad_00[0x58];
    Work50F40 *work;          /* +0x58 */
} Screen50F40;

extern void *D_089D69E8;
extern void *D_089D6998;
int func_08885608(void *box, int arg);
int func_0887E718(void *object);
void func_08885220(void *box);

void func_08A50F40(Screen50F40 *self)
{
    void *box = D_089D69E8;
    int none = 0;

    if (func_08885608(box, none) == 0) {
        return;
    }
    if (func_0887E718(D_089D6998) == 0) {
        func_08885220(D_089D69E8);
        self->work->state = 0xD;
    }
}
