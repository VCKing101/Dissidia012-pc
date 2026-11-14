typedef struct ChargeSlot {
    char pad_00[0x4];
    void *body;
} ChargeSlot;

typedef struct ChargeTask {
    char pad_000[0xC8];
    float charge;
    char pad_0CC[0x94];
    int id;
} ChargeTask;

extern ChargeSlot *func_08AB3264(int id);
extern int func_08AB3274(int id);
extern int func_08A82638(int unit, int skill);
extern int func_08AB327C(int id);
extern float func_08AB13FC(int unit);
extern float func_08838300(void *body);

/* Charges up while the unit has skill 0x17 (up to its limit), otherwise empties the charge; -1 no body. */
int func_08A9C5B0(ChargeTask *self)
{
    void *body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -1;
    }
    if (func_08A82638(func_08AB3274(self->id), 0x17) == 1) {
        float limit = func_08AB13FC(func_08AB327C(self->id));
        float charge = self->charge + func_08838300(body);
        self->charge = charge;
        if (charge > limit) {
            self->charge = limit;
        }
    } else {
        self->charge = 0.0f;
    }
    return 0;
}
