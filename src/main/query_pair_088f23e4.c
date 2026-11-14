/* Runs a lookup query for `key` and copies its 8-byte result to `out` when given. */
typedef struct QueryResult {
    int first;
    int second;
} QueryResult;

typedef struct Query {
    int head;
    QueryResult result;
    char work[0x180 - 0xC];
} Query;

extern void func_088C1C0C(Query *query);
extern void func_088C1CD4(Query *query, QueryResult *result, int key, int mode);

void func_088F23E4(QueryResult *out, int key)
{
    Query query;

    func_088C1C0C(&query);
    func_088C1CD4(&query, &query.result, key, 1);
    if (out != 0) {
        *out = query.result;
    }
}
