//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_d_dump.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_decoding related codes (dump)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_h_d.hpp"


//*** FUNCTION *****************************************************************
// dumpFile
void JPG_H_D::dumpFile()
{
    // basic
    JPG_UNIT::dumpFile();

}

// dumpDatCoeQtz
void JPG_H_D::dumpDatCoeQtz(
int idxLaC
) {
    string strObj = "datCoeQtz";
    string strIdx[2] = {"LU","CH"};
    string strTyp = "S0I8F0";
    string strPst = "dat";
    int    datMsk = 0xff;
    string strFmt = "%x";
    int    siz    = JPG_NUMB_PXL_BLK;
    for (int i = 0; i < siz; ++i) {
        JPG_FUNC_DUMP_D_0(m_dumpDatCoeQtzFlg[idxLaC]
            ,             m_dumpDatCoeQtzFpt[idxLaC]
            ,             strObj, strIdx[idxLaC], strTyp, strPst
            ,             jpg_m_datCoeQtzZigzag[idxLaC][i]
            ,             datMsk, strFmt
        );
    }
}

// dumpNumBitDict
void JPG_H_D::dumpDatBitDictThresh(
int enmCoe, int idxLaC
) {
    string strObj = "datBitDictThresh";
    string strIdx0[2] = {"DC","AC"};
    string strIdx1[2] = {"LU","CH"};
    string strIdx = strIdx0[enmCoe] + "_" + strIdx1[idxLaC];
    string strTyp = "S0I16F0";
    string strPst = "dat";
    int    datMsk = 0xffff;
    string strFmt = "%x";
    int    siz    = JPG_H_D_SIZE_CODE_MAX;
    for (int i = 0; i < siz; ++i) {
        JPG_FUNC_DUMP_D_0(m_dumpDatBitDictThreshFlg[idxLaC + enmCoe * (int)jpg_enmCoe_t::NUMB]
            ,             m_dumpDatBitDictThreshFpt[idxLaC + enmCoe * (int)jpg_enmCoe_t::NUMB]
            ,             strObj, strIdx, strTyp, strPst
            ,             (*m_datBitDictThresh)[enmCoe][idxLaC][i]
            ,             datMsk, strFmt
        );
    }
}

// dumpDatBitDict
void JPG_H_D::dumpIdxBitDictThresh(
int enmCoe, int idxLaC
) {
    string strObj = "idxBitDictThresh";
    string strIdx0[2] = {"DC","AC"};
    string strIdx1[2] = {"LU","CH"};
    string strIdx = strIdx0[enmCoe] + "_" + strIdx1[idxLaC];
    string strTyp = "S0I8F0";
    string strPst = "dat";
    int    datMsk = 0xff;
    string strFmt = "%04x";
    int    siz    = JPG_H_D_SIZE_CODE_MAX;
    for (int i = 0; i < siz; ++i) {
        JPG_FUNC_DUMP_D_0(m_dumpIdxBitDictThreshFlg[idxLaC + enmCoe * (int)jpg_enmCoe_t::NUMB]
            ,             m_dumpIdxBitDictThreshFpt[idxLaC + enmCoe * (int)jpg_enmCoe_t::NUMB]
            ,             strObj, strIdx, strTyp, strPst
            ,             (*m_idxBitDictThresh)[enmCoe][idxLaC][i]
            ,             datMsk, strFmt
        );
    }
}
