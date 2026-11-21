/* Closes the window: its parts, the message box, the layout and its
 * eighteen texts; then state 4. */
typedef struct Work4A974 {
    char pad_000[0xC];
    char layout[0x884];       /* +0x00C */
    void *texts[0x12];        /* +0x890 */
    char pad_8D8[0x1A4];
    int state;                /* +0xA7C */
} Work4A974;

typedef struct Screen4A974 {
    char pad_000[0x14C];
    Work4A974 *work;          /* +0x14C */
} Screen4A974;

extern void *D_089D69E8;
void func_08A4AB80(Screen4A974 *self);
void func_08885220(void *box);
void func_088D3A04(void *layout);
void func_0887BA5C(void *text);

void func_08A4A974(Screen4A974 *self)
{
    int i;

    func_08A4AB80(self);
    func_08885220(D_089D69E8);
    func_088D3A04(self->work->layout);
    for (i = 0; i < 0x12; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    self->work->state = 4;
}
