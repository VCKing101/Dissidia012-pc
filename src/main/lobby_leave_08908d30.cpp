/* From state 2: clears the ready flag and moves to state 3. */
struct Lobby {
    char pad_000[0x23C];
    int state;
    char pad_240[0x25C - 0x240];
    unsigned char ready;
};

extern "C" {
extern void func_08908FDC(Lobby *lobby, int state);

bool func_08908D30(Lobby *lobby)
{
    if (lobby->state == 2) {
        lobby->ready = 0;
        func_08908FDC(lobby, 3);
        return true;
    }
    return false;
}
}
