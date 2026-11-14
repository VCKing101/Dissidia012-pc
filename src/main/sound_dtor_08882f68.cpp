struct Sound08882F68 {
    unsigned char unknown_00[0x50];
    int buffer;
    unsigned char unknown_54[0x10];
    int voice;
};

extern "C" {
extern int D_089D69CC;
void func_0885C538(int heap, int buffer, int flags);
void func_089C5E48(int voice);
void func_088831C4(Sound08882F68 *self);
}

struct Release08882F68 {
    static void destroy(Sound08882F68 *self, int flags) {
        if (self != 0) {
            if (self->buffer != 0) {
                func_0885C538(D_089D69CC, self->buffer, 0);
                self->buffer = 0;
            }
            if (self->voice != -1) {
                func_089C5E48(self->voice);
                self->voice = -1;
            }
            if (flags & 1) {
                func_088831C4(self);
            }
        }
    }
};

extern "C" {
void func_08882F68(Sound08882F68 *self, int flags) {
    Release08882F68::destroy(self, flags);
}
}
