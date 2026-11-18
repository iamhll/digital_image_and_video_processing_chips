//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_knl.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : kernel related headers
    //
//------------------------------------------------------------------------------

#ifndef __JPG_KNL_HPP__
#define __JPG_KNL_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_fth.hpp"
    #include "jpg_dct.hpp"
    #include "jpg_h_c.hpp"
    #include "jpg_h_d.hpp"
    #include "jpg_dmp.hpp"


//*** CLASS ********************************************************************
// JPG_KNL
class JPG_KNL
{
//--- PRIVATE ENUMERATOR ---------------
private:
    // enmFunc_t
        enum class enmFunc_t {
            LOAD = 0,
            PROC,
            PROC_PREV,
            PROC_CORE,
            PROC_CORE_MISC,
            PROC_POST,
            DUMP,
            NUMB
        };


//--- PRIVATE VARIABLE -----------------
private:
    // identification
        string m_strTag;

    // cfg
        jpg_cfg_t *m_cfg;

    // pixel (ori, rec)
        jpg_pxl_t (*m_datOriFrame)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X];
        jpg_pxl_t (*m_datRecFrame)[JPG_NUMB_CHN][JPG_SIZE_FRAME_MAX_Y][JPG_SIZE_FRAME_MAX_X];

    // pixel (rbs, fbs)
        int      m_numRbsFrame;    // bs number without header
        uint8_t *m_datRbsFrame;    // bs data   without header
        int      m_numFbsFrame;    // bs number with    header
        uint8_t *m_datFbsFrame;    // bs data   with    header

    // information (dct)
        int      (*m_datQtzCoe)[JPG_NUMB_LAC][JPG_SIZE_BLK    ][JPG_SIZE_BLK    ];

    // time
        #ifdef JPG_KNOB_TIME
        #ifdef __linux__
        timespec m_datTic      [(int)enmFunc_t::NUMB];
        timespec m_datToc      [(int)enmFunc_t::NUMB];
        unsigned long m_datTAcc[(int)enmFunc_t::NUMB] = {0};
        #endif
        #endif

    // module
        JPG_FTH m_fth;
        JPG_DCT m_dct;
        JPG_H_C m_h_c;
        JPG_H_D m_h_d;
        JPG_DMP m_dmp;


//--- PRIVATE FUNCTION -----------------
private:
    // oneShot
        void load();
        void proc();
        void dump();

    // load
        void loadFile();
        void loadFileOri   (                               ); bool m_loadFileOriFlg       = 0; FILE *m_loadFileOriFpt;
        void loadFileBs    (                               ); bool m_loadFileBsFlg        = 0; FILE *m_loadFileBsFpt ;
        void loadFileQtz   (int idxLaC                     ); bool m_loadFileQtzFlg[2]    = {0}; FILE *m_loadFileQtzFpt[2];
        void loadFileHfmNum(int enmCoe, int idxLaC         ); bool m_loadFileHfmNumFlg[4] = {0}; FILE *m_loadFileHfmNumFpt[4];
        void loadFileHfmDat(int enmCoe, int idxLaC, int siz); bool m_loadFileHfmDatFlg[4] = {0}; FILE *m_loadFileHfmDatFpt[4];

    // process
        void procMain();

        void procPrev();
        void procCore();
        void procPost();

        void procCoreCfg();

    // dump
        void dumpFile();

        void dumpFileRec(); bool m_dumpFileRecFlg = 0; FILE *m_dumpFileRecFpt;
        void dumpFileBin(); bool m_dumpFileBinFlg = 0; FILE *m_dumpFileBinFpt;
        void dumpFileCsv(); bool m_dumpFileCsvFlg = 0; FILE *m_dumpFileCsvFpt;
        void dumpFileCsvTail(double datPsnr[3], double datSsim[3], int cntFrame, double datBitRate, int idx, double datWeightY, double datWeightYuv);

        void dumpDatOriFrame(int idxChn); bool m_dumpDatOriFrameFlg[3] = {0}; FILE *m_dumpDatOriFrameFpt[3];
        void dumpDatRecFrame(int idxChn); bool m_dumpDatRecFrameFlg[3] = {0}; FILE *m_dumpDatRecFrameFpt[3];

        void dumpDatSymbolDht(int idxLaC, int idxTypeCoe); bool m_dumpDatSymbolDhtFlg[4] = {0}; FILE *m_dumpDatSymbolDhtFpt[4];


//--- PRIVATE INLINE -------------------
private:
    #include "jpg_knl_proc.hpp"


//--- PUBLIC FUNCTION ------------------
public:
    // basic
        JPG_KNL();
        ~JPG_KNL();
        void connect(jpg_cfg_t &cfg);
        void oneShot();

    // display
        void dspRunTime();
};

#endif /* __JPG_KNL_HPP__ */
