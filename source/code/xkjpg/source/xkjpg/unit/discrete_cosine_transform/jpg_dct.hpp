//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_dct.hpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : discrete_cosine_transform related headers
    //
//------------------------------------------------------------------------------

#ifndef __JPG_DCT_HPP__
#define __JPG_DCT_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_unit.hpp"


//*** DEFINE *******************************************************************
    #define JPG_DCT_DATA_DCT_FRC_WD    8


//*** CLASS ********************************************************************
// JPG_DCT
class JPG_DCT: public JPG_UNIT
{
public:
    // connection
        JPG_UNIT *m_h_d;
//--- PRIVATE TABLE --------------------
private:


//--- PRIVATE VARIABLE -----------------
private:
    // pixel (ori)
        jpg_pxl_t (*m_true_datOriMcu)[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU];

    // pixel (rec)
        jpg_pxl_t (*     m_datRecMcu)[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU];
        jpg_pxl_t (*m_true_datRecMcu)[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU];

    // transformation (tsf)
        //                     [pixel position y][pixel position x]
        double (*m_datCoeTsf08)[JPG_SIZE_BLK    ][JPG_SIZE_BLK    ];

    // transformation (qtz)
        //                      [channel     ][pixel position y][pixel position x]
        double (*m_datCoeQtzInv)[JPG_NUMB_CHN][JPG_SIZE_BLK    ][JPG_SIZE_BLK    ];


//--- PRIVATE FUNCTION -----------------
private:
    // load
        virtual void loadPipe();
        //virtual void loadFile();
        //virtual void loadRand();

    // process
        virtual void procPrev();
        virtual void procCore();
                void procCoreMcuFT();
                void procCoreMcuBT();
        virtual void procPost();

    // dump
        virtual void dumpFile();
                void dumpDatRecMcu(int idxChn); bool m_dumpDatRecMcuFlg[3] = {0}; FILE *m_dumpDatRecMcuFpt[3];


//--- PUBLIC FUNCTION ------------------
public:
    // basic
        JPG_DCT();
        ~JPG_DCT();
        virtual void connect(jpg_cfg_t &cfg, JPG_FTH &fth, JPG_DMP &dmp, JPG_UNIT &h_d);


//--- PUBLIC INLINE --------------------
public:
    //...
};

#endif /* __JPG_DCT_HPP__ */
