//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_d.hpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_decoding related headers
    //
//------------------------------------------------------------------------------

#ifndef __JPG_H_D_HPP__
#define __JPG_H_D_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_unit.hpp"


//*** DEFINE *******************************************************************
    // NUMB
        #define JPG_H_D_NUMB_SYMBOL_MAX    JPG_H_D_DATA_SYMBOL_MAX    ///< maximum number of symbols

    // SIZE
        #define JPG_H_D_SIZE_CODE_MAX           16    ///< maximum size of huffman codes
        #define JPG_H_D_SIZE_SHIFTER_THRESH     JPG_H_D_SIZE_CODE_MAX * 2              ///< maximum size of RLE unit in bits
        #define JPG_H_D_SIZE_SHIFTER            JPG_H_D_SIZE_SHIFTER_THRESH * 2 / 8    ///< maximum size of shifter in byte

    // DATA
        #define JPG_H_D_DATA_SYMBOL_WD         (JPG_H_D_DATA_SYMBOL_SIZ_WD    \
                                              + JPG_H_D_DATA_SYMBOL_RUN_WD    \
                                               )                                    ///<   data width of symbols
        #define JPG_H_D_DATA_SYMBOL_RUN_WD     4                                    ///<   data width of symbols (run  part)
        #define JPG_H_D_DATA_SYMBOL_SIZ_WD     4                                    ///<   data width of symbols (size part)
        #define JPG_H_D_DATA_SYMBOL_MAX        (1 << JPG_H_D_DATA_SYMBOL_WD)        ///< maximum data of symbols

        #define JPG_H_D_DATA_SYMBOL_ZRL        0xf0    ///< constant symbol for zero run length (16 zeros)
        #define JPG_H_D_DATA_SYMBOL_EOB        0x00    ///< constant symbol for end of block

        #define JPG_H_D_DATA_BYTE_HDR_BGN      0xff    ///< constant byte for the start for header words
        #define JPG_H_D_DATA_BYTE_ESCAPE       0x00    ///< constant byte for escape code in raw bitstream

        #define JPG_H_D_DATA_HDR_DQT           0xffdb  ///< constant header for define quantization table
        #define JPG_H_D_DATA_HDR_DHT           0xffc4  ///< constant header for define huffman      table
        #define JPG_H_D_DATA_HDR_SOI           0xffd8  ///< constant header for start  of image
        #define JPG_H_D_DATA_HDR_EOI           0xffd9  ///< constant header for end    of image
        #define JPG_H_D_DATA_HDR_SOF0          0xffc0  ///< constant header for start  of frame        (baseline)

        #define JPG_H_D_DATA_HDR_SOS           0xffda  ///< constant header for start  of scan

        #define JPG_H_D_DATA_WORD_LOS          0x000c  ///< constant word   for length of scan  header
        #define JPG_H_D_DATA_WORD_LOF0         0x0011  ///< constant word   for length of frame header (baseline)

//*** CLASS ********************************************************************
// JPG_H_D
class JPG_H_D: public JPG_UNIT
{
private:

    // information (decoder)
        int      m_numPxlBlk;

        ///                        the quantized coefficients in zig-zag order
        ///                        [category index  ]
        jpg_coe_t (*m_datCoeZigZag)[JPG_NUMB_PXL_BLK];

    // pixel (rbs, fbs)
        uint8_t *m_datRbsFrame;
        int      m_idxRbsFrame;
        uint64_t m_datShifter;
        int      m_sizShifter;

    // information (qtz)
        ///                        the quantization coefficients in zigzag order
        jpg_coe_t (*m_datCoeQtzInv)[JPG_NUMB_LAC ][JPG_NUMB_PXL_BLK];

    // information (dict)
        ///                           threshold value of codes of huffman encoder
        ///                           [AC/DC                  ][lac         ][code length          ]
        uint16_t (*m_datBitDictThresh)[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_D_SIZE_CODE_MAX];

        ///                           threshold index of codes of huffman encoder
        ///                           [AC/DC                  ][lac         ][code length          ]
        int      (*m_idxBitDictThresh)[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_H_D_SIZE_CODE_MAX];

        ///                     DC coefficient data of previous block
        ///                     [block channel]
        jpg_coe_t m_datCoeDcPrev[JPG_NUMB_CHN ];


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

        void dumpDatCoeQtz(int idxLaC); bool m_dumpDatCoeQtzFlg[2] = {0}; FILE *m_dumpDatCoeQtzFpt[2];
        void dumpDatBitDictThresh(int enmCoe, int idxLaC); bool m_dumpDatBitDictThreshFlg[4] = {0}; FILE *m_dumpDatBitDictThreshFpt[4];
        void dumpIdxBitDictThresh(int enmCoe, int idxLaC); bool m_dumpIdxBitDictThreshFlg[4] = {0}; FILE *m_dumpIdxBitDictThreshFpt[4];

    // tool (body)
        void addShifter();
        void rmvShifter(int numBits);
        void decBody(int idxChn, int idxSubX, int idxSubY);


//--- PRIVATE INLINE ------------------
private:
    // setDat8


//--- PUBLIC FUNCTION ------------------
public:
    // basic
        JPG_H_D();
        ~JPG_H_D();
        virtual void connect(jpg_cfg_t &cfg, JPG_FTH &fth, JPG_DMP &dmp, uint8_t *datRbsFrame);


//--- PUBLIC INLINE --------------------
public:
    // cpyNumRbsFrame

    // cpyNumFbsFrame

};

#endif /* __JPG_H_D_HPP__ */
