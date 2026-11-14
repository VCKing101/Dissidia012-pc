struct Link088864E0 {
    int handle;
};

extern "C" {
extern int D_089D7830;
void func_088D0984(int system, int handle, unsigned char kind, unsigned short value);
}

struct Sound088864E0 {
    static void send(int handle, unsigned char kind, unsigned short value) {
        func_088D0984(D_089D7830, handle, kind, value);
    }
};

extern "C" {
void func_088864E0(Link088864E0 *self, unsigned char kind, unsigned short value) {
    if (self->handle != 0) {
        Sound088864E0::send(self->handle, kind, value);
    }
}
}
