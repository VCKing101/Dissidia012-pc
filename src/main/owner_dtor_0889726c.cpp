extern "C" void func_08992F04(void *p);

struct State0889726C {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    unsigned char field_14;
    unsigned char unknown_15[3];
    int field_18;

    static void reset(State0889726C *state) {
        if (state != 0) {
            state->field_14 = 0;
            state->field_00 = 0;
            state->field_04 = 0;
            state->field_08 = 0;
            state->field_0C = 0;
            state->field_10 = 0;
            state->field_18 = 0;
        }
    }
};

struct Owner0889726C {
    unsigned char unknown_00[0xAC];
    State0889726C state;

    static void release(Owner0889726C *self, int flags) {
        State0889726C::reset(&self->state);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
};

extern "C" {
void func_0889726C(Owner0889726C *self, int flags) {
    if (self != 0) {
        Owner0889726C::release(self, flags);
    }
}
}
