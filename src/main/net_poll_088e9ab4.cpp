struct Net {
    char data_0000[0x90];
    char session[8];
    void *peer;
    char data_009C[0x1710 - 0x9C];
    signed char rejoin[4];
    static bool active(Net *n) { return n->peer != 0; }
    static bool wantsRejoin(signed char *flag) { return *flag != 0; }
};
extern "C" {
extern Net *D_089D7E88;
int func_088E0F14(void *session);
void func_088E9474(Net *n);
void func_088E9500(Net *n);
void func_088E01C4(void *session, int flags);
void func_088E91E8(Net *n);

void func_088E9AB4(void)
{
    Net *n = D_089D7E88;
    if (Net::active(n)) {
        void *session = n->session;
        if (func_088E0F14(session) == 0) {
            func_088E9474(n);
            func_088E9500(n);
            func_088E01C4(session, 0);
            if (Net::wantsRejoin(n->rejoin)) {
                func_088E91E8(n);
            }
        }
    }
}
}
