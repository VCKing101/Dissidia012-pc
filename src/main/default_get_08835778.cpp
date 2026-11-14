struct Agent4Defaulted {
    unsigned char unknown_0000[0x310];
    void *custom;

    static void *or_default(void *value, void *fallback) {
        if (value != 0) {
            return value;
        }
        return fallback;
    }
};

extern "C" {
extern int D_09AFE820;

void *func_08835778(Agent4Defaulted *self) {
    return Agent4Defaulted::or_default(self->custom, &D_09AFE820);
}

void *func_08835798(Agent4Defaulted *self) {
    return Agent4Defaulted::or_default(self->custom, &D_09AFE820);
}
}
