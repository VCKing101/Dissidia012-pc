struct Message088A0DBC {
    void *type;
    int a;
    int b;
    int c;
    unsigned char flag;

    Message088A0DBC(void *message_type, int a0, int b0, int c0, unsigned char flag0)
        : type(message_type), a(a0), b(b0), c(c0), flag(flag0) {
    }
};

extern "C" {
extern unsigned char D_089CABF8[];
void func_088A0EEC(void *target, Message088A0DBC *message);

void func_088A0DBC(void *target, int a, int b, int c, unsigned char flag) {
    Message088A0DBC message(D_089CABF8, a, b, c, flag);
    func_088A0EEC(target, &message);
}
}
