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

/// Mosaic's metatable
#define MOSAIC_METATABLE "mosaic"

/// Mosaic pointer, used by various functions
typedef MOSAIC * MOSAICptr;

#define checkmosaic(L) \
	(Mosaic *) luaL_checkudate ((L), 1, MOSAIC_METATABLE)
/// Auxiliary function for getting a Mosaic, checking it's type/metatable
static MOSAICptr checkMOSAIC (lua_State *L, int index);
/// Auxiliary function for pushing a Mosaic to the Lua Stack (as light userdata)
static MOSAICptr * pushMOSAIC (lua_State *L, MOSAICptr mos);
