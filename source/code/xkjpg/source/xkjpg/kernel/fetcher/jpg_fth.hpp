//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_fth.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : fetcher related headers
    //
//------------------------------------------------------------------------------

#ifndef __JPG_FTH_HPP__
#define __JPG_FTH_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_common.hpp"


//*** CLASS ********************************************************************
class JPG_FTH
{
//--- PRIVATE VARIABLE -----------------
private:
    // pixel
        jpg_pxl_t (*m_datOriFrame)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X];


//--- PUBLIC FUNCTION ------------------
public:
    // basic
        void connect(jpg_pxl_t (*datOriFrame)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X]
        );

    // copy
        void cpyDatOriMcu(jpg_pxl_t dat[JPG_SIZE_MCU][JPG_SIZE_MCU], const jpg_cfg_t &cfg, int idxChn);
};

#endif /* __JPG_FTH_HPP__ */
