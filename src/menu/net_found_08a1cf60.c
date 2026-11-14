/* MenuRoot: after a network search (flag 0x1000 of +0x8E60), finds the
   first used save whose session entry (+0x8E70, 0x1270 bytes each) the
   network object accepts, keeps its place (+0x25B58) and the found
   position (+0x8E20 to +0x25B60), then marks the search done (flag 4,
   flags 0x1238 cleared), clears the request's busy byte and moves to
   step 5. Standalone: these fields are not in the region header. */
typedef struct NetPos29 {
    short x;
    short y;
} NetPos29;

typedef struct NetFound29 {
    unsigned short x;
    short y;
} NetFound29;

typedef struct NetSave29 {
    unsigned short flags;
    char pad_02[0x24A];
} NetSave29;

typedef struct NetSession29 {
    char data[0x1270];
} NetSession29;

typedef struct NetObject29 {
    char pad_00000[0x31968];
    int count; /* +0x31968 */
} NetObject29;

typedef struct NetWork29 {
    char pad_00000[0x2BC0];
    NetSave29 saves[26];       /* +0x2BC0 */
    char pad_06778[0x1EC];
    NetObject29 *net;          /* +0x6964 */
    int step;                  /* +0x6968 */
    char pad_0696C[0x127C];
    char search[0x1238];       /* +0x7BE8 */
    NetFound29 found;          /* +0x8E20 */
    char pad_08E24[0x3C];
    unsigned int flags;        /* +0x8E60 */
    char pad_08E64[0xC];
    NetSession29 sessions[24]; /* +0x8E70 */
    char pad_248F0[0x1268];
    int chosen;                /* +0x25B58 */
    char pad_25B5C[0x4];
    NetFound29 shown;          /* +0x25B60 */
} NetWork29;

typedef struct NetRoot29 {
    char pad_000[0x154];
    NetWork29 *work;
} NetRoot29;

typedef struct NetRequest29 {
    char pad_00[0x18];
    unsigned char busy;
} NetRequest29;

extern NetRequest29 D_09B6D840;
extern void func_08A1CD14(NetRoot29 *self);
extern int func_088F5E80(char *search, NetSession29 *session);

void func_08A1CF60(NetRoot29 *self)
{
    int count;
    int i;

    func_08A1CD14(self);
    if (self->work->flags & 0x1000) {
        count = self->work->net->count;
        for (i = 0; i < count; i++) {
            if (self->work->saves[i].flags & 1) {
                if (func_088F5E80(self->work->search, &self->work->sessions[i]) != 0) {
                    self->work->chosen = i;
                    self->work->shown = self->work->found;
                    break;
                }
            }
        }
        self->work->flags |= 4;
        self->work->flags &= ~0x1238;
        D_09B6D840.busy = 0;
        self->work->step = 5;
    }
}
