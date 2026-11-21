typedef struct Seq0887BAD4 {
    unsigned char unk_00[0xC];
    short last_frame;                  /* +0x0C */
} Seq0887BAD4;

typedef struct Anim0887BAD4 {
    Seq0887BAD4 *seq;                  /* +0x00 */
    short frame;                       /* +0x04 */
    signed char wait;                  /* +0x06 ticks per frame, 0 = stopped */
    signed char count;                 /* +0x07 */
    unsigned char unk_08[8];
    unsigned int flags;                /* +0x10 bit 1, bit 13: paused */
} Anim0887BAD4;

extern void func_0887B7E8(Anim0887BAD4 *anim);   /* reached the end */

void func_0887BAD4(Anim0887BAD4 *anim)
{
    if (anim->wait != 0) {
        unsigned int flags = anim->flags;

        if (!(unsigned char)((flags & 2) != 0)) {
            if (!(unsigned char)((flags & 0x2000) != 0)) {
                anim->count++;
                if (anim->count >= anim->wait) {
                    anim->count = 0;
                    anim->frame++;
                    if (anim->seq->last_frame < anim->frame) {
                        func_0887B7E8(anim);
                    }
                }
            }
        }
    }
}
