/* Binary-searches the catalog's fourth list for an id. */
#include "main/agent17_types.h"

extern "C" {

CatalogByte2 *func_088B7354(CatalogByte2 *begin, CatalogByte2 *end, const CatalogByte2 *key, SearchOrder order);

void func_088B9084(Catalog *catalog, int id)
{
    CatalogByte2 key;
    key.id = id;
    func_088B7354(catalog->fourth, catalog->fourth + catalog->fourth_count, &key, SearchOrder());
}

}
