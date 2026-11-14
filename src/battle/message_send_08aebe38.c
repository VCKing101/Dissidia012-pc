typedef struct BattleMessage {
    int value;
    int kind;
    char pad_08[0x14];
    unsigned char flag_1C;
    char pad_1D[0x7];
    unsigned char flag_24;
    char pad_25[0xB];
} BattleMessage;

typedef struct MessageUnit {
    char pad_0000[0x10CC];
    short id;
} MessageUnit;

typedef struct MessageOwner {
    MessageUnit *unit;
} MessageOwner;

extern void func_089EB798(BattleMessage *msg);
extern void func_0888ACE4(BattleMessage *msg);
extern void func_0888A274(void);
extern void func_089EB7D0(BattleMessage *msg, int flags);

void func_08AEBE38(void *self, MessageOwner *owner)
{
    BattleMessage msg;
    func_089EB798(&msg);
    msg.value = owner->unit->id;
    msg.kind = 0x4A;
    msg.flag_1C = 1;
    msg.flag_24 = 1;
    func_0888ACE4(&msg);
    func_0888A274();
    func_089EB7D0(&msg, 2);
}
