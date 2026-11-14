/* Loads the badge set's frame and four badge models for the side, sets the
 * pad light and refreshes the set. */
struct BadgeSet {
    unsigned char open;
    unsigned char pad_01[3];
    void *frame;
    void *badges[4];
    int side;
    int pad_index;
    void *badge_source;
    void *source;
    int frame_name;
};

extern "C" {

extern unsigned int func_08A22CA8(int side);
extern void *func_088DAA40(void *source, int name, int flags);
extern void *func_088DAA68(void *source);
extern void func_08889F28(int side, int pad);
extern void func_08A008FC(BadgeSet *set);

int func_08A00ADC(BadgeSet *set)
{
    if (!set->open) {
        return -1;
    }
    if (func_08A22CA8(set->side) >= 2) {
        return -2;
    }
    if (set->frame == 0) {
        set->frame = func_088DAA40(set->source, set->frame_name, 0);
        if (set->frame == 0) {
            return -3;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (set->badges[i] == 0) {
            set->badges[i] = func_088DAA68(set->badge_source);
            if (set->badges[i] == 0) {
                return -4;
            }
        }
    }
    func_08889F28(set->side, set->pad_index + 5);
    func_08A008FC(set);
    return 0;
}

}
