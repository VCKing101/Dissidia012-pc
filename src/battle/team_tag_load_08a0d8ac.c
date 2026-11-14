/* Loads the team tag's model for the owner's side on first use. */
typedef struct TeamTag {
    unsigned char open;
    unsigned char pad_01[3];
    void *model;
    int owner;
    void *source;
} TeamTag;

extern unsigned int func_08A22CA8(int owner);
extern void *func_088DAA40(void *source, int name, int flags);
extern int D_08A0D958[2];

int func_08A0D8AC(TeamTag *tag)
{
    if (!tag->open) {
        return -1;
    }
    if (tag->model == 0) {
        unsigned int side = func_08A22CA8(tag->owner);
        if (side >= 2) {
            return -2;
        }
        tag->model = func_088DAA40(tag->source, D_08A0D958[side], 0);
        if (tag->model == 0) {
            return -3;
        }
    }
    return 0;
}
