//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_unit_dump.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : unit related codes (dump)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_unit.hpp"


//*** FUNCTION *****************************************************************
// dumpFile
void JPG_UNIT::dumpFile()
{
    // dumpDatOriMcu
    if (   ((jpg_enmDump_t)m_cfg->dctEnmDump >= jpg_enmDump_t::INOUT && m_strTag == "JPG_DCT")
    ) {
        for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn)
            dumpDatOriMcu(idxChn);
    }

    // dumpDatCoeMcu
    if (   ((jpg_enmDump_t)m_cfg->dctEnmDump >= jpg_enmDump_t::INOUT && m_strTag == "JPG_DCT")
    ) {
        for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn)
            dumpDatCoeMcu(idxChn);
    }
}

// dumpDatOriMcu
void JPG_UNIT::dumpDatOriMcu(
int idxChn)
{
    int    datShf    = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
    string strObj    = "datOriMcu";
    string strIdx[3] = {"Y", "U", "V"};
    string strTyp    = "S0I8F0";
    string strPst    = "dat";
    int    datMsk    = 0xff;
    string strFmt    = "%02x";
    int    siz       = (m_cfg->sizMcu >> datShf);
    int    idxDlt    = 1;
    JPG_FUNC_DUMP_D_2(m_dumpDatOriMcuFlg[idxChn]
        ,             m_dumpDatOriMcuFpt[idxChn]
        ,             strObj, strIdx[idxChn], strTyp, strPst
        ,             (*m_datOriMcu)[idxChn]
        ,             datMsk, strFmt
        ,             siz, idxDlt
        ,             siz, idxDlt
    );
}

// dumpDatCoeMcu
void JPG_UNIT::dumpDatCoeMcu(
int idxChn)
{
    int    datShf    = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
    string strObj    = "datCoeMcu";
    string strIdx[3] = {"Y", "U", "V"};
    string strTyp    = "S0I8F0";
    string strPst    = "dat";
    int    datMsk    = 0xff;
    string strFmt    = "%02x";
    int    siz       = (m_cfg->sizMcu >> datShf);
    int    idxDlt    = 1;
    JPG_FUNC_DUMP_D_2(m_dumpDatCoeMcuFlg[idxChn]
        ,             m_dumpDatCoeMcuFpt[idxChn]
        ,             strObj, strIdx[idxChn], strTyp, strPst
        ,             (*m_datCoeMcu)[idxChn]
        ,             datMsk, strFmt
        ,             siz, idxDlt
        ,             siz, idxDlt
    );
}