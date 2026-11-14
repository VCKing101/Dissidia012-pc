typedef struct World { char data_0000[0xBDE8]; unsigned int flags; } World;
typedef struct Game { char data_000[0x4C]; int state; char data_050[0x7C8 - 0x50]; World *world; } Game;
extern unsigned long long D_089CA0B8;
extern void *func_088720A4(void);
extern void func_08872304(void *masks, unsigned long long bits);
extern void func_088C96E8(World *world, int arg);

void func_08889BAC(Game *game, int arg, unsigned char mark)
{
    func_08872304(func_088720A4(), D_089CA0B8);
    func_088C96E8(game->world, arg);
    if (mark) {
        game->world->flags |= 2;
    }
    game->state = 9;
}
