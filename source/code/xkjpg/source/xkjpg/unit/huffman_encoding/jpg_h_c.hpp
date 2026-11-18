//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_c.hpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_encoding related headers
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2022-12-09 by HLL, LYZ
    //  Description    : fully tidied up
    //
//------------------------------------------------------------------------------

#ifndef __JPG_H_C_HPP__
#define __JPG_H_C_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_unit.hpp"


//*** DEFINE *******************************************************************
    // NUMB
        #define JPG_H_C_NUMB_SYMBOL_MAX    JPG_H_C_DATA_SYMBOL_MAX    ///< maximum number of symbols

    // SIZE
        #define JPG_H_C_SIZE_CODE_MAX    16    ///< maximum size of huffman codes

    // DATA
        #define JPG_H_C_DATA_SYMBOL_WD         (JPG_H_C_DATA_SYMBOL_SIZ_WD    \
                                              + JPG_H_C_DATA_SYMBOL_RUN_WD    \
                                               )                                    ///<   data width of symbols
        #define JPG_H_C_DATA_SYMBOL_RUN_WD     4                                    ///<   data width of symbols (run  part)
        #define JPG_H_C_DATA_SYMBOL_SIZ_WD     4                                    ///<   data width of symbols (size part)
        #define JPG_H_C_DATA_SYMBOL_MAX        (1 << JPG_H_C_DATA_SYMBOL_WD)        ///< maximum data of symbols

        #define JPG_H_C_DATA_SYMBOL_ZRL        0xf0    ///< constant symbol for zero run length (16 zeros)
        #define JPG_H_C_DATA_SYMBOL_EOB        0x00    ///< constant symbol for end of block

        #define JPG_H_C_DATA_BYTE_HDR_BGN      0xff    ///< constant byte for the start for header words
        #define JPG_H_C_DATA_BYTE_ESCAPE       0x00    ///< constant byte for escape code in raw bitstream

        #define JPG_H_C_DATA_WORD_LOS          0x000c  ///< constant word   for length of scan  header
        #define JPG_H_C_DATA_WORD_LOF0         0x0011  ///< constant word   for length of frame header (baseline)


//*** CLASS ********************************************************************
// JPG_H_C
class JPG_H_C: public JPG_UNIT
{
public:
    // connection
        JPG_UNIT *m_dct;

    // pixel (coe)
        ///                        the result coefficients in zigzag order
        ///                        [block channel][sequence index  ]
        jpg_coe_t (*m_datCoeZigZag)[JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK];

        ///                     DC coefficient data of previous block
        ///                     [block channel]
        jpg_coe_t m_datCoeDcPrev[JPG_NUMB_CHN ];

        ///                   the enumerator(catefories) of coefficients after DPCM(DC) & RLE(AC)
        ///                   [block channel][category index  ]
        uint8_t (*m_enmCoeRle)[JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK];

        ///                     the value of coefficients after DPCM(DC) & RLE(AC)
        ///                     [block channel][category index  ]
        jpg_coe_t (*m_datCoeRle)[JPG_NUMB_CHN ][JPG_NUMB_PXL_BLK];

    // pixel (rbs, fbs)
        // byte number of one frame's                    pixel data after  rbsp packing, updated for each frame
        int      m_numRbsFrame;
        // bit  number of one frame's                    pixel data after  rbsp packing, updated for each frame
        int      m_numBitFrame;
        // byte number of one frame's vps, sps, pps, sh, pixel data after  fbsp packing, updated for each frame
        int      m_numFbsFrame;

        uint8_t *m_datRbsFrame;
        uint8_t *m_datFbsFrame;

    // information (qtz)
        ///                        the quantization coefficients in zigzag order
        ///                        [luma / chroma][sequence index  ]
        jpg_coe_t (*m_datCoeQtz)   [JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK];
        ///                        [luma / chroma][sequence index  ]
        jpg_coe_t (*m_datCoeQtzInv)[JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK];

    // information (dict)
        ///                     number of bits of huffman encoder
        ///                     [AC/DC                  ][lac         ][symbol value           ]
        uint16_t (*m_numBitDict)[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_C_NUMB_SYMBOL_MAX];

        ///                     value of codes of huffman encoder
        ///                     [AC/DC                  ][lac         ][symbol value           ]
        uint16_t (*m_datBitDict)[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_C_NUMB_SYMBOL_MAX];


//--- PRIVATE FUNCTION -----------------
private:
    // load
        virtual void loadPipe();

    // proc
        virtual void procPrev();
        virtual void procCore();
        virtual void procPost();

    // dump
        void dumpFile();
        void dumpNumRbsFrame(); bool m_dumpNumRbsFrameFlg = 0; FILE *m_dumpNumRbsFrameFpt;
        void dumpDatRbsFrame(); bool m_dumpDatRbsFrameFlg = 0; FILE *m_dumpDatRbsFrameFpt;

        void dumpDatCoeQtzInv(int idxLaC); bool m_dumpDatCoeQtzInvFlg[2] = {0}; FILE *m_dumpDatCoeQtzInvFpt[2];
        void dumpNumBitDict(int enmCoe, int idxLaC); bool m_dumpNumBitDictFlg[4] = {0}; FILE *m_dumpNumBitDictFpt[4];
        void dumpDatBitDict(int enmCoe, int idxLaC); bool m_dumpDatBitDictFlg[4] = {0}; FILE *m_dumpDatBitDictFpt[4];

    // tool (header)
        void addHeader();
        void addHeaderDqt();
        void addHeaderDht();
        void addHeaderFrame();
        void addHeaderScan();

    // tool (body)
        void addBody(int datBit, int numBit);


//--- PRIVATE INLINE ------------------
private:
    // setDat8
        inline void setDat8(uint8_t dat8) {
            m_datFbsFrame[m_numFbsFrame] = dat8;
            m_numFbsFrame += 1;
            return;
        };

    // setDat16
        inline void setDat16(uint16_t dat16) {
            uint8_t datTmp0 = dat16 >> 8;
            uint8_t datTmp1 = dat16 % (1 << 8);
            m_datFbsFrame[m_numFbsFrame]     = datTmp0;
            m_datFbsFrame[m_numFbsFrame + 1] = datTmp1;
            m_numFbsFrame += 2;
            return;
        };


//--- PUBLIC FUNCTION ------------------
public:
    // basic
        JPG_H_C();
        ~JPG_H_C();
        virtual void connect(jpg_cfg_t &cfg, JPG_FTH &fth, JPG_DMP &dmp, JPG_UNIT &dct, uint8_t *datRbsFrame, uint8_t *datFbsFrame);


//--- PUBLIC INLINE --------------------
public:
    // cpyNumRbsFrame
        inline void cpyNumRbsFrame(void *dst)
        {
            memcpy(dst, &m_numRbsFrame, sizeof(m_numRbsFrame));
        };

    // cpyNumFbsFrame
        inline void cpyNumFbsFrame(void *dst)
        {
            memcpy(dst, &m_numFbsFrame, sizeof(m_numFbsFrame));
        };
};

#endif /* __JPG_H_C_HPP__ */
