//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_c.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_encoding related codes
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
// JPG_H_C
JPG_H_C::JPG_H_C()
{
    // identification
    m_strTag = "JPG_H_C";

    // pixel (coe)
    m_datCoeZigZag = (jpg_coe_t (*)[JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK])
        new           jpg_coe_t    [JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK];
    memset(*m_datCoeZigZag, 0, sizeof(*m_datCoeZigZag));

    m_enmCoeRle = (uint8_t   (*)[JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK])
        new        uint8_t      [JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK];
    m_datCoeRle = (jpg_coe_t (*)[JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK])
        new        jpg_coe_t    [JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK];
    memset(*m_enmCoeRle, 0, sizeof(*m_enmCoeRle));
    memset(*m_datCoeRle, 0, sizeof(*m_datCoeRle));

    // information (qtz)
    m_datCoeQtz = (jpg_coe_t (*)[JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK])
        new        jpg_coe_t    [JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK];
    memset(*m_datCoeQtz, 0, sizeof(*m_datCoeQtz));
    m_datCoeQtzInv = (jpg_coe_t (*)[JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK])
        new           jpg_coe_t    [JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK];
    memset(*m_datCoeQtzInv, 0, sizeof(*m_datCoeQtzInv));

    // information (dict)
    m_numBitDict = (uint16_t (*)[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_C_NUMB_SYMBOL_MAX])
        new         uint16_t    [(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_C_NUMB_SYMBOL_MAX];
    m_datBitDict = (uint16_t (*)[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_C_NUMB_SYMBOL_MAX])
        new         uint16_t    [(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_C_NUMB_SYMBOL_MAX];
    memset(*m_numBitDict, 0, sizeof(*m_numBitDict));
    memset(*m_datBitDict, 0, sizeof(*m_datBitDict));
}

// ~JPG_H_C
JPG_H_C::~JPG_H_C()
{
    // module
    //m_h_c_to_hm.destroy();

    // pixel (coe)
    delete[] m_datCoeZigZag;
    delete[] m_enmCoeRle;
    delete[] m_datCoeRle;

    // information (qtz)
    delete[] m_datCoeQtz;
    delete[] m_datCoeQtzInv;

    // information (dict)
    delete[] m_numBitDict;
    delete[] m_datBitDict;
}

// connect
void JPG_H_C::connect(
jpg_cfg_t &cfg, JPG_FTH &fth, JPG_DMP &dmp, JPG_UNIT &dct, uint8_t *datRbsFrame, uint8_t *datFbsFrame)
{
    // basic
    JPG_UNIT::connect(cfg, fth, dmp);

    // other modules
    m_dct         = &dct;
    m_datRbsFrame = datRbsFrame;
    m_datFbsFrame = datFbsFrame;

    // other configurations
    m_cfgFlgProc =                m_cfg->h_cFlgProc;
    m_cfgEnmLoad = (jpg_enmLoad_t)m_cfg->h_cEnmLoad;
}
