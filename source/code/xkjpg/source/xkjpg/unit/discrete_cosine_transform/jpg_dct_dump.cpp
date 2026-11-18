//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_dct_dump.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : discrete_cosine_transform related codes (dump)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_dct.hpp"


//*** FUNCTION *****************************************************************
// dumpFile
void JPG_DCT::dumpFile()
{
    // basic
    JPG_UNIT::dumpFile();

    //...
    if ((jpg_enmDump_t)m_cfg->dctEnmDump >= jpg_enmDump_t::INOUT) {
        for (int c = 0; c< JPG_NUMB_CHN; ++c) {
            dumpDatRecMcu(c);
        }
    }
}

// dumpDatRecMcu
void JPG_DCT::dumpDatRecMcu(
int idxChn)
{
    int    datShf    = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
    string strObj    = "datRecMcu";
    string strIdx[3] = {"Y", "U", "V"};
    string strTyp    = "S0I8F0";
    string strPst    = "dat";
    int    datMsk    = 0xff;
    string strFmt    = "%02x";
    int    siz       = (m_cfg->sizMcu >> datShf);
    int    idxDlt    = 1;
    JPG_FUNC_DUMP_D_2(m_dumpDatRecMcuFlg[idxChn]
        ,             m_dumpDatRecMcuFpt[idxChn]
        ,             strObj, strIdx[idxChn], strTyp, strPst
        ,             (*m_datRecMcu)[idxChn]
        ,             datMsk, strFmt
        ,             siz, idxDlt
        ,             siz, idxDlt
    );
}