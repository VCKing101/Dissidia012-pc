/* Resets the lobby timer and peer address. */
typedef struct Lobby {
    char pad_000[0x14];
    char timer[0x28 - 0x14];
    int interval;
    char pad_02C[0x250 - 0x2C];
    unsigned long long peer;
    int peer_id;
    char pad_25C;
    unsigned char joined;
} Lobby;

extern unsigned long long D_089CDCD8;
extern void func_089055C0(void *timer, int ms, int interval);

void func_08908D74(Lobby *lobby)
{
    func_089055C0(lobby->timer, 1000, lobby->interval);
    lobby->peer = D_089CDCD8;
    lobby->peer_id = -1;
    lobby->joined = 0;
}
