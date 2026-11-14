/* Resets an animation player and restarts it, switching animation unless
   -1 is given. */
typedef struct AnimPlayer {
    int field_00;
    int field_04;
    int anim;                   /* 0x08 */
} AnimPlayer;

extern void func_08921C7C(AnimPlayer *player);
extern void func_08921248(AnimPlayer *player, int anim);

void func_08920A7C(AnimPlayer *player, int anim)
{
    func_08921C7C(player);
    if (anim != -1) {
        player->anim = anim;
    }
    func_08921248(player, player->anim);
}
