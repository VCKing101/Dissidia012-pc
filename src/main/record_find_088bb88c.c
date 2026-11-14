/* Finds the record with the id in the global sorted table; 0 when absent. */
typedef struct Key088BB88C {
    unsigned short id;
    char pad_02[14];
} Key088BB88C;

typedef struct Record088BB88C {
    unsigned short id;
    char pad_02[14];
} Record088BB88C;

typedef struct Info088BB88C {
    char pad_00[8];
    unsigned short count;
} Info088BB88C;

extern Info088BB88C *D_089D7388;
extern Record088BB88C *D_089D738C;

Key088BB88C *func_088BB760(Key088BB88C *key, int id);
Record088BB88C *func_088BB900(Record088BB88C *begin, Record088BB88C *end, Key088BB88C *key);

Record088BB88C *func_088BB88C(int id)
{
    Key088BB88C key;
    Record088BB88C *found;
    func_088BB760(&key, id);
    found = func_088BB900(D_089D738C, D_089D738C + D_089D7388->count, &key);
    if (found == D_089D738C + D_089D7388->count) {
        found = 0;
    }
    return found;
}
