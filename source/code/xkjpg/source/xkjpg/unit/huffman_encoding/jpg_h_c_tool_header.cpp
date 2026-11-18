//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_c_tool_header.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_encoding related codes (header tool)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_h_c.hpp"


//*** FUNCTION *****************************************************************
// addHeader
void JPG_H_C::addHeader()
{
    setDat16(JPG_DATA_HDR_SOI);    // SOI
    addHeaderDqt();
    addHeaderDht();
    addHeaderFrame();
    addHeaderScan();
};

// addHeaderDqt
void JPG_H_C::addHeaderDqt()
{
    int numDqt = 2;
    setDat16(JPG_DATA_HDR_DQT);    // DQT
    setDat16(2 + (1 + JPG_NUMB_PXL_BLK) * numDqt);    // length
    for (int idxDqt = 0; idxDqt < numDqt; ++idxDqt) {
        setDat8(idxDqt);    // idx
        for (int idxZigZag = 0; idxZigZag < JPG_NUMB_PXL_BLK; ++idxZigZag) {
            setDat8((*m_datCoeQtz)[idxDqt][idxZigZag]);    // dat
        }
    }
};

// addHeaderDht
void JPG_H_C::addHeaderDht()
{
    setDat16(JPG_DATA_HDR_DHT);    // DHT

    int sizDht = 0;
    for (int enmCoe = 0; enmCoe < (int)jpg_enmCoe_t::NUMB; ++enmCoe) {
        for (int idxLaC = 0; idxLaC < JPG_NUMB_LAC; ++idxLaC) {
            for (int i = 0; i < JPG_SIZE_CODE_MAX; ++i) {
                sizDht += jpg_m_numSymbolDht[enmCoe][idxLaC][i];
            }
        }
    }

    setDat16(2
        +    (1 + JPG_H_C_SIZE_CODE_MAX) * (int)jpg_enmCoe_t::NUMB * m_cfg->numLaC
        +    sizDht
    );    // length: 2 + 4 idx + 16 * 4 + sizDht

    for (int idxEnmCoe = 0; idxEnmCoe < (int)jpg_enmCoe_t::NUMB; ++idxEnmCoe) {
        for (int idxLaC = 0; idxLaC < m_cfg->numLaC; ++idxLaC) {
            setDat8((idxEnmCoe << 4) + idxLaC);    // idx
            int numSymbDht = 0;
            for(int lenSymbol = 0; lenSymbol < JPG_H_C_SIZE_CODE_MAX; lenSymbol++) {
                setDat8(jpg_m_numSymbolDht[idxEnmCoe][idxLaC][lenSymbol]);
                numSymbDht += jpg_m_numSymbolDht[idxEnmCoe][idxLaC][lenSymbol];    // num
            }
            for(int idxSymbolDht = 0; idxSymbolDht < numSymbDht; ++idxSymbolDht)
                setDat8(jpg_m_datSymbolDht[idxEnmCoe][idxLaC][idxSymbolDht]);    // dat
        }
    }
};

// addHeaderFrame
void JPG_H_C::addHeaderFrame()
{
    setDat16(JPG_DATA_HDR_SOF0);     // SOF
    setDat16(JPG_H_C_DATA_WORD_LOF0);    // LOF
    setDat8(JPG_DATA_PXL_WD);            // P
    setDat16(m_cfg->trueSizFrameY);      // Y
    setDat16(m_cfg->trueSizFrameX);      // X
    setDat8(m_cfg->numChn);              // Nf
    switch ((jpg_enmFormat_t)m_cfg->enmFormat) {
    case jpg_enmFormat_t::YUV420:
        setDat16(0x0122);                    // C(Y), H(2), V(2)
        setDat8(0x00);                       // Tq (of Y)
        setDat16(0x0211);                    // C(U), H(1), V(1)
        setDat8(0x01);                       // Tq (of U)
        setDat16(0x0311);                    // C(V), H(1), V(1)
        setDat8(0x01);                       // Tq (of V)
    break;
    case jpg_enmFormat_t::YUV422:
        setDat16(0x0121);                    // C(Y), H(2), V(2)
        setDat8(0x00);                       // Tq (of Y)
        setDat16(0x0211);                    // C(U), H(1), V(2)
        setDat8(0x01);                       // Tq (of U)
        setDat16(0x0311);                    // C(V), H(1), V(2)
        setDat8(0x01);                       // Tq (of V)
    break;
    }
};

// addHeaderScan
void JPG_H_C::addHeaderScan()
{
    setDat16(JPG_DATA_HDR_SOS);     // SOS
    setDat16(JPG_H_C_DATA_WORD_LOS);    // LOS
    setDat8(m_cfg->numChn);             // Ns
    setDat16(0x0100);                   // C(Y), idxDc, idxAc
    setDat16(0x0211);                   // C(U), idxDc, idxAc
    setDat16(0x0311);                   // C(V), idxDc, idxAc
    setDat8(0x00);                      // Ss
    setDat8(0x3f);                      // Se
    setDat8(0x00);                      // Ah Al
};
