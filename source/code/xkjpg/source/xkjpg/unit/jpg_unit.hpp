//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_unit.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : unit related headers
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2022-12-09 by HLL, LYZ
    //  Description    : fully tidied up
    //
//------------------------------------------------------------------------------

#ifndef __JPG_UNIT_HPP__
#define __JPG_UNIT_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_common.hpp"
    #include "jpg_fth.hpp"
    #include "jpg_dmp.hpp"


//*** CLASS ********************************************************************
// JPG_UNIT
class JPG_UNIT
{
//--- PROTECTED ENUMERATOR -------------
protected:
    // enmFunc_t
        enum class enmFunc_t {
            MAIN = 0,
            MISC_0,
            MISC_1,
            MISC_2,
            MISC_3,
            MISC_4,
            MISC_5,
            MISC_6,
            MISC_7,
            NUMB
        };


//--- PROTECTED VARIABLE ---------------
protected:
    // identification
        string m_strTag = "JPG_UNIT";

    // connection
        const jpg_cfg_t *m_cfg;
        jpg_cfg_t       *m_cfgWritable;
        JPG_FTH         *m_fth;
        JPG_DMP         *m_dmp;

    // configuration
        bool          m_cfgFlgProc = (bool)         0;
        jpg_enmLoad_t m_cfgEnmLoad = (jpg_enmLoad_t)0;

    // pixel (ori)
        //                      [channel     ][pixel position y][pixel position x]
        jpg_pxl_t (*m_datOriMcu)[JPG_NUMB_CHN][JPG_SIZE_MCU    ][JPG_SIZE_MCU    ]; bool m_loadDatOriMcuFlg[JPG_NUMB_CHN] = {0};

    // pixel (coe)
        //                      [channel     ][pixel position y][pixel position x]
        jpg_coe_t (*m_datCoeMcu)[JPG_NUMB_CHN][JPG_SIZE_MCU    ][JPG_SIZE_MCU    ];

    // time
        #ifdef JPG_KNOB_TIME
        #ifdef __linux__
        timespec      m_datTic [(int)enmFunc_t::NUMB];
        timespec      m_datToc [(int)enmFunc_t::NUMB];
        unsigned long m_datTAcc[(int)enmFunc_t::NUMB] = {0};
        #endif
        #endif


//--- PROTECTED FUNCTION ---------------
protected:
    // oneShot
        void load();
        void proc();
        void dump();

    // load
        virtual void loadPipe();
        virtual void loadFile();
        virtual void loadRand();

    // process
        void procMain();
        virtual void procPrev();
        virtual void procCore();
        virtual void procPost();

    // dump
        virtual void dumpFile();
        void         dumpDatOriMcu(int idxChn); bool m_dumpDatOriMcuFlg[3] = {0}; FILE *m_dumpDatOriMcuFpt[3];
        void         dumpDatCoeMcu(int idxChn); bool m_dumpDatCoeMcuFlg[3] = {0}; FILE *m_dumpDatCoeMcuFpt[3];


//--- PROTECTED INLINE -----------------
protected:
    #include "jpg_unit_tool_time.hpp"


//--- PUBLIC FUNCTION ------------------
public:
    // basic
        JPG_UNIT();
        ~JPG_UNIT();
        virtual void connect(jpg_cfg_t &cfg, JPG_FTH &fth, JPG_DMP &dmp);
        void oneShot();


//--- PUBLIC INLINE --------------------
public:
    // cpyCoeMcu
        inline void cpyCoeMcu(void *dst)
        {
            memcpy(dst, m_datCoeMcu, sizeof(*m_datCoeMcu));
        }

    // dspRunTime
        inline void dspRunTime()
        {
            #ifdef JPG_KNOB_TIME
            #ifdef __linux__
            cout << "run time of PROC CORE M" << m_strTag.c_str() + JPG_SIZE_PREFIX << " is " << setprecision(3) << (double)m_datTAcc[0] / 1000 << " s" << endl;
            for (int i = 1; i < (int)enmFunc_t::NUMB; ++i) {
                if (m_datTAcc[i] > 0)
                    cout << "run time of PROC CORE M" << m_strTag.c_str() + JPG_SIZE_PREFIX << " MISC" << i - 1 << setprecision(3) << (double)m_datTAcc[i] / 1000 << " s" << endl;
            }
            #endif
            #endif
        }
};

#endif /* __JPG_UNIT_HPP__ */
