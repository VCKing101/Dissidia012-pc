typedef struct VoiceSource {
    char pad000[0x5A0];
    void *bank;         /* 0x5A0 */
} VoiceSource;

typedef struct VoiceChannel {
    int field_00;
    int state;          /* 0x04 */
    int field_08;
    VoiceSource *source;    /* 0x0C */
    int field_10;
    int field_14;
    char pad18[0x14];
    void *player;       /* 0x2C */
} VoiceChannel;

extern void func_088DE6D4(void *player, void *bank, int id, int one);

int func_08B1E220(VoiceChannel *channel, int id, int value)
{
    if (channel->source == 0) {
        return -1;
    }
    if (channel->source->bank == 0) {
        return -2;
    }
    func_088DE6D4(channel->player, channel->source->bank, id, 1);
    channel->state = 6;
    channel->field_14 = value;
    return 0;
}
