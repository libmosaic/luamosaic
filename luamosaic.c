/** @file luamosaic.c
 */

// lua
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
// mosaic
#include <mosaic.h>
#include <stream_io.h>

#define MOSAIC_METATABLE "Mosaic.mosaic"

#define checkmosaic(L) \
	(Mosaic *) luaL_checkudate ((L), 1, MOSAIC_METATABLE)

/// The functions to be registered
const struct luaL_Reg mosaiclib [] = {
	{NULL, NULL}
};

int luaopen_mosaic (lua_State *L) {
	luaL_newmetatable (L, MOSAIC_METATABLE);
	luaL_getmetatable (L, MOSAIC_METATABLE);
	luaL_setfuncs (L, mosaiclib, 0);
	return 1;
}
