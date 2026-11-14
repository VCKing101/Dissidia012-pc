/* Adds the icon set that matches the current player's character type. */
typedef struct IconHolder {
    unsigned char open;
} IconHolder;

typedef struct BattleGame {
    unsigned char pad_000[0x5C4];
    void *player;
} BattleGame;

extern BattleGame *func_08889E44(void);
extern void *func_08A642BC(void *player);
extern unsigned int func_08A7EC18(void *character);
extern void func_089E0F24(IconHolder *self, void *icons, int count);
extern char D_089E1074[];
extern char D_089E10B4[];
extern char D_089E10F4[];
extern char D_089E1134[];
extern char D_089E1174[];

int func_089E0DE0(IconHolder *self)
{
    BattleGame *game;
    void *player;
    unsigned int kind;

    if (!self->open) {
        return -1;
    }
    game = func_08889E44();
    if (game == 0) {
        return -2;
    }
    player = game->player;
    if (player == 0) {
        return -3;
    }
    kind = func_08A7EC18(func_08A642BC(player));
    if (kind >= 7) {
        return 0;
    }
    func_089E0F24(self, D_089E1074, 2);
    if (kind == 0 || kind == 1) {
        func_089E0F24(self, D_089E10B4, 2);
    }
    if (kind == 0 || kind == 2) {
        func_089E0F24(self, D_089E10F4, 2);
    }
    if (kind == 2) {
        func_089E0F24(self, D_089E1134, 2);
    }
    if (kind == 1) {
        func_089E0F24(self, D_089E1174, 2);
    }
    return 0;
}
