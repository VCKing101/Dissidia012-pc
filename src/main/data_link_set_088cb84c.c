/* Stores the data pointer and marks it loaded (or null). */
typedef struct DataLink088CB84C {
    void *first;
    void *second;
} DataLink088CB84C;

void func_088CB84C(DataLink088CB84C *link, void *value)
{
    void *none = 0;
    link->first = value;
    link->second = value != 0 ? value : none;
}
