//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_common.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : common headers
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2022-12-09 by HLL, LYZ
    //  Description    : fully tidied up
    //
//------------------------------------------------------------------------------

#ifndef __JPG_COMMON_HPP__
#define __JPG_COMMON_HPP__

//*** INCLUDE ******************************************************************
    #ifdef   NDEBUG
    #undef   NDEBUG
    #include <assert.h>
    #define  NDEBUG
    #else
    #include <assert.h>
    #endif
    #include <bitset>
    #include <cstring>
    #include <fstream>
    #include <iomanip>
    #include <iostream>
    #include <math.h>
    #include <string>
    #ifdef CALC_KNOB_TIME
    #ifdef __linux__
    #include <sys/time.h>
    #endif
    #endif


//*** NAMESPACE ****************************************************************
    using std::bitset;
    using std::cout;
    using std::cerr;
    using std::endl;
    using std::fstream;
    using std::setw;
    using std::setfill;
    using std::setprecision;
    using std::string;
    using std::to_string;


//*** DEFINITION ***************************************************************
//--- TOP ------------------------------
    // OTHER
        #define _CRT_SECURE_NO_WARNINGS

    // FUNCTION
        #define JPG_FUNC_INI_FPT_DUMP(flg, fpt, strObj, strIdx, strTyp, strPst)                          \
        {                                                                                                \
            if (!flg) {                                                                                  \
                flg = 1;                                                                                 \
                string strFul = m_cfg->strPrefixDump                                                     \
                    +                                 (m_strTag.c_str() + JPG_SIZE_PREFIX)               \
                    +           (strObj != "" ? "_" + strObj : "")                                       \
                    +           (strIdx != "" ? "_" + strIdx : "")                                       \
                    +           (strTyp != "" ? "_" + strTyp : "")                                       \
                    +                           "." + strPst                                             \
                ;                                                                                        \
                if (strPst == "dat")                                                                     \
                    fpt = fopen(strFul.c_str(), "w");                                                    \
                else if (strPst == "bin")                                                                \
                    fpt = fopen(strFul.c_str(), "wb");                                                   \
                else                                                                                     \
                    assert(0);                                                                           \
                if (fpt == NULL) {                                                                       \
                    cerr << "[error from c model] cannot open dump file: " << strFul.c_str() << endl;    \
                    assert(0);                                                                           \
                }                                                                                        \
            }                                                                                            \
        }
        #define JPG_FUNC_INI_FPT_LOAD(flg, fpt, strObj, strIdx, strTyp, strPst)                          \
        {                                                                                                \
            if (!flg) {                                                                                  \
                flg = 1;                                                                                 \
                string strFul = m_cfg->strPrefixLoad                                                     \
                    +                                 (m_strTag.c_str() + JPG_SIZE_PREFIX)               \
                    +           (strObj != "" ? "_" + strObj : "")                                       \
                    +           (strIdx != "" ? "_" + strIdx : "")                                       \
                    +           (strTyp != "" ? "_" + strTyp : "")                                       \
                    +                           "." + strPst                                             \
                ;                                                                                        \
                fpt = fopen(strFul.c_str(), "r");                                                        \
                if (fpt == NULL) {                                                                       \
                    cerr << "[error from c model] cannot open load file: " << strFul.c_str() << endl;    \
                    assert(0);                                                                           \
                }                                                                                        \
            }                                                                                            \
        }
        #define JPG_FUNC_DUMP_D_0(flg, fpt, strObj, strIdx, strTyp, strPst, dat, datMsk, strFmt)    \
        {                                                                                           \
            JPG_FUNC_INI_FPT_DUMP(flg, fpt, strObj, strIdx, strTyp, strPst)                         \
            string strFmtFul = strFmt + "\n";                                                       \
            if (strPst == "dat")                                                                    \
                fprintf(fpt, strFmtFul.c_str(), dat & datMsk);                                      \
            else                                                                                    \
                fwrite(&dat, sizeof(dat), 1, fpt);                                                  \
        }
        #define JPG_FUNC_DUMP_D_1(flg, fpt, strObj, strIdx, strTyp, strPst, dat, datMsk, strFmt, siz0, idxDlt0)    \
        {                                                                                                          \
            JPG_FUNC_INI_FPT_DUMP(flg, fpt, strObj, strIdx, strTyp, strPst)                                        \
            string strFmtFul = " " + strFmt;                                                                       \
            for (int idx0 = 0; idx0 < siz0; idx0 += idxDlt0) {                                                     \
                if (strPst == "dat") {                                                                             \
                    if (idx0 == 0)                                                                                 \
                        fprintf(fpt,    strFmt.c_str(), dat[idx0] & datMsk);                                       \
                    else                                                                                           \
                        fprintf(fpt, strFmtFul.c_str(), dat[idx0] & datMsk);                                       \
                }                                                                                                  \
                else {                                                                                             \
                    fwrite(&dat[idx0], sizeof(dat[idx0]), 1, fpt);                                                 \
                }                                                                                                  \
            }                                                                                                      \
            fprintf(fpt, "\n");                                                                                    \
        }
        #define JPG_FUNC_LOAD_D_1(flg, fpt, strObj, strIdx, strTyp, strPst, dat, strFmt, siz0, idxDlt0)    \
        {                                                                                                  \
            JPG_FUNC_INI_FPT_LOAD(flg, fpt, strObj, strIdx, strTyp, strPst)                                \
            string strFmtFul = " " + strFmt;                                                               \
            for (int idx0 = 0; idx0 < siz0; idx0 += idxDlt0) {                                             \
                if (idx0 == 0)                                                                             \
                    assert(fscanf(fpt,    strFmt.c_str(), &dat[idx0]) == 1);                               \
                else                                                                                       \
                    assert(fscanf(fpt, strFmtFul.c_str(), &dat[idx0]) == 1);                               \
            }                                                                                              \
        }
        #define JPG_FUNC_DUMP_D_2(flg, fpt, strObj, strIdx, strTyp, strPst, dat, datMsk, strFmt, siz1, idxDlt1, siz0, idxDlt0)    \
        {                                                                                                                         \
            JPG_FUNC_INI_FPT_DUMP(flg, fpt, strObj, strIdx, strTyp, strPst)                                                       \
            string strFmtFul = " " + strFmt;                                                                                      \
            for (int idx1 = 0; idx1 < siz1; idx1 += idxDlt1) {                                                                    \
                for (int idx0 = 0; idx0 < siz0; idx0 += idxDlt0) {                                                                \
                    if (strPst == "dat") {                                                                                        \
                        if (idx0 == 0)                                                                                            \
                            fprintf(fpt,    strFmt.c_str(), (int)dat[idx1][idx0] & datMsk);                                       \
                        else                                                                                                      \
                            fprintf(fpt, strFmtFul.c_str(), (int)dat[idx1][idx0] & datMsk);                                       \
                    }                                                                                                             \
                    else {                                                                                                        \
                        fwrite(&dat[idx1][idx0], sizeof(dat[idx1][idx0]), 1, fpt);                                                \
                    }                                                                                                             \
                }                                                                                                                 \
                fprintf(fpt, "\n");                                                                                               \
            }                                                                                                                     \
        }
        #define JPG_FUNC_LOAD_D_2(flg, fpt, strObj, strIdx, strTyp, strPst, dat, strFmt, siz1, idxDlt1, siz0, idxDlt0)    \
        {                                                                                                                 \
            JPG_FUNC_INI_FPT_LOAD(flg, fpt, strObj, strIdx, strTyp, strPst)                                               \
            string strFmtFul = " " + strFmt;                                                                              \
            for (int idx1 = 0; idx1 < siz1; idx1 += idxDlt1) {                                                            \
                for (int idx0 = 0; idx0 < siz0; idx0 += idxDlt0) {                                                        \
                    if (idx0 == 0)                                                                                        \
                        assert(fscanf(fpt,    strFmt.c_str(), &dat[idx1][idx0]) == 1);                                    \
                    else                                                                                                  \
                        assert(fscanf(fpt, strFmtFul.c_str(), &dat[idx1][idx0]) == 1);                                    \
                }                                                                                                         \
            }                                                                                                             \
        }
        #define JPG_FUNC_DUMP_D_3(flg, fpt, strObj, strIdx, strTyp, strPst, dat, datMsk, strFmt, siz2, idxDlt2, siz1, idxDlt1, siz0, idxDlt0)    \
        {                                                                                                                                        \
            JPG_FUNC_INI_FPT_DUMP(flg, fpt, strObj, strIdx, strTyp, strPst)                                                                      \
            string strFmtFul = " " + strFmt;                                                                                                     \
            for (int idx2 = 0; idx2 < siz2; idx2 += idxDlt2) {                                                                                   \
                for (int idx1 = 0; idx1 < siz1; idx1 += idxDlt1) {                                                                               \
                    for (int idx0 = 0; idx0 < siz0; idx0 += idxDlt0) {                                                                           \
                        if (strPst == "dat") {                                                                                                   \
                            if (idx0 == 0)                                                                                                       \
                                fprintf(fpt,    strFmt.c_str(), dat[idx2][idx1][idx0] & datMsk);                                                 \
                            else                                                                                                                 \
                                fprintf(fpt, strFmtFul.c_str(), dat[idx2][idx1][idx0] & datMsk);                                                 \
                        }                                                                                                                        \
                        else {                                                                                                                   \
                            fwrite(&dat[idx2][idx1][idx0], sizeof(dat[idx2][idx1][idx0]), 1, fpt);                                               \
                        }                                                                                                                        \
                    }                                                                                                                            \
                    fprintf(fpt, "\n");                                                                                                          \
                }                                                                                                                                \
            }                                                                                                                                    \
        }
        #define JPG_FUNC_DUMP_D_4(flg, fpt, strObj, strIdx, strTyp, strPst, dat, datMsk, strFmt, siz3, idxDlt3, siz2, idxDlt2, siz1, idxDlt1, siz0, idxDlt0)    \
        {                                                                                                                                                       \
            JPG_FUNC_INI_FPT_DUMP(flg, fpt, strObj, strIdx, strTyp, strPst)                                                                                     \
            string strFmtFul = " " + strFmt;                                                                                                                    \
            for (int idx3 = 0; idx3 < siz3; idx3 += idxDlt3) {                                                                                                  \
                for (int idx2 = 0; idx2 < siz2; idx2 += idxDlt2) {                                                                                              \
                    for (int idx1 = 0; idx1 < siz1; idx1 += idxDlt1) {                                                                                          \
                        for (int idx0 = 0; idx0 < siz0; idx0 += idxDlt0) {                                                                                      \
                            if (strPst == "dat") {                                                                                                              \
                                if (idx0 == 0)                                                                                                                  \
                                    fprintf(fpt,    strFmt.c_str(), dat[idx3][idx2][idx1][idx0] & datMsk);                                                      \
                                else                                                                                                                            \
                                    fprintf(fpt, strFmtFul.c_str(), dat[idx3][idx2][idx1][idx0] & datMsk);                                                      \
                            }                                                                                                                                   \
                            else {                                                                                                                              \
                                fwrite(&dat[idx3][idx2][idx1][idx0], sizeof(dat[idx3][idx2][idx1][idx0]), 1, fpt);                                              \
                            }                                                                                                                                   \
                        }                                                                                                                                       \
                        fprintf(fpt, "\n");                                                                                                                     \
                    }                                                                                                                                           \
                }                                                                                                                                               \
            }                                                                                                                                                   \
        }

    // KNOB
        #define JPG_KNOB_TIME

    // SIZE
        #define JPG_SIZE_PREFIX    4


//--- KERNEL ---------------------------
    // SIZE
        #define JPG_SIZE_FRAME_MAX_X    16384
        #define JPG_SIZE_FRAME_MAX_Y    16384


//--- COMMON ---------------------------
    // FUNC
        #define JPG_FUNC_LOG2(dat)     \
        (     (dat <= 1 <<  0) ?  0    \
            : (dat <= 1 <<  1) ?  1    \
            : (dat <= 1 <<  2) ?  2    \
            : (dat <= 1 <<  3) ?  3    \
            : (dat <= 1 <<  4) ?  4    \
            : (dat <= 1 <<  5) ?  5    \
            : (dat <= 1 <<  6) ?  6    \
            : (dat <= 1 <<  7) ?  7    \
            : (dat <= 1 <<  8) ?  8    \
            : (dat <= 1 <<  9) ?  9    \
            : (dat <= 1 << 10) ? 10    \
            : (dat <= 1 << 11) ? 11    \
            : (dat <= 1 << 12) ? 12    \
            : (dat <= 1 << 13) ? 13    \
            : (dat <= 1 << 14) ? 14    \
            : (dat <= 1 << 15) ? 15    \
            : (dat <= 1 << 16) ? 16    \
            : (dat <= 1 << 17) ? 17    \
            : (dat <= 1 << 18) ? 18    \
            : (dat <= 1 << 19) ? 19    \
            : (dat <= 1 << 20) ? 20    \
            : (dat <= 1 << 21) ? 21    \
            : (dat <= 1 << 22) ? 22    \
            : (dat <= 1 << 23) ? 23    \
            : (dat <= 1 << 24) ? 24    \
            : (dat <= 1 << 25) ? 25    \
            : (dat <= 1 << 26) ? 26    \
            : (dat <= 1 << 27) ? 27    \
            : (dat <= 1 << 28) ? 28    \
            : (dat <= 1 << 29) ? 29    \
            : (dat <= 1 << 30) ? 30    \
            : (dat <= 1 << 31) ? 31    \
            :                    32    \
        )
        #define JPG_FUNC_MIN(datX, datY)             ((datX) < (datY) ? (datX) : (datY))
        #define JPG_FUNC_MAX(datX, datY)             ((datX) > (datY) ? (datX) : (datY))
        #define JPG_FUNC_ROUND(dat, datPrc)          ((int)((dat) / (datPrc) + 0.5f) * (datPrc))
        #define JPG_FUNC_CLP(datMin, datMax, dat)    (((dat) < (datMin)) ? (datMin) : (((dat) > (datMax)) ? (datMax) : (dat)))
        #define JPG_FUNC_CLP_PXL(dat)                JPG_FUNC_CLP(0, ((1 << JPG_DATA_PXL_WD) - 1), (dat))
        #define JPG_FUNC_ABS(dat)                    (((dat) >= 0) ? (dat) : -(dat))

    // NUMB
        #define JPG_NUMB_CHN          3
        #define JPG_NUMB_LAC          (JPG_NUMB_CHN == 1 ? 1 : 2)
        #define JPG_NUMB_MCU_MAX_X    ((JPG_SIZE_FRAME_MAX_X + (JPG_SIZE_MCU_MIN - 1)) / JPG_SIZE_MCU_MIN)
        #define JPG_NUMB_MCU_MAX_Y    ((JPG_SIZE_FRAME_MAX_Y + (JPG_SIZE_MCU_MIN - 1)) / JPG_SIZE_MCU_MIN)
        #define JPG_NUMB_PXL_BLK      (JPG_SIZE_BLK * JPG_SIZE_BLK)
        #define JPG_NUMB_SYMBOL_MAX    256    ///< maximum number of symbols

    // SIZE
        #define JPG_SIZE_MCU        16
        #define JPG_SIZE_MCU_MIN    16
        #define JPG_SIZE_BLK        8
        #define JPG_SIZE_CODE_MAX   16    ///< maximum size of huffman codes

    // DATA
        #define JPG_DATA_PXL_WD        8
        #define JPG_DATA_PXL_MAX       ((1 << (JPG_DATA_PXL_WD)) - 1)
        #define JPG_DATA_PXL_HALF      (1 << (JPG_DATA_PXL_WD - 1))
        #define JPG_DATA_COE_WD_MAX    10
        #define JPG_DATA_COE_MAX_DC    ( (1 << JPG_DATA_COE_WD_MAX      ) - 1)
        #define JPG_DATA_COE_MIN_DC    (-(1 << JPG_DATA_COE_WD_MAX      )    )
        #define JPG_DATA_COE_MAX_AC    ( (1 << (JPG_DATA_COE_WD_MAX - 1)) - 1)
        #define JPG_DATA_COE_MIN_AC    (-(1 << (JPG_DATA_COE_WD_MAX - 1))    )
        #define JPG_DATA_SHF_CH        1
        #define JPG_DATA_QTZ_FRC_WD    16

        #define JPG_DATA_HDR_DQT           0xffdb  ///< constant header for define quantization table
        #define JPG_DATA_HDR_DHT           0xffc4  ///< constant header for define huffman      table
        #define JPG_DATA_HDR_SOI           0xffd8  ///< constant header for start  of image
        #define JPG_DATA_HDR_EOI           0xffd9  ///< constant header for end    of image
        #define JPG_DATA_HDR_SOF0          0xffc0  ///< constant header for start  of frame        (baseline)

        #define JPG_DATA_HDR_SOS           0xffda  ///< constant header for start  of scan
        #define JPG_DATA_WORD_LOS          0x000c  ///< constant word   for length of scan  header
        #define JPG_DATA_WORD_LOF0         0x0011  ///< constant word   for length of frame header (baseline)


//*** TYPEDEF ******************************************************************
//--- COMMON PREDICION -----------------
    // jpg_pxl_t
        #if JPG_DATA_PXL_WD > 8
        typedef uint16_t    jpg_pxl_t;
        #else
        typedef uint8_t     jpg_pxl_t;
        #endif

    // jpg_coe_t
        typedef int16_t    jpg_coe_t;


//*** ENUMERATOR ***************************************************************
//--- UNIT -----------------------------
    // jpg_enmLoad_t
        enum class jpg_enmLoad_t {
            PIPE = 0,
            FILE,
            RANDOM
        };

    // jpg_enmProc_t
        enum class jpg_enmProc_t {
            SOFTWARE = 0,
            HARDWARE
        };

    // jpg_enmInfo_t
        enum class jpg_enmInfo_t {
            NONE = 0,
            TOP,
            KERNEL,
            UNIT,
            COMMON
        };

    // jpg_enmDump_t
        enum class jpg_enmDump_t {
            NONE = 0,
            INOUT,
            INTERMEDIATE
        };

    // jpg_enmModRun_t
        enum class jpg_enmModRun_t {
            ENC = 0,
            DEC
        };

    // jpg_enmFormat_t
        enum class jpg_enmFormat_t {
            YUV420 = 0,
            YUV422
        };

    // jpg_enmCoe_t
        enum class jpg_enmCoe_t {
            DC = 0,
            AC    ,
            NUMB
        };


//*** STRUCT *******************************************************************
//--- TOP ------------------------------
    // jpg_cfg_t
        #include "jpg_cfg_type.hpp"


//*** TABLE ********************************************************************
//--- COMMON TRANSFORMATION ------------
    // jpg_m_datCoeQtz
        extern int jpg_m_datCoeQtz[JPG_NUMB_LAC][JPG_SIZE_BLK][JPG_SIZE_BLK];

    // jpg_m_datCoeQtzZigzag
        extern int jpg_m_datCoeQtzZigzag[JPG_NUMB_LAC][JPG_NUMB_PXL_BLK];


//--- DEFINE HUFFMAN TABLE -------------
    // jpg_m_numSymbolDht
        extern int jpg_m_numSymbolDht[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_SIZE_CODE_MAX  ];

    // jpg_m_datSymbolDht
        extern int jpg_m_datSymbolDht[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_NUMB_SYMBOL_MAX];


//*** INLINE *******************************************************************
//--- COMMON ---------------------------
    #include "jpg_common_tool_general.hpp"

    // getNumBit
        inline uint8_t getNumBit(int dat) {
            int datAbs = dat >= 0 ? dat : -dat;
            return JPG_FUNC_LOG2(datAbs + 1);
        };

    // getDatInMinusOneComplement
        inline int getDatInMinusOneComplement(int dat) {
            return dat >= 0 ? dat : dat - 1;
        };

    // getDatInMinusOneComplement
        inline int getCoeDec(int dat,int siz) {
            if (siz == 0) return 0;
            if (dat < 1 << (siz - 1)) return dat - (1 << siz) + 1;
            return dat;
        };

#endif /* __JPG_COMMON_HPP__ */
