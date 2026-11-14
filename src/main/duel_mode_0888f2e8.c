typedef struct Side { char data_00[4]; unsigned char set; unsigned char ready; char data_06[2]; } Side;
typedef struct Duel { int field_00; Side sides[2]; } Duel;
extern int D_09B491E8;
extern void func_088DAA40(int ctx, int mode, int flags);

void func_0888F2E8(Duel *duel, int side)
{
    int ctx = D_09B491E8;
    Side *sides = duel->sides;
    int mode = 4;
    if (sides[side].ready == 1) {
        mode = sides[1 - side].ready == 1 ? 4 : 3;
    } else if (sides[1 - side].ready == 1) {
        mode = 5;
    }
    func_088DAA40(ctx, mode, 0);
}
