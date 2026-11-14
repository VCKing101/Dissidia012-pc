/* Binary-searches the catalog's third list for an id. */
#include "main/agent17_types.h"

extern "C" {

CatalogByte4 *func_088B72AC(CatalogByte4 *begin, CatalogByte4 *end, const CatalogByte4 *key, SearchOrder order);

void func_088B9044(Catalog *catalog, int id)
{
    CatalogByte4 key;
    key.id = id;
    func_088B72AC(catalog->third, catalog->third + catalog->third_count, &key, SearchOrder());
}

}
