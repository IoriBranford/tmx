#include "tmx.h"

uint32_t tmx_get_layer_gid(tmx_map *map, tmx_layer *layer, int x, int y) {
    if (layer->type != L_LAYER)
        return 0;
    if (x < 0 || y < 0 || x >= map->width || y >= map->height)
        return 0;
    int i = y * map->width + x;
    return layer->content.gids[i];
}

void tmx_set_layer_gid(tmx_map *map, tmx_layer *layer, int x, int y, uint32_t gid) {
    if (layer->type != L_LAYER)
        return;
    if (x < 0 || y < 0 || x >= map->width || y >= map->height)
        return;
    if (gid & TMX_FLIP_BITS_REMOVAL >= map->tilecount)
        return;
    int i = y * map->width + x;
    layer->content.gids[i] = gid;
}

tmx_tile* tmx_get_layer_tile(tmx_map *map, tmx_layer *layer, int x, int y, uint32_t *outGid) {
    uint32_t gid = tmx_get_layer_gid(map, layer, x, y);
    if (outGid)
        *outGid = gid;
    int i = gid & TMX_FLIP_BITS_REMOVAL;
    if (i >= map->tilecount)
        return NULL;
    return map->tiles[i];
}

void tmx_set_layer_tile(tmx_map *map, tmx_layer *layer, int x, int y, tmx_tile *tile, uint32_t flags) {
    uint32_t gid = 0;
    if (tile) {
        tmx_tileset *tileset = tile->tileset;
        unsigned int firstgid = 0;
        for (tmx_tileset_list *tslist = map->ts_head; tslist; tslist = tslist->next) {
            if (tileset == tslist->tileset) {
                firstgid = tslist->firstgid;
                break;
            }
        }
        gid = (firstgid + tile->id) | flags;
    }
    tmx_set_layer_gid(map, layer, x, y, gid);
}