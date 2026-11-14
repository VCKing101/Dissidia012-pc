struct Agent4MotionState {
    unsigned char unknown_0000[0x180];
    short motion;
};

struct Agent4Kind {
    int kind;

    static bool is_special(Agent4Kind *kind) {
        int value = kind->kind;
        return value == 0xC || value == 0xD || value == 0xE || value == 0x13 || value == 0x19;
    }
};

struct Agent4KindHolder {
    unsigned char unknown_0000[0x3A4];
    Agent4Kind *kind;
};

extern "C" {
bool func_08838160(Agent4MotionState *self) {
    return (self->motion >= 900) & (self->motion < 0x390);
}

bool func_08838178(Agent4KindHolder *self) {
    Agent4Kind *kind = self->kind;
    if (kind == 0) {
        return false;
    }
    return Agent4Kind::is_special(kind);
}
}
