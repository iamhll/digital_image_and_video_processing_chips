//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_knl_load.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : kernel related codes (load)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_knl.hpp"


//*** FUNCTION *****************************************************************
// loadFile
void JPG_KNL::loadFile()
{
    // for each frame
    {
        if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC) {
            loadFileOri();
        }
        else {
            loadFileBs();
        }
    }

    // for the first frame
    static bool flag = 1;
    if (flag) {
        // clear flag
        flag = 0;
        if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC) {
            for (int idxLaC = 0; idxLaC < JPG_NUMB_LAC; ++idxLaC) {
                loadFileQtz(idxLaC);
            }
            for (int enmCoe = 0; enmCoe < (int)jpg_enmCoe_t::NUMB; ++enmCoe) {
                for (int idxLaC = 0; idxLaC < JPG_NUMB_LAC; ++idxLaC) {
                    int siz = 0;
                    loadFileHfmNum(enmCoe,idxLaC);
                    for (int i = 0; i < JPG_SIZE_CODE_MAX; ++i) {
                        siz += jpg_m_numSymbolDht[enmCoe][idxLaC][i];
                    }
                    loadFileHfmDat(enmCoe,idxLaC,siz);
                }
            }
        }
    }
}

// loadFileOri
void JPG_KNL::loadFileOri()
{
    // open file
    if (!m_loadFileOriFlg) {
        m_loadFileOriFpt = fopen(m_cfg->strFileInp.c_str(), "rb");
        m_loadFileOriFlg = 1;
    }

    // load
    long offset = ((long)m_cfg->idxFrameBgn + m_cfg->idxFrame)                     // frame index
        *         m_cfg->trueSizFrameX * m_cfg->trueSizFrameY                      // frame size of luma channel
        *         (4 + ((m_cfg->numChn == 1 ? 0 : 8) >> (JPG_DATA_SHF_CH * 2)))    // normalized channel amount
        /          4
        *          sizeof(jpg_pxl_t)                                               // pixel bytes
    ;
    fseek(m_loadFileOriFpt, offset, SEEK_SET);
    for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn) {
        int datShf = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
        for (int j = 0; j < m_cfg->trueSizFrameY >> datShf; ++j)
            assert(fread((*m_datOriFrame)[idxChn][j], sizeof(jpg_pxl_t), m_cfg->trueSizFrameX >> datShf, m_loadFileOriFpt) == (size_t)m_cfg->trueSizFrameX >> datShf);
    }

    // extend
    for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn) {
        int datShf = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
        for (int j = 0; j < m_cfg->trueSizFrameY >> datShf; ++j) {
            for (int i = m_cfg->trueSizFrameX >> datShf; i < m_cfg->sizFrameX >> datShf; ++i)
                (*m_datOriFrame)[idxChn][j][i] = (*m_datOriFrame)[idxChn][j][(m_cfg->trueSizFrameX >> datShf) - 1];
        }
        for (int j = m_cfg->trueSizFrameY >> datShf; j < m_cfg->sizFrameY >> datShf; ++j) {
            for (int i = 0; i < m_cfg->sizFrameX >> datShf; ++i)
                (*m_datOriFrame)[idxChn][j][i] = (*m_datOriFrame)[idxChn][(m_cfg->trueSizFrameY >> datShf) - 1][i];
        }
    }
}

// loadFileBs
void JPG_KNL::loadFileBs()
{
    // open file
    string strFileCfg = m_cfg->strFileInp + "_" + to_string(m_cfg->idxFrame) + ".jpg";;
    FILE* fptBs = fopen(strFileCfg.c_str(), "rb");

    // load
    fseek(fptBs,-2,SEEK_END); // !!! BYTE_EOI
    int sizRbs = ftell(fptBs) - m_cfg->idxBsCfg;
    fseek(fptBs,m_cfg->idxBsCfg,SEEK_SET);
    memset(m_datRbsFrame, 0, JPG_NUMB_CHN * JPG_SIZE_FRAME_MAX_Y * JPG_SIZE_FRAME_MAX_X * sizeof(jpg_pxl_t));
    assert(fread(m_datRbsFrame, sizeof(uint8_t), sizRbs, fptBs) == (size_t)sizRbs);
    fclose(fptBs);
}

// loadFileQtz
void JPG_KNL::loadFileQtz(
int idxLaC
){
    string strObj = "datQtzCoe";
    string strIdx[2] = {"LU","CH"};
    string strTyp = "S0I8F0";
    string strPst = "dat";
    string strFmt = "%d";
    int    siz    = JPG_SIZE_BLK;
    int    idxDlt = 1;
    JPG_FUNC_LOAD_D_2(m_loadFileQtzFlg[idxLaC]
        ,             m_loadFileQtzFpt[idxLaC]
        ,             strObj, strIdx[idxLaC], strTyp, strPst
        ,             jpg_m_datCoeQtz[idxLaC]
        ,             strFmt
        ,             siz, idxDlt
        ,             siz, idxDlt
    );
}

// loadFileHfmNum
void JPG_KNL::loadFileHfmNum(
int enmCoe, int idxLaC
){
    string strObj = "numSymbolDht";
    string strIdx0[2] = {"DC","AC"};
    string strIdx1[2] = {"LU","CH"};
    string strIdx  = strIdx0[enmCoe] + "_" + strIdx1[idxLaC];
    string strTyp = "S0I8F0";
    string strPst = "dat";
    string strFmt = "%02x";
    int    siz    = JPG_SIZE_CODE_MAX;
    int    idxDlt = 1;
    JPG_FUNC_LOAD_D_1(m_loadFileHfmNumFlg[enmCoe + idxLaC * (int)jpg_enmCoe_t::NUMB]
        ,             m_loadFileHfmNumFpt[enmCoe + idxLaC * (int)jpg_enmCoe_t::NUMB]
        ,             strObj, strIdx, strTyp, strPst
        ,             jpg_m_numSymbolDht[enmCoe][idxLaC]
        ,             strFmt
        ,             siz, idxDlt
    );
}

// loadFileHfmDat
void JPG_KNL::loadFileHfmDat(
int enmCoe, int idxLaC, int siz
){
    string strObj = "datSymbolDht";
    string strIdx0[2] = {"DC","AC"};
    string strIdx1[2] = {"LU","CH"};
    string strIdx  = strIdx0[enmCoe] + "_" + strIdx1[idxLaC];
    string strTyp = "S0I8F0";
    string strPst = "dat";
    string strFmt = "%02x";
    int    idxDlt = 1;
    JPG_FUNC_LOAD_D_1(m_loadFileHfmDatFlg[enmCoe + idxLaC * (int)jpg_enmCoe_t::NUMB]
        ,             m_loadFileHfmDatFpt[enmCoe + idxLaC * (int)jpg_enmCoe_t::NUMB]
        ,             strObj, strIdx, strTyp, strPst
        ,             jpg_m_datSymbolDht[enmCoe][idxLaC]
        ,             strFmt
        ,             siz, idxDlt
    );
}
