/* Resets an animation player and starts the given animation. */
typedef struct AnimPlayer {
    int field_00;
    int field_04;
    int anim;                   /* 0x08 */
} AnimPlayer;

extern void func_08921C7C(AnimPlayer *player);
extern void func_08921248(AnimPlayer *player, int anim);

void func_08920A3C(AnimPlayer *player, int anim)
{
    func_08921C7C(player);
    player->anim = anim;
    func_08921248(player, anim);
}
