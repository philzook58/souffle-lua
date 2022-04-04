#include "souffle/SouffleFunctor.h"
#include <cassert>
#include <cstdint>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


#if RAM_DOMAIN_SIZE == 64
using FF_int = int64_t;
using FF_uint = uint64_t;
using FF_float = double;
#else
using FF_int = int32_t;
using FF_uint = uint32_t;
using FF_float = float;
#endif


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
        lua_getglobal(L, f);           /* function to be called */
        lua_pushnumber(L, (double)n);  /* push 1st argument */
        lua_pushnumber(L, (double)n2); /* push 1st argument */
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

    souffle::RamDomain luacall_F_S(souffle::SymbolTable *symbolTable, souffle::RecordTable *recordTable,
                                   souffle::RamDomain f, souffle::RamDomain arg1) // RamFloat? RamTypes.h
    {
        assert(symbolTable && "NULL symbol table");
        assert(recordTable && "NULL record table");
        const std::string &f2 = symbolTable->decode(f);
        lua_getglobal(L, f2.c_str());    /* function to be called */
        lua_pushnumber(L, (double)arg1); /* push 1st argument */

        if (lua_pcall(L, 1, 1, 0) != LUA_OK)
            error(L, "error running function '%s': %s", f2,
                  lua_tostring(L, -1));
        int isnum;
        const char *z = lua_tostring(L, -1);
        // std::string s = z;
        // if (!isnum)
        //     error(L, "function '%s' should return a string", f);
        souffle::RamDomain retval = symbolTable->encode(z);
        lua_pop(L, 1); /* pop returned value */
        return retval;
    }
}