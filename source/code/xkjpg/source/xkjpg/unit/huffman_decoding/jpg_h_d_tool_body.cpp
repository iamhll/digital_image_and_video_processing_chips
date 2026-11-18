//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_d_tool_body.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_encoding related codes (encoding tool)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_h_d.hpp"


//*** FUNCTION *****************************************************************
// addShifter
void JPG_H_D::addShifter()
{
    // variables

    // main body
    for (int idxByte = 0; idxByte < JPG_H_D_SIZE_SHIFTER_THRESH >> 3; ++idxByte) {
        int datByte = 0;
        datByte = m_datRbsFrame[m_idxRbsFrame];
        assert(datByte == m_datRbsFrame[m_idxRbsFrame]);
        m_idxRbsFrame++;
        m_datShifter = (m_datShifter << 8) + datByte;
        // escape code
        if (datByte == JPG_H_D_DATA_BYTE_HDR_BGN) {
            m_idxRbsFrame++;
        }
    }
    m_sizShifter += JPG_H_D_SIZE_SHIFTER_THRESH;
};

// rmvShifter
void JPG_H_D::rmvShifter(int numBits)
{
    // variables
    uint64_t datRmv = m_datShifter >> (m_sizShifter - numBits);
    m_datShifter -= datRmv << (m_sizShifter - numBits);
    m_sizShifter -= numBits;

};

// remove bits in shifter
// huffman decode
void JPG_H_D::decBody(int idxChn, int idxSubX, int idxSubY)
{
    // variables
    jpg_enmCoe_t enmCoe = m_numPxlBlk == 0 ? jpg_enmCoe_t::DC : jpg_enmCoe_t::AC;
    int datCategory = 0;
    int idxLaC = idxChn == 0 ? 0 : 1;
    uint32_t datBitTemp = 0;
    int sizBitTemp = 0;
    int sizHuffmanCode = 0;
    int datHuffmanCode = 0;
    // main body
    assert(m_sizShifter >= JPG_H_D_SIZE_SHIFTER_THRESH);
    sizBitTemp += JPG_H_D_SIZE_SHIFTER_THRESH;
    datBitTemp = m_datShifter >> (m_sizShifter - JPG_H_D_SIZE_SHIFTER_THRESH);
    // huffman decode
    {
        uint32_t datCodeThreshPrev = 0;
        int      idxCodeThreshPrev = 0;
        for (int idxLengthDht = 0; idxLengthDht < JPG_H_D_SIZE_CODE_MAX; ++idxLengthDht) {
            int nBits = idxLengthDht + 1;
            uint32_t datCodeTemp = datBitTemp >> (JPG_H_D_SIZE_SHIFTER_THRESH - nBits);
            if (datCodeTemp < (*m_datBitDictThresh)[(int)enmCoe][idxLaC][idxLengthDht]) {
                int idxPick = datCodeTemp - (datCodeThreshPrev << 1) + idxCodeThreshPrev;
                datCategory    = jpg_m_datSymbolDht[(int)enmCoe][idxLaC][idxPick];
                sizHuffmanCode = nBits;
                datHuffmanCode = datCodeTemp;
                break;
            }
            datCodeThreshPrev = (*m_datBitDictThresh)[(int)enmCoe][idxLaC][idxLengthDht];
            idxCodeThreshPrev = (*m_idxBitDictThresh)[(int)enmCoe][idxLaC][idxLengthDht];
        }
    }

    sizBitTemp -= sizHuffmanCode;
    datBitTemp -= datHuffmanCode << sizBitTemp;

    // write in 
    int sizCoeQtz = datCategory % (1 << JPG_H_D_DATA_SYMBOL_SIZ_WD);
    int numRunLenth = datCategory >> JPG_H_D_DATA_SYMBOL_SIZ_WD;
    if (datCategory == JPG_H_D_DATA_SYMBOL_ZRL && m_numPxlBlk > 0) {
        m_numPxlBlk += 1 << JPG_H_D_DATA_SYMBOL_RUN_WD;
    }
    else if (datCategory == JPG_H_D_DATA_SYMBOL_EOB && m_numPxlBlk > 0) {
        m_numPxlBlk = JPG_NUMB_PXL_BLK;
    }
    else {
        int datCoeQtz = getCoeDec(datBitTemp >> (sizBitTemp - sizCoeQtz),sizCoeQtz);
        if (m_numPxlBlk == 0) {
            datCoeQtz += m_datCoeDcPrev[idxChn];
            m_datCoeDcPrev[idxChn] = datCoeQtz;
        }
        (*m_datCoeZigZag)[ m_numPxlBlk + numRunLenth] = datCoeQtz;
        m_numPxlBlk += numRunLenth + 1;
        sizBitTemp -= sizCoeQtz;
        datBitTemp -= datCoeQtz << sizBitTemp;
    }

    // remove bits from shifter
    int sizRmv = sizHuffmanCode + sizCoeQtz;
    rmvShifter(sizRmv);
};
