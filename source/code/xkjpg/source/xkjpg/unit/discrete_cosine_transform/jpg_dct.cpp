//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_dct.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : discrete_cosine_transform related codes (top)
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2022-12-09 by HLL, LYZ
    //  Description    : fully tidied up
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_dct.hpp"


//*** FUNCTION *****************************************************************
// JPG_DCT
JPG_DCT::JPG_DCT()
{
    // identification
    m_strTag = "JPG_DCT";

    // load (flg)
    for (int c = 0; c < JPG_NUMB_CHN; ++c)
        m_loadDatOriMcuFlg[c] = 1;

    // pixel (ori)
    m_true_datOriMcu = (jpg_pxl_t (*)[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU])
        new             jpg_pxl_t    [JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU];
    memset(*m_true_datOriMcu, 0, sizeof(*m_true_datOriMcu));

    // pixel (rec)
    m_datRecMcu      = (jpg_pxl_t (*)[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU])
        new             jpg_pxl_t    [JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU];
    m_true_datRecMcu = (jpg_pxl_t (*)[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU])
        new             jpg_pxl_t    [JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU];
    memset(*     m_datRecMcu, 0, sizeof(*     m_datRecMcu));
    memset(*m_true_datRecMcu, 0, sizeof(*m_true_datRecMcu));

    // transformation (tsf)
    m_datCoeTsf08 =  (double   (*)[JPG_SIZE_BLK][JPG_SIZE_BLK])
        new           double      [JPG_SIZE_BLK][JPG_SIZE_BLK];
    memset(*m_datCoeTsf08, 0, sizeof(*m_datCoeTsf08));

    // transformation (qtz)
    m_datCoeQtzInv = (double   (*)[JPG_NUMB_CHN][JPG_SIZE_BLK][JPG_SIZE_BLK])
        new           double      [JPG_NUMB_CHN][JPG_SIZE_BLK][JPG_SIZE_BLK];
    memset(*m_datCoeQtzInv, 0, sizeof(*m_datCoeQtzInv));
}

// ~JPG_DCT
JPG_DCT::~JPG_DCT()
{
    // delete
    delete[] m_true_datOriMcu;

    delete[] m_datRecMcu;
    delete[] m_true_datRecMcu;

    delete[] m_datCoeTsf08;

    delete[] m_datCoeQtzInv;
}

// connect
void JPG_DCT::connect(
jpg_cfg_t &cfg, JPG_FTH &fth, JPG_DMP &dmp, JPG_UNIT &h_d)
{
    // basic
    JPG_UNIT::connect(cfg, fth, dmp);

    // other modules
    m_h_d         = &h_d;

    // other configurations
    m_cfgFlgProc =                m_cfg->dctFlgProc;
    m_cfgEnmLoad = (jpg_enmLoad_t)m_cfg->dctEnmLoad;
}
