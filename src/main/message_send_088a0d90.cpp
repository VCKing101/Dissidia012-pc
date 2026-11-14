struct Message088A0D90 {
    void *type;
    int value;

    Message088A0D90(void *message_type, int message_value) : type(message_type), value(message_value) {
    }
};

extern "C" {
extern unsigned char D_089CABE0[];
void func_088A0EEC(void *target, Message088A0D90 *message);

void func_088A0D90(void *target, int value) {
    Message088A0D90 message(D_089CABE0, value);
    func_088A0EEC(target, &message);
}
}
