/** @file luamosaic.c
 */

#include "luamosaic.h"
#include <string.h>

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
	int height = luaL_checkinteger (L, 1);
	int width = luaL_checkinteger (L, 2);
	pushMOSAIC (L, NewMOSAIC (height, width));
	return 1;
}

static int lSubMOSAIC (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int height = luaL_checkinteger (L, 2);
	int width = luaL_checkinteger (L, 3);
	int begin_y = luaL_checkinteger (L, 4);
	int begin_x = luaL_checkinteger (L, 5);

	MOSAICptr sub = SubMOSAIC (mos, height, width, begin_y - 1, begin_x - 1);

	if (sub) {
		pushMOSAIC (L, sub);
		return 1;
	}
	// error
	else {
		lua_pushnil (L);
		lua_pushliteral (L, "Error in getting SubMOSAIC");
		return 2;
	}
}

static int lFreeMOSAIC (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	FreeMOSAIC (mos);
	return 0;
}

static int lmosSetCh (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int y = luaL_checkinteger (L, 2);
	int x = luaL_checkinteger (L, 3);
	char * new_char = luaL_checkstring (L, 4);
	int ret = mosSetCh (mos, y - 1, x - 1, new_char[0]);
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
	int y = luaL_checkinteger (L, 2);
	int x = luaL_checkinteger (L, 3);
	mos_char ret = mosGetCh (mos, y - 1, x - 1);
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
	int y = luaL_checkinteger (L, 2);
	int x = luaL_checkinteger (L, 3);
	mos_attr new_attr = luaL_checkinteger (L, 4);
	int ret = mosSetAttr (mos, y - 1, x - 1, new_attr);
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
	int y = luaL_checkinteger (L, 2);
	int x = luaL_checkinteger (L, 3);
	mos_attr ret = mosGetAttr (mos, y - 1, x - 1);

	// pushes result even if out of bounds, as mosGetAttr
	// returns Normal in these cases
	lua_pushinteger (L, ret);
	return 1;
}

static int lmosGetWidth (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	lua_pushinteger (L, mos->width);
	return 1;
}

static int lmosGetHeight (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	lua_pushinteger (L, mos->height);
	return 1;
}

static int lResizeMOSAIC (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int height = luaL_checkinteger (L, 2);
	int width = luaL_checkinteger (L, 3);
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

static int lOutOfBounds (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	int y = luaL_checkinteger (L, 2);
	int x = luaL_checkinteger (L, 3);
	lua_pushinteger (L, mosOutOfBoundaries (mos, y, x));
	return 1;
}

/* MOSAIC.COLOR! */
/// Register colors in table on top of the stack		[-0, +0, -]
void lRegisterColors (lua_State *L) {
	const char *color_names[] = {
		"Normal", "NBk", "NR", "NG", "NY", "NBl", "NM",	"NC", "NW",
		"BkN", "BkBk", "BkR", "BkG", "BkY", "BkBl", "BkM", "BkC", "BkW",
		"RN", "RBk", "RR", "RG", "RY", "RBl", "RM", "RC", "RW",
		"GN", "GBk", "GR", "GG", "GY", "GBl", "GM", "GC", "GW",
		"YN", "YBk", "YR", "YG", "YY", "YBl", "YM", "YC", "YW",
		"BlN", "BlBk", "BlR", "BlG", "BlY", "BlBl", "BlM", "BlC", "BlW",
		"MN", "MBk", "MR", "MG", "MY", "MBl", "MM", "MC", "MW",
		"CN", "CBk", "CR", "CG", "CY", "CBl", "CM", "CC", "CW",
		"WN", "WBk", "WR", "WG", "WY", "WBl", "WM", "WC", "WW"
	};

	int i;
	for (i = 0; i < MAX_COLORS; i++) {
		lua_pushinteger (L, i);
		lua_setfield (L, -2, color_names[i]);
	}
}

static int lTcolor (lua_State *L) {
	mos_attr color = luaL_checkinteger (L, 1);
	mos_attr bold = lua_isnoneornil (L, 2) ? 0 : BOLD;

	Tcolor (color | bold);
	
	return 0;
}

static int lGetFore (lua_State *L) {
	mos_attr color = luaL_checkinteger (L, 1);
	lua_pushinteger (L, GetFore (color));
	return 1;
}

static int lGetBack (lua_State *L) {
	mos_attr color = luaL_checkinteger (L, 1);
	lua_pushinteger (L, GetBack (color));
	return 1;
}

/* MOSAIC.IO */
static int lSaveMOSAIC (lua_State *L) {
	MOSAICptr mos = checkMOSAIC (L, 1);
	char * file_name = luaL_checkstring (L, 2);

	int ret = SaveMOSAIC (mos, file_name);

	if (!ret) {
		// no errors
		lua_pushboolean (L, 1);
		return 1;
	}
	else {
		lua_pushnil (L);
		lua_pushstring (L, strerror (ret));
		return 2;
	}
}

static int lLoadMOSAIC (lua_State *L) {
	char * file_name = luaL_checkstring (L, 1);

	MOSAIC * new = NewMOSAIC (0, 0);
	int ret = LoadMOSAIC (new, file_name);

	if (ret == 0 || ret == EUNKNSTRGFMT || ret == ENODIMENSIONS) {
		// no errors
		pushMOSAIC (L, new);
		return 1;
	}
	else {
		lua_pushnil (L);
		lua_pushstring (L, strerror (ret));
		return 2;
	}
}

/// The Mosaic functions to be registered
const struct luaL_Reg mosaiclib [] = {
	{"New", lNewMOSAIC},
	{"Sub", lSubMOSAIC},
	{"GetCh", lmosGetCh},
	{"SetCh", lmosSetCh},
	{"GetAttr", lmosGetAttr},
	{"SetAttr", lmosSetAttr},
	{"GetWidth", lmosGetWidth},
	{"GetHeight", lmosGetHeight},
	{"Resize", lResizeMOSAIC},
	{"OutOfBounds", lOutOfBounds},
	{"__gc", lFreeMOSAIC},
	{NULL, NULL}
};

/// The Color functions to be registered
const struct luaL_Reg colorlib [] = {
	{"Tcolor", lTcolor},
	{"GetFore", lGetFore},
	{"GetBack", lGetBack},
	{NULL, NULL}
};

/// The stream_io functions to be registered
const struct luaL_Reg stream_iolib [] = {
	{"Save", lSaveMOSAIC},
	{"Load", lLoadMOSAIC},
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

	// now, push the auxiliary modules
	// IO
	lua_newtable (L);
	luaL_setfuncs (L, stream_iolib, 0);
	lua_setfield (L, -2, "io");
	// COLOR
	lua_newtable (L);
	luaL_setfuncs (L, colorlib, 0);
	// register the colors from "enum color", each as it's name
	lRegisterColors (L);
	lua_setfield (L, -2, "color");

	return 1;
}
