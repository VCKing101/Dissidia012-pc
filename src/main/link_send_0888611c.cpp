struct Link0888611C {
    int handle;
    void *target;
};

extern "C" {
extern int D_089D7830;
void func_08885E2C(Link0888611C *self, int a, int b);
void func_088D0984(int system, int handle, unsigned char kind, unsigned short value);
}

struct Sound0888611C {
    static void send(int handle, unsigned char kind, unsigned short value) {
        func_088D0984(D_089D7830, handle, kind, value);
    }
};

extern "C" {
void func_0888611C(Link0888611C *self, unsigned char kind, unsigned short value) {
    if (self->handle != 0) {
        if (self->target == 0) {
            func_08885E2C(self, 0, 0);
        }
        Sound0888611C::send(self->handle, kind, value);
    }
}
}
