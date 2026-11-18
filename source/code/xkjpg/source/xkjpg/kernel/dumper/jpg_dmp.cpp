//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_dmp.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : dumper related codes
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_dmp.hpp"


//*** FUNCTION *****************************************************************
// connect
void JPG_DMP::connect(
jpg_cfg_t &cfg,
jpg_pxl_t (*datRecFrame)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X])
{
    m_datRecFrame = datRecFrame ;
}

// setDatRecFrame
void JPG_DMP::setDatRecFrame(
const jpg_cfg_t cfg, const jpg_pxl_t datRecMcu[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU])
{
    if ((jpg_enmFormat_t)cfg.enmFormat == jpg_enmFormat_t::YUV420) {
        for(int idxChn = 0; idxChn < cfg.numChn; ++idxChn) {
            int datShift = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
            for(int j = 0; j < cfg.sizMcu >> datShift; ++j) {
                for(int i = 0; i < cfg.sizMcu >> datShift; ++i)
                    (*m_datRecFrame)[idxChn][cfg.idxMcuY * (cfg.sizMcu >> datShift) + j][cfg.idxMcuX * (cfg.sizMcu >> datShift) + i] = datRecMcu[idxChn][j][i];
            }
        }
    }
    else if ((jpg_enmFormat_t)cfg.enmFormat == jpg_enmFormat_t::YUV422) {
        for(int idxChn = 0; idxChn < cfg.numChn; ++idxChn) {
            int datShift = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
            for(int j = 0; j < (cfg.sizMcu >> (datShift + 1)); ++j) {
                for(int i = 0; i < cfg.sizMcu >> datShift; ++i)
                    (*m_datRecFrame)[idxChn][cfg.idxMcuY * (cfg.sizMcu >> datShift) + (cfg.sizMcu >> (datShift + 1)) * cfg.idxMcuR  + j][cfg.idxMcuX * (cfg.sizMcu >> datShift) + i] = datRecMcu[idxChn][j + (cfg.sizMcu >> (datShift + 1)) *cfg.idxMcuR][i];
            }
        }
    }
}
