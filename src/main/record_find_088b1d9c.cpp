/* Binary-searches the global record table at 0x089D7294 for an id; 0 for id -1. */
struct Record088B1D9C {
    short id;
    short fields[19];
};

struct Info088B1D9C {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088B1D9C {
    signed char descending;

    Order088B1D9C() : descending(0) {}
};

extern "C" {

extern Info088B1D9C *D_089D7290;
extern Record088B1D9C *D_089D7294;

Record088B1D9C *func_088B1C14(Record088B1D9C *begin, Record088B1D9C *end, const Record088B1D9C *key, Order088B1D9C order);

Record088B1D9C *func_088B1D9C(int id)
{
    Record088B1D9C *result = 0;
    if (id != -1) {
        Record088B1D9C key;
        key.id = id;
        result = func_088B1C14(D_089D7294, D_089D7294 + D_089D7290->count, &key, Order088B1D9C());
    }
    return result;
}

}
