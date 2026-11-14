/* Binary-searches the global record table at 0x089D73E8 for an id; 0 for id -1. */
struct Record088BFEEC {
    short id;
    short fields[3];
};

struct Info088BFEEC {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088BFEEC {
    signed char descending;

    Order088BFEEC() : descending(0) {}
};

extern "C" {

extern Info088BFEEC *D_089D73E0;
extern Record088BFEEC *D_089D73E8;

Record088BFEEC *func_088BFBEC(Record088BFEEC *begin, Record088BFEEC *end, const Record088BFEEC *key, Order088BFEEC order);

Record088BFEEC *func_088BFEEC(int id)
{
    Record088BFEEC *result = 0;
    if (id != -1) {
        Record088BFEEC key;
        key.id = id;
        result = func_088BFBEC(D_089D73E8, D_089D73E8 + D_089D73E0->count, &key, Order088BFEEC());
    }
    return result;
}

}
