# souffle-lua

It's pretty straightforward to make clunky lua bindings. The delta of utility on this over just using C++ is somewhat small.
With a bit more polish it could be more useful though.

Currently it just looks for a file called `souffle.lua`, loads it up.

- Not safe for use in parallelism. Probably the simplest approach is to make a thread safe pool of LuaStates and just give them out as need be.
Writing stateful lua functions is likely to cause pain anyhow.
- It would be nice to give access, possibly pleasant access to the record table and symboltable from within lua.
- LuaJIT might be desirable for speed.



 souffle::RamDomain luacall_S_S(souffle::SymbolTable* symbolTable, souffle::RecordTable* recordTable,
        souffle::RamDomain f, souffle::RamDomain arg1) {
    assert(symbolTable && "NULL symbol table");
    assert(recordTable && "NULL record table");
    const std::string& f = symbolTable->decode(f);
    lua_getglobal(L, f.c_str()); /* function to be called */
    lua_pushstring(L, arg1); /* push 1st argument */
    /* do the call (2 arguments, 1 result) */
    if (lua_pcall(L, 1, 1, 0) != LUA_OK)
        error(L, "error running function 'f': %s",
        lua_tostring(L, -1));
    z = lua_tonumberx(L, -1, &isnum);
    if (!isnum)
        error(L, "function 'f' should return a number");
    lua_pop(L, 1); /* pop returned value */
    return z;
    //return symbolTable->encode(result);


    Huh. the standard library doesn't have a pool datastructure?