struct Agent4FlagBytes {
    unsigned char unknown_0000[0x190];
    unsigned char flags[8];

    static bool long_range(Agent4FlagBytes *self) {
        unsigned char *flags = self->flags;
        return flags[6] & 1;
    }
};

extern "C" {
float func_08838E24(Agent4FlagBytes *self) {
    if (Agent4FlagBytes::long_range(self)) {
        return 1000.0f;
    } else {
        return 100.0f;
    }
}
}
