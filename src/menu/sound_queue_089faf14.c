/* Queues a (sound, value) pair in the four-entry ring and flags the queue (0x20). */
typedef struct QueuedSound {
    unsigned short field_00;
    short field_02;
} QueuedSound;

typedef struct QueueState {
    char pad_0000[0x3280];
    unsigned int field_3280;
    char pad_3284[0x3418 - 0x3284];
    int field_3418;
    char pad_341C[0x4];
    int field_3420;
    char pad_3424[0x10];
    QueuedSound field_3434[4];
} QueueState;

typedef struct QueueOwner {
    char pad_000[0x14C];
    QueueState *field_14C;
} QueueOwner;

void func_089FAF14(QueueOwner *self, int sound, short value)
{
    if (self->field_14C->field_3418 < 4) {
        self->field_14C->field_3434[self->field_14C->field_3420].field_00 = sound;
        self->field_14C->field_3434[self->field_14C->field_3420].field_02 = value;
        self->field_14C->field_3420++;
        if (self->field_14C->field_3420 >= 4) {
            self->field_14C->field_3420 = 0;
        }
        self->field_14C->field_3418++;
        self->field_14C->field_3280 |= 0x20;
    }
}
