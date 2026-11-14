/* Clears the total entry, then updates each of the 35 entries and adds it in. */
struct Entry088B3060 {
    char data[200];
};

struct Table088B3060 {
    Entry088B3060 entries[35];
    Entry088B3060 total;
};

extern "C" {

void func_088B2ED0(Entry088B3060 *entry);
void func_088B2FE8(Entry088B3060 *entry);
void func_088B2F80(Entry088B3060 *total, Entry088B3060 *entry);

void func_088B3060(Table088B3060 *self)
{
    func_088B2ED0(&self->total);
    for (int i = 0; i < 35; i++) {
        func_088B2FE8(&self->entries[i]);
        func_088B2F80(&self->total, &self->entries[i]);
    }
}

}
