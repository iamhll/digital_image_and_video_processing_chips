//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_unit_proc.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : unit related codes (proc)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_unit.hpp"


//*** FUNCTION *****************************************************************
// procMain
void JPG_UNIT::procMain()
{
    procPrev();
    procCore();
    procPost();
}

// procPrev
void JPG_UNIT::procPrev()
{
    //...
}

// procCore
void JPG_UNIT::procCore()
{
    // this function must be overridden
    assert(0);
}

// procPost
void JPG_UNIT::procPost()
{
    //...
}
