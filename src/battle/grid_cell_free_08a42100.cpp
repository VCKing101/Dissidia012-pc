struct GridRow42100 {
    int width;
    char pad_004[0x10];
    int cells[12];
};

struct Grid42100 {
    char pad_000[0x4];
    int index;
    char pad_008[0x4];
    int column;
    int row;
    GridRow42100 rows[4];
};

struct GridFighter42100 {
    char pad_000[0xD60];
    signed char side;
};

struct GridOwner42100 {
    GridFighter42100 *fighter;
    char pad_004[0x10];
    Grid42100 *grid;
};

extern "C" {

bool func_08A42100(GridOwner42100 *self)
{
    Grid42100 *grid = self->grid;
    int index = grid->index;
    int cell = grid->row * grid->rows[index].width + grid->column;
    if (self->fighter->side == 1) {
        if (index == 3 && grid->rows[index].cells[cell] == 0) {
            return true;
        }
    } else {
        if (index == 2 && grid->rows[index].cells[cell] == 0) {
            return true;
        }
    }
    return false;
}

}
