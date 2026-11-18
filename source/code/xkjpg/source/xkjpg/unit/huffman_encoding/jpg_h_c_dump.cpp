//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_c_dump.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_encoding related codes (dump)
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
// dumpFile
void JPG_H_C::dumpFile()
{
    // basic
    JPG_UNIT::dumpFile();

    // dumpNumRbsFrame
    if (   (jpg_enmDump_t)m_cfg->enmDump    >= jpg_enmDump_t::INOUT
        || (jpg_enmDump_t)m_cfg->h_cEnmDump >= jpg_enmDump_t::INOUT
        || (jpg_enmDump_t)m_cfg->itfEnmDump >= jpg_enmDump_t::INOUT
    ) {
        if (   m_cfg->idxMcuX == m_cfg->numMcuX - 1
            && m_cfg->idxMcuY == m_cfg->numMcuY - 1
            && m_cfg->idxMcuR == m_cfg->numMcuR - 1
        ) {
            dumpNumRbsFrame();
        }
    }

    // dumpDatRbsFrame
    if (   (jpg_enmDump_t)m_cfg->enmDump    >= jpg_enmDump_t::INOUT
        || (jpg_enmDump_t)m_cfg->h_cEnmDump >= jpg_enmDump_t::INOUT
        || (jpg_enmDump_t)m_cfg->itfEnmDump >= jpg_enmDump_t::INOUT
    ) {
        if (   m_cfg->idxMcuX == m_cfg->numMcuX - 1
            && m_cfg->idxMcuY == m_cfg->numMcuY - 1
            && m_cfg->idxMcuR == m_cfg->numMcuR - 1
        ) {
            dumpDatRbsFrame();
        }
    }
}

// dumpNumRbsFrame
void JPG_H_C::dumpNumRbsFrame() {
    string strObj = "numRbsFrame";
    string strIdx = "";
    string strTyp = "S0I32F0";
    string strPst = "dat";
    int    datMsk = 0xffffffff;
    string strFmt = "%08x";
    JPG_FUNC_DUMP_D_0(m_dumpNumRbsFrameFlg
        ,             m_dumpNumRbsFrameFpt
        ,             strObj, strIdx, strTyp, strPst
        ,             m_numRbsFrame
        ,             datMsk, strFmt
    );
}

// dumpDatRbsFrame
void JPG_H_C::dumpDatRbsFrame() {
    string strObj = "datRbsFrame";
    string strIdx = "";
    string strTyp = "S0I8F0";
    string strPst = "dat";
    int    datMsk = 0xff;
    string strFmt = "%02x";
    int    siz    = m_numRbsFrame;
    for (int i = 0; i < siz; ++i) {
        JPG_FUNC_DUMP_D_0(m_dumpDatRbsFrameFlg
            ,             m_dumpDatRbsFrameFpt
            ,             strObj, strIdx, strTyp, strPst
            ,             m_datRbsFrame[i]
            ,             datMsk, strFmt
        );
    }
}

// dumpDatCoeQtz
void JPG_H_C::dumpDatCoeQtzInv(
int idxLaC
) {
    string strObj = "datCoeQtzInv";
    string strIdx[2] = {"LU","CH"};
    string strTyp = "S1I1F16";
    string strPst = "dat";
    int    datMsk = 0x1ffff;
    string strFmt = "%x";
    int    siz    = JPG_NUMB_PXL_BLK;
    for (int i = 0; i < siz; ++i) {
        JPG_FUNC_DUMP_D_0(m_dumpDatCoeQtzInvFlg[idxLaC]
            ,             m_dumpDatCoeQtzInvFpt[idxLaC]
            ,             strObj, strIdx[idxLaC], strTyp, strPst
            ,             (*m_datCoeQtzInv)[idxLaC][i]
            ,             datMsk, strFmt
        );
    }
}

// dumpNumBitDict
void JPG_H_C::dumpNumBitDict(
int enmCoe, int idxLaC
) {
    string strObj = "numBitDict";
    string strIdx0[2] = {"DC","AC"};
    string strIdx1[2] = {"LU","CH"};
    string strIdx = strIdx0[enmCoe] + "_" + strIdx1[idxLaC];
    string strTyp = "S0I8F0";
    string strPst = "dat";
    int    datMsk = 0xff;
    string strFmt = "%x";
    int    siz    = JPG_H_C_NUMB_SYMBOL_MAX;
    for (int i = 0; i < siz; ++i) {
        JPG_FUNC_DUMP_D_0(m_dumpNumBitDictFlg[idxLaC + enmCoe * (int)jpg_enmCoe_t::NUMB]
            ,             m_dumpNumBitDictFpt[idxLaC + enmCoe * (int)jpg_enmCoe_t::NUMB]
            ,             strObj, strIdx, strTyp, strPst
            ,             (*m_numBitDict)[enmCoe][idxLaC][i]
            ,             datMsk, strFmt
        );
    }
}

// dumpDatBitDict
void JPG_H_C::dumpDatBitDict(
int enmCoe, int idxLaC
) {
    string strObj = "datBitDict";
    string strIdx0[2] = {"DC","AC"};
    string strIdx1[2] = {"LU","CH"};
    string strIdx = strIdx0[enmCoe] + "_" + strIdx1[idxLaC];
    string strTyp = "S0I16F0";
    string strPst = "dat";
    int    datMsk = 0xffff;
    string strFmt = "%04x";
    int    siz    = JPG_H_C_NUMB_SYMBOL_MAX;
    for (int i = 0; i < siz; ++i) {
        JPG_FUNC_DUMP_D_0(m_dumpDatBitDictFlg[idxLaC + enmCoe * (int)jpg_enmCoe_t::NUMB]
            ,             m_dumpDatBitDictFpt[idxLaC + enmCoe * (int)jpg_enmCoe_t::NUMB]
            ,             strObj, strIdx, strTyp, strPst
            ,             (*m_datBitDict)[enmCoe][idxLaC][i]
            ,             datMsk, strFmt
        );
    }
}
