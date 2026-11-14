/* Menu step: when flag 0x10 is set, runs the dialog at +0x894 first and stops
   while it is still open; then, once func_08A9B1B8 allows it, leaves the step
   with state 0x1f.
   (Not a stack-spill function.) */
typedef struct Data08A9914C {
    char unknown_0000[0x910];
    int state;
    int sub_state;
    char unknown_0918[0x7740];
    unsigned char busy;
    char unknown_8059[0x15B];
    unsigned flags;
} Data08A9914C;

typedef struct Menu08A9914C {
    char unknown_000[0x154];
    Data08A9914C *data;
    char unknown_158[0x73C];
    char dialog[4];
} Menu08A9914C;

int func_08AB6E14(void *dialog);
int func_08AB6E38(void *dialog);
int func_08AB5E58(void *dialog);
void func_08AB6E5C(void *dialog);
void func_08AB6D20(void *dialog, int mode);
int func_08A9B1B8(Menu08A9914C *menu);
void func_08A9B238(Menu08A9914C *menu);
void func_08A9D080(Menu08A9914C *menu);

void func_08A9914C(Menu08A9914C *self)
{
    if ((self->data->flags & 0x10) != 0) {
        void *dialog = self->dialog;
        if (func_08AB6E14(dialog) == 1) {
            if (func_08AB6E38(dialog) == 0) {
                return;
            }
            self->data->busy = 0;
            self->data->sub_state = 0x30;
        } else {
            if (func_08AB5E58(dialog) == 0) {
                return;
            }
            func_08AB6E5C(dialog);
            func_08AB6D20(dialog, 1);
        }
    }
    if (func_08A9B1B8(self) == 0) {
        return;
    }
    func_08A9B238(self);
    func_08A9D080(self);
    self->data->state = 0x1f;
}
