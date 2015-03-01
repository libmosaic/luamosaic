/** @file luamosaic.c
 */

#include "luamosaic.h"

/* luamosaic.h stuff */
static MOSAICptr checkMOSAIC (lua_State *L, int index) {
	MOSAICptr *pointer, mos;
	pointer = (MOSAICptr *) luaL_checkudata (L, index, MOSAIC_METATABLE);
	mos = *pointer;
	if (!mos) {
		luaL_error (L, "null MOSAIC");
	}
	return mos;
}


static MOSAICptr * pushMOSAIC (lua_State *L, MOSAICptr mos) {
	MOSAICptr * pointer = (MOSAICptr *) lua_newuserdata (L, sizeof (MOSAICptr));
	*pointer = mos;
	luaL_getmetatable (L, MOSAIC_METATABLE);
	lua_setmetatable (L, -2);
	return pointer;
}

/* And now, methods we will register */
static int lNewMOSAIC (lua_State *L) {
	int height = luaL_checkint (L, 1);
	int width = luaL_checkint (L, 2);
	pushMOSAIC (L, NewMOSAIC (height, width));
	return 1;
}

static int lFreeMOSAIC (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	FreeMOSAIC (mos);
	return 0;
}

static int lmosSetCh (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int y = luaL_checkint (L, 2);
	int x = luaL_checkint (L, 3);
	char * new_char = luaL_checkstring (L, 4);
	int ret = mosSetCh (mos, y, x, new_char[0]);
	// all right, s return the char added
	if (ret) {
		lua_pushlstring (L, new_char, 1);
		return 1;
	}
	// out of bounds, push nil!
	else {
		lua_pushnil (L);
		lua_pushliteral (L, "Index out of bounds");
		return 2;
	}
}

static int lmosGetCh (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int y = luaL_checkint (L, 2);
	int x = luaL_checkint (L, 3);
	mos_char ret = mosGetCh (mos, y, x);
	// all right, return the char got
	if (ret) {
		lua_pushlstring (L, (char *) &ret, 1);
		return 1;
	}
	// out of bounds, push nil!
	else {
		lua_pushnil (L);
		lua_pushliteral (L, "Index out of bounds");
		return 2;
	}
}

static int lmosSetAttr (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int y = luaL_checkint (L, 2);
	int x = luaL_checkint (L, 3);
	mos_attr new_attr = luaL_checkint (L, 4);
	int ret = mosSetAttr (mos, y, x, new_attr);
	// all right, s return the char added
	if (ret) {
		lua_pushinteger (L, new_attr);
		return 1;
	}
	// out of bounds, push nil!
	else {
		lua_pushnil (L);
		lua_pushliteral (L, "Index out of bounds");
		return 2;
	}
}

static int lmosGetAttr (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int y = luaL_checkint (L, 2);
	int x = luaL_checkint (L, 3);
	mos_attr ret = mosGetAttr (mos, y, x);
	// all right, return the char got
	if (ret) {
		lua_pushinteger (L, ret);
		return 1;
	}
	// out of bounds, push nil!
	else {
		lua_pushnil (L);
		lua_pushliteral (L, "Index out of bounds");
		return 2;
	}
}

static int lResizeMOSAIC (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int height = luaL_checkint (L, 2);
	int width = luaL_checkint (L, 3);
	int ret = ResizeMOSAIC (mos, height, width);
	// no problems
	if (!ret) {
		lua_pushboolean (L, 1);
		return 1;
	}
	// allocation failed! Can be used as `assert (mos:Resize (y, x))`
	// return true, error_msg
	else {
		lua_pushnil (L);
		lua_pushliteral (L, "Resize allocation error!");
		return 2;
	}
}


/// The functions to be registered
const struct luaL_Reg mosaiclib [] = {
	{"New", lNewMOSAIC},
	{"GetCh", lmosGetCh},
	{"SetCh", lmosSetCh},
	{"GetAttr", lmosGetAttr},
	{"SetAttr", lmosSetAttr},
	{"Resize", lResizeMOSAIC},
	{"__gc", lFreeMOSAIC},
	{NULL, NULL}
};

int luaopen_mosaic (lua_State *L) {
	// create the module
	luaL_newmetatable (L, MOSAIC_METATABLE);
	luaL_setfuncs (L, mosaiclib, 0);
	// let "__index" point to the methods, so we can use them!
	// duplicate the table, as it'll be consumed by `setfield`
	lua_pushvalue (L, -1);
	lua_setfield (L, -2, "__index");
	// and let's put a nice name in our table
	lua_pushliteral (L, MOSAIC_METATABLE);
	lua_setfield (L, -2, "__metatable");
	return 1;
}
