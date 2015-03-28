/** @file luamosaic.h
 */

#pragma once

// lua
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
// mosaic
#include <mosaic.h>
#include <color.h>
#include <stream_io.h>

#define MOSAIC_METATABLE "mosaic"

typedef MOSAIC * MOSAICptr;

#define checkmosaic(L) \
	(Mosaic *) luaL_checkudate ((L), 1, MOSAIC_METATABLE)

static MOSAICptr checkMOSAIC (lua_State *L, int index);
static MOSAICptr * pushMOSAIC (lua_State *L, MOSAICptr mos);
