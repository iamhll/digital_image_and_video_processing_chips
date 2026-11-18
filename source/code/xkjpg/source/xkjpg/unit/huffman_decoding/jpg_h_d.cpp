//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_d.hpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_decoding related codes
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_h_d.hpp"


//*** FUNCTION *****************************************************************
// JPG_H_D
JPG_H_D::JPG_H_D()
{
    // identification
    m_strTag = "JPG_H_D";

    // pixel (coe)
    m_datCoeZigZag = (jpg_coe_t (*)[JPG_NUMB_PXL_BLK])
        new           jpg_coe_t    [JPG_NUMB_PXL_BLK];
    memset(*m_datCoeZigZag, 0, sizeof(*m_datCoeZigZag));

    // information (qtz)
    m_datCoeQtzInv = (jpg_coe_t (*)[JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK])
        new           jpg_coe_t    [JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK];
    memset(*m_datCoeQtzInv, 0, sizeof(*m_datCoeQtzInv));

    // information (huffman decode)
    m_datBitDictThresh = (uint16_t (*)[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_D_SIZE_CODE_MAX])
        new               uint16_t    [(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_D_SIZE_CODE_MAX];
    memset(*m_datBitDictThresh, 0, sizeof(*m_datBitDictThresh));
    m_idxBitDictThresh = (int (*)[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_D_SIZE_CODE_MAX])
        new               int    [(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_D_SIZE_CODE_MAX];
    memset(*m_idxBitDictThresh, 0, sizeof(*m_idxBitDictThresh));
}

// ~JPG_H_D
JPG_H_D::~JPG_H_D()
{
    // module
    //m_h_c_to_hm.destroy();

    // pixel (coe)
    delete[] m_datCoeZigZag;

    // information (qtz)
    delete[] m_datCoeQtzInv;

    // information (huffman decode)
    delete[] m_datBitDictThresh;
    delete[] m_idxBitDictThresh;
}

// connect
void JPG_H_D::connect(
jpg_cfg_t &cfg, JPG_FTH &fth, JPG_DMP &dmp, uint8_t *datRbsFrame)
{
    // basic
    JPG_UNIT::connect(cfg, fth, dmp);

    // other modules
    m_datRbsFrame = datRbsFrame;

    // other configurations
    m_cfgFlgProc =                m_cfg->h_cFlgProc;
    m_cfgEnmLoad = (jpg_enmLoad_t)m_cfg->h_cEnmLoad;
}
