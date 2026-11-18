//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_knl.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : kernel related codes
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_knl.hpp"


//*** CLASS ********************************************************************
// JPG_KNL
JPG_KNL::JPG_KNL()
{
    // name
    m_strTag = "JPG_KNL";

    // pixel (ori, rec)
    m_datOriFrame = (jpg_pxl_t (*)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X])
        new          jpg_pxl_t    [JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X];
    m_datRecFrame = (jpg_pxl_t (*)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X])
        new          jpg_pxl_t    [JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X];
    memset(*m_datOriFrame, 0, sizeof(*m_datOriFrame));
    memset(*m_datRecFrame, 0, sizeof(*m_datRecFrame));

    // pixel (rbs, fbs)
    m_datRbsFrame = new uint8_t[JPG_NUMB_CHN * JPG_SIZE_FRAME_MAX_Y * JPG_SIZE_FRAME_MAX_X * sizeof(jpg_pxl_t)];
    m_datFbsFrame = new uint8_t[JPG_NUMB_CHN * JPG_SIZE_FRAME_MAX_Y * JPG_SIZE_FRAME_MAX_X * sizeof(jpg_pxl_t)];
    memset(m_datRbsFrame, 0, JPG_NUMB_CHN * JPG_SIZE_FRAME_MAX_Y * JPG_SIZE_FRAME_MAX_X * sizeof(jpg_pxl_t));
    memset(m_datFbsFrame, 0, JPG_NUMB_CHN * JPG_SIZE_FRAME_MAX_Y * JPG_SIZE_FRAME_MAX_X * sizeof(jpg_pxl_t));

    // information (dct)
    m_datQtzCoe =     (int (*)[JPG_NUMB_LAC][JPG_SIZE_BLK][JPG_SIZE_BLK])
        new            int    [JPG_NUMB_LAC][JPG_SIZE_BLK][JPG_SIZE_BLK];
    memset(*m_datQtzCoe, 0, sizeof(*m_datQtzCoe));

    // !!! special cases
    for (int c = 0; c < JPG_NUMB_CHN      ; ++c) {
        for (int j = 0; j < JPG_SIZE_FRAME_MAX_Y; ++j) {
            for (int i = 0; i < JPG_SIZE_FRAME_MAX_X; ++i)
                (*m_datOriFrame)[c][j][i] = 1 << (JPG_DATA_PXL_WD - 1);
        }
    }
}

// ~JPG_KNL
JPG_KNL::~JPG_KNL()
{
    // delete
    {
        delete[] m_datOriFrame;
        delete[] m_datRecFrame;

        delete[] m_datRbsFrame;
        delete[] m_datFbsFrame;

        delete[] m_datQtzCoe;
    }
}

// connect
void JPG_KNL::connect(
jpg_cfg_t &cfg)
{
    // cfg
    m_cfg = &cfg;

    // other modules
    switch ((jpg_enmModRun_t)m_cfg->enmModRun) {
    case jpg_enmModRun_t::ENC: {
        m_fth.connect(     m_datOriFrame);
        m_dct.connect(cfg, m_fth, m_dmp, m_h_d);
        m_h_c.connect(cfg, m_fth, m_dmp, m_dct, m_datRbsFrame, m_datFbsFrame);
        m_dmp.connect(cfg, m_datRecFrame);
    break;}
    case jpg_enmModRun_t::DEC: {
        m_h_d.connect(cfg, m_fth, m_dmp, m_datRbsFrame);
        m_dct.connect(cfg, m_fth, m_dmp, m_h_d);
        m_dmp.connect(cfg, m_datRecFrame);
    break;}
    }
}

// oneShot
void JPG_KNL::oneShot()
{
    updTic(enmFunc_t::LOAD); load(); updToc(enmFunc_t::LOAD);
    updTic(enmFunc_t::PROC); proc(); updToc(enmFunc_t::PROC);
    updTic(enmFunc_t::DUMP); dump(); updToc(enmFunc_t::DUMP);
}

// load
void JPG_KNL::load()
{
    loadFile();
}

// proc
void JPG_KNL::proc()
{
    procMain();
}

// dump
void JPG_KNL::dump()
{
    dumpFile();
}

// dspRunTime
void JPG_KNL::dspRunTime()
{
    #ifdef JPG_KNOB_TIME
    #ifdef __linux__
    cout << "run time of LOAD           is " << setprecision(3) << (double)m_datTAcc[(int)enmFunc_t::LOAD          ] / 1000 << " s" << endl;
    cout << "run time of PROC           is " << setprecision(3) << (double)m_datTAcc[(int)enmFunc_t::PROC          ] / 1000 << " s" << endl;
    cout << "run time of PROC PREV      is " << setprecision(3) << (double)m_datTAcc[(int)enmFunc_t::PROC_PREV     ] / 1000 << " s" << endl;
    cout << "run time of PROC CORE      is " << setprecision(3) << (double)m_datTAcc[(int)enmFunc_t::PROC_CORE     ] / 1000 << " s" << endl;
    m_dct.dspRunTime();
    m_h_c.dspRunTime();
    cout << "run time of PROC CORE MISC is " << setprecision(3) << (double)m_datTAcc[(int)enmFunc_t::PROC_CORE_MISC] / 1000 << " s" << endl;
    cout << "run time of PROC POST      is " << setprecision(3) << (double)m_datTAcc[(int)enmFunc_t::PROC_POST     ] / 1000 << " s" << endl;
    cout << "run time of DUMP           is " << setprecision(3) << (double)m_datTAcc[(int)enmFunc_t::DUMP          ] / 1000 << " s" << endl;
    cout << endl;
    #endif
    #endif
}
