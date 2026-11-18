//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_unit_load.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : unit related codes (load)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_unit.hpp"


//*** FUNCTION *****************************************************************
// loadPipe
void JPG_UNIT::loadPipe()
{
    // pixel (ori)
    if (m_cfg->enmModRun == (int)jpg_enmModRun_t::ENC) {
        for (int c = 0; c < m_cfg->numChn; ++c) {
            if (m_loadDatOriMcuFlg[c])
                m_fth->cpyDatOriMcu((*m_datOriMcu)[c], *m_cfg, c);
        }
    }
}

// loadFile
void JPG_UNIT::loadFile()
{
    //...
}

// loadRand
void JPG_UNIT::loadRand()
{
    //...
}
