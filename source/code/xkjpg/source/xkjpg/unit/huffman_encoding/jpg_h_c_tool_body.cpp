//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_c_tool_body.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_encoding related codes (encoding tool)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_h_c.hpp"


//*** FUNCTION *****************************************************************
// add bits to the encode buffer by bytes
// input  : datBit, numBit
// output : m_datRbsFrame, m_numBitFrame
void JPG_H_C::addBody(int datBit, int numBit)
{
    // variables
    int numBitTemp = numBit;
    int numBitPrev = m_numBitFrame % 8;
    int idxByteBgn = m_numBitFrame >> 3;
    int idxByteEnd = (m_numBitFrame + numBit) >> 3;
    int idxByte    = 0;
    int datBitTemp = datBit;

    // main body
    m_numBitFrame += numBit;
    for (idxByte = idxByteBgn; idxByte <= idxByteEnd; ++idxByte) {
        int numBitWrite = 8 - numBitPrev;
        int datBitWrite = datBitTemp >> (numBitTemp - numBitWrite);
        if (idxByte < idxByteEnd)
            m_datRbsFrame[idxByte] = (m_datRbsFrame[idxByte] << numBitWrite) + datBitWrite;
        else
            m_datRbsFrame[idxByte] = (m_datRbsFrame[idxByte] << numBitTemp ) + datBitTemp ;
        if (m_datRbsFrame[idxByte] == JPG_H_C_DATA_BYTE_HDR_BGN) {
            m_numBitFrame += 8;
            m_datRbsFrame[idxByte + 1] = JPG_H_C_DATA_BYTE_ESCAPE;
            idxByte++;
            idxByteEnd++;
        }
        numBitPrev = 0;
        numBitTemp -= numBitWrite;
        datBitTemp -= datBitWrite << numBitTemp;
    }
};
