/*
  Written in 1-2 hours in a Voice-chat, don't expect good code.
  Author: Kiko
  P.S: Hi :3
*/
namespace CUtil
{
    struct Character
    {
        std::string Name;
        int x, y;
        bool onScreen;
        int Health;
    };

    Character Fetch(uintptr_t L, std::string Name)
    {
        Character C;
        /* Cache stack size */
        int fSS = r_lua_gettop(L);
        /* GetService */
        r_lua_getfield(L, LUA_GLOBALSINDEX, "game");
        r_lua_getfield(L, -1, "GetService");
        r_lua_pushvalue(L, -2);
        r_lua_pushstring(L, "Players");
        r_lua_call(L, 2, 1);
        /* Get Player */
        r_lua_getfield(L, -1, Name.c_str()); // We could've loop but why waste lines.

        switch (r_lua_type(L, -1))
        {
        case R_LUA_TNIL: /* Couldn't find user. */
        {
            exit(420);
            break;
        }
        case R_LUA_TUSERDATA:
        {
            r_lua_getfield(L, -1, "Character");
            /* Used to crash without this check! */
            if (r_lua_type(L, -1) == R_LUA_TNIL)
            {
                break;
            }
            r_lua_getfield(L, -1, "HumanoidRootPart");
            r_lua_getfield(L, -1, "Position"); /* Vector3 */

            r_lua_getfield(L, LUA_GLOBALSINDEX, "workspace");
            r_lua_getfield(L, -1, "CurrentCamera");
            r_lua_getfield(L, -1, "WorldToScreenPoint");
            r_lua_pushvalue(L, -2);
            r_lua_pushvalue(L, -5); /* Position */

            r_lua_call(L, 2, 2);

            C.Name = Name;
            C.onScreen = r_lua_toboolean(L, -1);
            /* Push and get position */
            r_lua_pushvalue(L, -2);
            /* X axis */
            r_lua_getfield(L, -1, "X");
            C.x = r_lua_tointeger(L, -1, NULL);
            r_lua_pop(L, 1);
            /* Y axis */
            r_lua_getfield(L, -1, "Y");
            C.y = r_lua_tointeger(L, -1, NULL);
            r_lua_pop(L, 1);

            /* Couldn't bother to re-use the one on the stack */
            r_lua_getfield(L, LUA_GLOBALSINDEX, "game");
            r_lua_getfield(L, -1, "GetService");
            r_lua_pushvalue(L, -2);
            r_lua_pushstring(L, "Players");
            r_lua_call(L, 2, 1);
            /* Get Player */
            r_lua_getfield(L, -1, Name.c_str());

            r_lua_getfield(L, -1, "Character");
            r_lua_getfield(L, -1, "Humanoid");
            r_lua_getfield(L, -1, "Health");
            C.Health = r_lua_tointeger(L, -1, NULL);
            break;
        }
        default: break;
        }
        /* We don't want extra stuff on the stack */
        r_lua_pop(L, r_lua_gettop(L) - fSS);
        return C;
    }
}

/*
Usage:
  CUtil::Character MyC = CUtil::Fetch(L, "Name Here");
*/
