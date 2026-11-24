/* In mode 5, follows the player's pause selection (menu 0x10) and then,
   unless the mode is ending, closes the pause menu and resumes. */
typedef struct ModeState {
    char pad_00[0x48];
    int state;
    int kind;
    char pad_50[0x58 - 0x50];
    int menu;
    int saved_menu;
} ModeState;

typedef struct PauseInfo {
    char pad_0000[0x2004];
    int a;
    int b;
} PauseInfo;

typedef struct PlayerInfo {
    char pad_0000[0x10E8];
    unsigned int flags;
    char pad_10EC[0x16E4 - 0x10EC];
    unsigned char single;
} PlayerInfo;

typedef struct ResultMenu {
    char pad_00[8];
    int open;
} ResultMenu;

typedef struct ModeTask {
    char pad_00[0x44];
    int a;
    int b;
} ModeTask;

extern ModeState *D_089D6A10;
extern PauseInfo *D_089D69A8;
extern PlayerInfo *D_089D8364;
extern int D_08B13B48;
extern ResultMenu *D_08B0CF20;

extern void func_08889944(ModeState *mode, int menu);
extern int func_08B133E4(int menu, int first);
extern void func_089072AC(PlayerInfo *player, int flag);
extern void func_08B13314(int menu);
extern void func_0888A7E0(void);

void func_08889138(ModeTask *self)
{
    ModeState *mode = D_089D6A10;

    if (mode->kind == 5) {
        PauseInfo *pause = D_089D69A8;
        PlayerInfo *player = D_089D8364;
        int state;
        if (player != 0) {
            if (player->flags & 1) {
                if (player->single == 1) {
                    if (mode->menu == 0x10) {
                        func_08889944(mode, mode->saved_menu);
                        pause->a = 1;
                        pause->b = 1;
                        self->a = 1;
                        self->b = 1;
                    }
                } else if (mode->menu != 0x10) {
                    mode->saved_menu = mode->menu;
                    func_08889944(mode, 0x10);
                    pause->a = 2;
                    pause->b = 2;
                    self->a = 2;
                    self->b = 2;
                }
            } else if (mode->menu == 0x10) {
                func_08889944(mode, mode->saved_menu);
                pause->a = 1;
                pause->b = 1;
                self->a = 1;
                self->b = 1;
            }
        }
        state = mode->state;
        if (state != 8 && state != 5 && state != 7) {
            int last = state == 3;
            if (func_08B133E4(D_08B13B48, last) == 0) {
                func_089072AC(D_089D8364, 1);
                func_08B13314(D_08B13B48);
                if ((unsigned char)(D_08B0CF20->open != 0)) {
                    mode->state = 5;
                } else {
                    func_0888A7E0();
                }
            }
        }
    }
}
