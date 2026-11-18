//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_dct_load.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : discrete_cosine_transform related codes (load)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_dct.hpp"


//*** FUNCTION *****************************************************************
// loadPipe
void JPG_DCT::loadPipe()
{
    // basic
    JPG_UNIT::loadPipe();

    // other module
    if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::DEC) {
        m_h_d->cpyCoeMcu(*m_datCoeMcu);
    }
}

// loadFile
//void JPG_DCT::loadFile()
//{
//    // basic
//    JPG_UNIT::loadFile();
//
//    //...
//}

// loadRand
//void JPG_DCT::loadRand()
//{
//    // basic
//    JPG_UNIT::loadRand();
//
//    //...
//}
