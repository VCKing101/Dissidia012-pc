/* Opens the room's lobby with the room parameters and links the listener record. */
typedef struct LobbyParams {
    int mode;
    int kind;
    int size;
    int user;
    unsigned char flag;
    char pad_11[3];
    int extra;
} LobbyParams;

typedef struct NetRoom {
    int owner;
    char lobby[0x10C - 4];
    void *listener;
    char pad_110[0x14C5C - 0x110];
    char listener_record[4];
} NetRoom;

extern void func_08903684(void *lobby, LobbyParams *params);

void func_08909758(NetRoom *room, int owner, int user, int extra)
{
    LobbyParams params;
    int kind = 2;

    room->owner = owner;
    params.mode = 0;
    params.kind = kind;
    params.flag = 0;
    params.size = 0x1258;
    params.user = user;
    params.extra = extra;
    func_08903684(room->lobby, &params);
    room->listener = room->listener_record;
}
