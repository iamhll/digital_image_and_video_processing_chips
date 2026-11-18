//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_c_load.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_encoding related codes (load)
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2022-12-09 by HLL, LYZ
    //  Description    : fully tidied up
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_h_c.hpp"


//*** FUNCTION *****************************************************************
// loadPipe
void JPG_H_C::loadPipe()
{
    // basic
    // !!! loadPipe of JPG_UNIT is disabled
    //JPG_UNIT::loadPipe();

    // information (dct)
    m_dct->cpyCoeMcu(*m_datCoeMcu);
}
