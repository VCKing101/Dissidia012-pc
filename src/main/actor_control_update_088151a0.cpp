/* Updates an actor's control state for its control mode: the input and
   camera parts, the command part, and whether the actor's body is moved by
   commands or by its own logic this frame. */
struct Pad088151A0 {
    char pad_00[0xA];
    short buttons;
};

struct Control088151A0 {
    unsigned int low : 3;
    unsigned int commands : 1;
    signed int mode : 4;
    unsigned int high : 24;
};

struct Actor088151A0 {
    char pad_000[0x380];
    unsigned int flags;
    char pad_384[0x394 - 0x384];
    void *body;
    char pad_398[0x560 - 0x398];
    Control088151A0 control;
    Pad088151A0 *pad;
    char pad_568[0x570 - 0x568];
    char input[0x5B0 - 0x570];
    char camera[0x5DC - 0x5B0];
    char command[0x5F4 - 0x5DC];
    void *view;
};

extern "C" {

void func_088223F8(void *input);
float func_08838300(Actor088151A0 *self);
void func_0881F1C0(void *view, float rate);
void func_08818070(void *camera);
void func_08821278(void *command);
int func_08815CE8(Actor088151A0 *self);
int func_088214B0(void *command);
void func_0886B230(void *body);
void func_0886B240(void *body);
void func_0886CA58(void *body);

int func_088151A0(Actor088151A0 *self)
{
    int mode = self->control.mode;
    if (mode != 3) {
        if (mode != 4) {
            func_088223F8(self->input);
        }
        if (self->view != 0 && mode == 4) {
            float rate = func_08838300(self);
            func_0881F1C0(self->view, rate);
            func_08818070(self->camera);
        }
    }
    if (self->control.commands) {
        if (!(self->pad->buttons & 1)) {
            if (mode == 1) {
                func_08821278(self->command);
            }
        } else if (mode != 3) {
            func_08821278(self->command);
        }
    }
    int done = func_08815CE8(self);
    if (mode == 1 && done == 1) {
        self->flags |= 1;
    }
    if (((mode == 4 && (self->pad->buttons & 1)) || (mode == 1 && done == 1))
        && !func_088214B0(self->command)) {
        func_0886B230(self->body);
    } else {
        func_0886B240(self->body);
        func_0886CA58(self->body);
    }
    return 0;
}

}
