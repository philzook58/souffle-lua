#include "souffle/SouffleFunctor.h"
#include <cassert>
#include <cstdint>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void error(lua_State *L, const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    exit(EXIT_FAILURE);
}

lua_State *init()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L); /* opens the standard libraries */
    if (luaL_loadfile(L, "souffle.lua") || lua_pcall(L, 0, 0, 0))
        error(L, "cannot run souffle.lua file: %s", lua_tostring(L, -1));
    return L;
}

lua_State *L = init();

extern "C"
{
    float luacall_F_F(char *f, float n)
    {
        lua_getglobal(L, f);  /* function to be called */
        lua_pushnumber(L, n); /* push 1st argument */
                              /* do the call (2 arguments, 1 result) */
        if (lua_pcall(L, 1, 1, 0) != LUA_OK)
            error(L, "error running function '%s': %s", f,
                  lua_tostring(L, -1));
        int isnum;
        double z = lua_tonumberx(L, -1, &isnum);
        if (!isnum)
            error(L, "function '%s' should return a number", f);
        lua_pop(L, 1); /* pop returned value */
        return (float)z;
    }
    float luacall_FF_F(char *f, float n, float n2)
    {
        lua_getglobal(L, f);  /* function to be called */
        lua_pushnumber(L, (double) n); /* push 1st argument */
        lua_pushnumber(L, (double) n2); /* push 1st argument */
                              /* do the call (2 arguments, 1 result) */
        if (lua_pcall(L, 2, 1, 0) != LUA_OK)
            error(L, "error running function '%s': %s", f,
                  lua_tostring(L, -1));
        int isnum;
        double z = lua_tonumberx(L, -1, &isnum);
        if (!isnum)
            error(L, "function '%s' should return a number", f);
        lua_pop(L, 1); /* pop returned value */
        return (float)z;
    }

    float luacall_FF_S(char *f, float n, float n2)
    {
        lua_getglobal(L, f);  /* function to be called */
        lua_pushnumber(L, (double) n); /* push 1st argument */
        lua_pushnumber(L, (double) n2); /* push 1st argument */
                              /* do the call (2 arguments, 1 result) */
        if (lua_pcall(L, 2, 1, 0) != LUA_OK)
            error(L, "error running function '%s': %s", f,
                  lua_tostring(L, -1));
        int isnum;
        double z = lua_tonumberx(L, -1, &isnum);
        if (!isnum)
            error(L, "function '%s' should return a number", f);
        lua_pop(L, 1); /* pop returned value */
        return (float)z;
    }
}