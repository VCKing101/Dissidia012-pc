typedef struct FloatSlot {
    unsigned char active;
    float value;
} FloatSlot;

extern int func_08AB3C4C(FloatSlot *self);

/* The slot's value, 0 when func_08AB3C4C rejects it. */
float func_08AB3C54(FloatSlot *self)
{
    if (func_08AB3C4C(self) == 0) {
        return 0.0f;
    }
    return self->value;
}
