/* Initialises the 35 200-byte entries and the trailing entry at 7000. */
struct Entry088B3004 {
    char data[200];
};

struct Table088B3004 {
    Entry088B3004 entries[35];
    Entry088B3004 total;
};

extern "C" {

void func_088B2ED0(Entry088B3004 *entry);

void func_088B3004(Table088B3004 *self)
{
    for (int i = 0; i < 35; i++) {
        func_088B2ED0(&self->entries[i]);
    }
    func_088B2ED0(&self->total);
}

}
