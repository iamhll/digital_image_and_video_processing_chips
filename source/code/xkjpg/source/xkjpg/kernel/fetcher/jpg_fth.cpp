//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_fth.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : fetcher related codes
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_fth.hpp"


//*** FUNCTION *****************************************************************
// connect
void JPG_FTH::connect(
jpg_pxl_t (*datOriFrame)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X])
{
    m_datOriFrame = datOriFrame ;
}

// cpyDatOriMcu
void JPG_FTH::cpyDatOriMcu(jpg_pxl_t dat[JPG_SIZE_MCU][JPG_SIZE_MCU],
const jpg_cfg_t &cfg, int idxChn)
{
    int datShf = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
    for (int j = 0; j < cfg.sizMcu >> datShf; ++j)
        memcpy(dat[j], &(*m_datOriFrame)[idxChn][cfg.idxMcuY * (cfg.sizMcu >> datShf) + j][cfg.idxMcuX * (cfg.sizMcu >> datShf)], sizeof(jpg_pxl_t) * (cfg.sizMcu >> datShf));
}
