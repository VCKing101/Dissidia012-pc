/* Updates an actor's input and camera parts for player control modes 1
   and 2, unless the actor is busy or its input is held. */
struct Control08815FAC {
    unsigned int enabled : 1;
    unsigned int low : 1;
    unsigned int held : 1;
    unsigned int commands : 1;
    signed int mode : 4;
    unsigned int high : 24;
};

struct Move08815FAC {
    char pad_00[0x28];
    float speed;
};

struct Actor08815FAC {
    char pad_000[0x560];
    Control08815FAC control;
    char pad_564[4];
    Move08815FAC *move;
    char pad_56C[4];
    char input[0x5B0 - 0x570];
    char camera[0x10];
};

extern "C" {

int func_08837990(Actor08815FAC *self);
void func_088226BC(void *input);
void func_08818070(void *camera);
float func_08838300(Actor08815FAC *self);
void func_08822458(void *input, float rate);
void func_08822800(void *input);

void func_08815FAC(Actor08815FAC *self)
{
    if (func_08837990(self) == 1) {
        return;
    }
    if (!self->control.held) {
        int mode = self->control.mode;
        if (mode == 2 || (mode == 1 && 0.0f < self->move->speed)) {
            func_088226BC(self->input);
            func_08818070(self->camera);
        } else if (mode == 1) {
            void *input = self->input;
            func_08822458(input, func_08838300(self));
            func_08822800(input);
            func_08818070(self->camera);
        }
    }
}

}
