//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_dmp.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : dumper related headers
    //
//------------------------------------------------------------------------------

#ifndef __JPG_DMP_HPP__
#define __JPG_DMP_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_common.hpp"


//*** CLASS ********************************************************************
class JPG_DMP
{
//--- PRIVATE VARIABLE -----------------
private:
    // pixel (rec)
        //                        [channel     ][pixel position y    ][pixel position x    ]
        jpg_pxl_t (*m_datRecFrame)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X];


//--- PUBLIC FUNCTION ------------------
public:
    // basic
        void connect(jpg_cfg_t &cfg
            ,        jpg_pxl_t (*datRecFrame)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X]
        );

    // set
        void setDatRecFrame(const jpg_cfg_t cfg, const jpg_pxl_t datRecMcu[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU]);
};

#endif /* __JPG_DMP_HPP__ */
