struct Scene0889740C {
    unsigned char unknown_000[0x4D0];
    int link;
};

struct Owner0889740C {
    unsigned char unknown_000[0x104];
    unsigned char state;
};

extern "C" Scene0889740C *D_089D6AC8;

struct Link0889740C {
    static bool none() {
        if (D_089D6AC8->link == -1) {
            return true;
        }
        return false;
    }
};

extern "C" {
bool func_0889740C(Owner0889740C *self) {
    if (self->state == 1) {
        return false;
    }
    if (!Link0889740C::none()) {
        return true;
    }
    return false;
}
}
