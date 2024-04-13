#pragma once
#include  <Engine\singleton.h>

class CLobbyMgr :
    public CSingleton<CLobbyMgr>
{
    SINGLE(CLobbyMgr);

public:
    static void begin();
    static void tick();
};

