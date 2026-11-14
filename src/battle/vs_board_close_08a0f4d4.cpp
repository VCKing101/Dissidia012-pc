/* Releases the versus board's models once its frame is hidden, and turns
 * off the pad light of each side still playing. */
struct BoundModel {
    unsigned char pad_00[0x20];
    unsigned short flags;

    static bool hidden(BoundModel *model) { return (model->flags & 0x20) != 0; }
};

struct VsMarks {
    void *marks[4];
};

struct VsBoardModels {
    unsigned char open;
    unsigned char pad_01[0xB];
    BoundModel *frame;
    void *glow;
    VsMarks sides[2];
    unsigned char pad_34[0xD8];
    int turn;
    int pad_index;
    unsigned char pad_114[8];
    unsigned char done[2];
};

extern "C" {

extern void func_088DA060(void *model);
extern void func_08889F70(int side, int pad);

int func_08A0F4D4(VsBoardModels *board)
{
    if (board->frame == 0 || BoundModel::hidden(board->frame) == 1) {
        for (int side = 0; side < 2; side++) {
            for (int mark = 0; mark < 4; mark++) {
                if (board->sides[side].marks[mark] != 0) {
                    func_088DA060(board->sides[side].marks[mark]);
                    board->sides[side].marks[mark] = 0;
                }
            }
        }
        if (board->glow != 0) {
            func_088DA060(board->glow);
            board->glow = 0;
        }
        if (board->frame != 0) {
            func_088DA060(board->frame);
            board->frame = 0;
        }
        for (int side = 0; side < 2; side++) {
            if (board->done[side] == 1) {
                continue;
            }
            func_08889F70((board->turn + side) % 2, board->pad_index + 5);
        }
    }
    return 0;
}

}
