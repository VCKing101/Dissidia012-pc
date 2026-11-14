/* Starts the turn banner moving from one side to the other over the given
 * time, cueing the side-change sound when the controlling fighter allows it. */
struct BannerFlags {
    unsigned char data[0x18];

    static unsigned char ready(BannerFlags *flags) { return (unsigned char)(flags->data[0x14] & 1); }
    static bool muted(BannerFlags *flags) { return (flags->data[0x14] & 0x20) != 0; }
};

struct TurnBanner {
    unsigned char open;
    unsigned char pad_01[3];
    int pad_index;
    int channel;
    BannerFlags *fighter;
    unsigned char track[0x20];
    unsigned char moving;
    unsigned char pad_31[3];
    float progress;
    int from;
    int to;
    float duration;
};

extern "C" {

extern void func_089F7E18(void *track);
extern void func_08889F28(int channel, int pad);
extern void func_08A23154(int cue);
extern void func_089F602C(TurnBanner *banner, float time);

int func_089F6208(TurnBanner *banner, int start, int end, float duration)
{
    if (!banner->open) {
        return -1;
    }
    func_089F7E18(banner->track);
    func_08889F28(banner->channel, banner->pad_index + 3);
    banner->from = start;
    banner->to = end;
    banner->duration = duration;
    banner->moving = 1;
    banner->progress = 0.0f;
    if (end != start) {
        if (BannerFlags::ready(banner->fighter) == 1 && !BannerFlags::muted(banner->fighter)) {
            func_08A23154(1);
        }
    }
    func_089F602C(banner, 0.0f);
    return 0;
}

}
