/* Sends message 11 (a short, two bytes and a short) unless func_08AB6DAC
   reports the menu busy. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct PoseMessage08AB4C30 {
    short field_0;
    signed char field_2;
    signed char field_3;
    short field_4;
};

struct Menu08AB4C30;

extern "C" {
int func_08AB6DAC(Menu08AB4C30 *menu);
void *func_08AB6D58(Menu08AB4C30 *menu);
void func_08A70838(PoseMessage08AB4C30 *message);
void func_08A6FC24(void *target, int kind, void *data, int size);

void func_08AB4C30(Menu08AB4C30 *self, int part, int first, int second, int third)
{
    PoseMessage08AB4C30 message;
    Kept<Menu08AB4C30 *> saved(self);
    int p = part;
    int busy = func_08AB6DAC(self);
    self = saved.value;
    if (busy == 1) {
        return;
    }
    void *target = func_08AB6D58(self);
    func_08A70838(&message);
    message.field_0 = first;
    message.field_2 = second;
    message.field_3 = third;
    message.field_4 = p;
    func_08A6FC24(target, 0xB, &message, 6);
}
}
