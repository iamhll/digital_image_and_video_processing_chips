//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_c_proc.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_encoding related codes (proc)
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2022-12-09 by HLL, LYZ
    //  Description    : fully tidied up
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_h_c.hpp"


//*** FUNCTION *****************************************************************
// procPrev
void JPG_H_C::procPrev()
{
    // basic
    JPG_UNIT::procPrev();

    static bool flag = 1;
    if (flag) {
        // clear flag
        flag = 0;
        // load quatization map in zig-zag
        for (int idxDqt = 0; idxDqt < JPG_NUMB_LAC; ++idxDqt) {
            int idxZigZag = 0;
            for (int idx = 0; idx <= (JPG_SIZE_BLK - 1) * 2; ++idx) {
                int idxX = 0;
                int idxY = 0;
                if (idx % 2) {
                    for (idxY = 0;idxY <= idx; ++idxY) {
                        idxX = idx - idxY;
                        if (idxX >= JPG_SIZE_BLK || idxY >= JPG_SIZE_BLK)
                            continue;
                        (*m_datCoeQtz)[idxDqt][idxZigZag] = jpg_m_datCoeQtz[idxDqt][idxY][idxX];
                        (*m_datCoeQtzInv)[idxDqt][idxZigZag] = round((double)(1 << JPG_DATA_QTZ_FRC_WD) / jpg_m_datCoeQtz[idxDqt][idxY][idxX]);
                        idxZigZag++;
                    }
                }
                else {
                    for (idxX = 0; idxX <= idx; ++idxX) {
                        idxY = idx - idxX;
                        if (idxX >= JPG_SIZE_BLK || idxY >= JPG_SIZE_BLK)
                            continue;
                        (*m_datCoeQtz)[idxDqt][idxZigZag] = jpg_m_datCoeQtz[idxDqt][idxY][idxX];
                        (*m_datCoeQtzInv)[idxDqt][idxZigZag] = round((double)(1 << JPG_DATA_QTZ_FRC_WD) / jpg_m_datCoeQtz[idxDqt][idxY][idxX]);
                        idxZigZag++;
                    }
                }
            }
        }

        // create huffman table
        memset(*m_numBitDict, 0, sizeof(*m_numBitDict));
        memset(*m_datBitDict, 0, sizeof(*m_datBitDict));
        for (int idxCoeType = 0; idxCoeType < (int)jpg_enmCoe_t::NUMB; ++idxCoeType) {
            for (int idxLaC = 0; idxLaC < m_cfg->numLaC; ++idxLaC) {
                int idxSymbolDht = 0;
                int datBitTemp = 0;
                for (int idxLengthDht = 0; idxLengthDht < JPG_H_C_SIZE_CODE_MAX; ++idxLengthDht) {
                    int nBits = idxLengthDht + 1;
                    int numSymbol = jpg_m_numSymbolDht[idxCoeType][idxLaC][idxLengthDht];
                    int numSymbolcnt = 0;
                    for (numSymbolcnt = 0;numSymbolcnt < numSymbol; ++numSymbolcnt) {
                        int enmCoeCat = jpg_m_datSymbolDht[idxCoeType][idxLaC][idxSymbolDht];
                        (*m_numBitDict)[idxCoeType][idxLaC][enmCoeCat] = nBits;
                        (*m_datBitDict)[idxCoeType][idxLaC][enmCoeCat] = datBitTemp;
                        datBitTemp++;
                        idxSymbolDht++;
                    }
                    datBitTemp = datBitTemp << 1;
                }
            }
        }

        // dumpDatCoeQtz
        // dumpNumBitDict dumpDatBitDict
        if (   (jpg_enmDump_t)m_cfg->enmDump    >= jpg_enmDump_t::INOUT
            || (jpg_enmDump_t)m_cfg->h_cEnmDump >= jpg_enmDump_t::INOUT
            || (jpg_enmDump_t)m_cfg->itfEnmDump >= jpg_enmDump_t::INOUT
        ) {
            for (int idxLaC = 0; idxLaC < JPG_NUMB_LAC; ++idxLaC) {
                dumpDatCoeQtzInv(idxLaC);
            }
            for (int enmCoe = 0; enmCoe < (int)jpg_enmCoe_t::NUMB; ++enmCoe) {
                for (int idxLaC = 0; idxLaC < JPG_NUMB_LAC; ++idxLaC) {
                    dumpNumBitDict(enmCoe,idxLaC);
                    dumpDatBitDict(enmCoe,idxLaC);
                }
            }
        }
    }

    // for the first mcu in each frame
    if (m_cfg->idxMcuX == 0 && m_cfg->idxMcuY == 0 && m_cfg->idxMcuR == 0) {
        // initialize bit stream
        m_numFbsFrame = 0;
        m_numRbsFrame = 0;
        m_numBitFrame = 0;
        memset(m_datFbsFrame, 0, JPG_NUMB_CHN * JPG_SIZE_FRAME_MAX_Y * JPG_SIZE_FRAME_MAX_X * sizeof(jpg_pxl_t));
        memset(m_datRbsFrame, 0, JPG_NUMB_CHN * JPG_SIZE_FRAME_MAX_Y * JPG_SIZE_FRAME_MAX_X * sizeof(jpg_pxl_t));

        // initial prev data
        memset(m_datCoeDcPrev, 0, sizeof(m_datCoeDcPrev));
    }
}

// procCore
void JPG_H_C::procCore()
{
    for (int idxChn = 0; idxChn < JPG_NUMB_CHN; ++idxChn) {
        int idxSubEndY = ((idxChn == 0) && ((jpg_enmFormat_t)m_cfg->enmFormat != jpg_enmFormat_t::YUV422)) ? 1 : 0;
        int idxSubEndX = idxChn == 0 ? 1 : 0;
        for (int idxSubY = 0;idxSubY <= idxSubEndY; ++idxSubY) {
            for (int idxSubX = 0;idxSubX <= idxSubEndX ; ++idxSubX) {
                // zig-zag reorder (start from top-left)
                {
                    int idxZigZag = 0;
                    for (int idx = 0; idx <= (JPG_SIZE_BLK - 1) * 2; ++idx) {
                        int idxX = 0;
                        int idxY = 0;
                        if (idx % 2) {
                            for (idxY = 0; idxY <= idx; ++idxY) {
                                idxX = idx - idxY;
                                if (idxX >= JPG_SIZE_BLK || idxY >= JPG_SIZE_BLK)
                                    continue;
                                int true_idxY = (idxSubY + m_cfg->idxMcuR) * JPG_SIZE_BLK + idxY;
                                int true_idxX = idxSubX * JPG_SIZE_BLK + idxX;
                                (*m_datCoeZigZag)[idxChn][idxZigZag] = (*m_datCoeMcu)[idxChn][true_idxY][true_idxX];
                                idxZigZag++;
                            }
                        }
                        else {
                            for (idxX = 0; idxX <= idx; ++idxX) {
                                idxY = idx - idxX;
                                if (idxX >= JPG_SIZE_BLK || idxY >= JPG_SIZE_BLK)
                                    continue;
                                int true_idxY = (idxSubY + m_cfg->idxMcuR) * JPG_SIZE_BLK + idxY;
                                int true_idxX = idxSubX * JPG_SIZE_BLK + idxX;
                                (*m_datCoeZigZag)[idxChn][idxZigZag] = (*m_datCoeMcu)[idxChn][true_idxY][true_idxX];
                                idxZigZag++;
                            }
                        }
                    }
                }

                // symbol generate
                int numCoeRle = 0;
                {
                    int numZeroRun = 0;
                    for (int idxZigZag = 0; idxZigZag < JPG_NUMB_PXL_BLK; ++idxZigZag) {
                        // DPCM
                        if (idxZigZag == 0) {
                            int datCoeDiff = (*m_datCoeZigZag)[idxChn][idxZigZag] - m_datCoeDcPrev[idxChn];
                            int numBitCoe = getNumBit(datCoeDiff);
                            (*m_enmCoeRle)[idxChn][idxZigZag] = numBitCoe;
                            (*m_datCoeRle)[idxChn][idxZigZag] = getDatInMinusOneComplement(datCoeDiff)  & ((1 << numBitCoe) - 1);
                                numCoeRle++;
                            m_datCoeDcPrev[idxChn] = (*m_datCoeZigZag)[idxChn][idxZigZag];
                        }
                        // RLE
                        else {
                            int datCoeAc = (*m_datCoeZigZag)[idxChn][idxZigZag];
                            int numBitCoe = getNumBit(datCoeAc);
                            if (datCoeAc == 0) {
                                numZeroRun++;
                            }
                            else {
                                for (; numZeroRun >= 16;) {
                                    numZeroRun -= 16;
                                    (*m_enmCoeRle)[idxChn][numCoeRle] = JPG_H_C_DATA_SYMBOL_ZRL;
                                    (*m_datCoeRle)[idxChn][numCoeRle] = 0;
                                    numCoeRle++;
                                }
                                (*m_enmCoeRle)[idxChn][numCoeRle] = (numZeroRun << JPG_H_C_DATA_SYMBOL_SIZ_WD) + numBitCoe;
                                (*m_datCoeRle)[idxChn][numCoeRle] = getDatInMinusOneComplement(datCoeAc) & ((1 << numBitCoe) - 1);
                                numZeroRun = 0;
                                numCoeRle++;
                            }
                        }
                    }
                    if (numZeroRun > 0) {
                        (*m_enmCoeRle)[idxChn][numCoeRle] = JPG_H_C_DATA_SYMBOL_EOB;
                        (*m_datCoeRle)[idxChn][numCoeRle] = 0;
                        numCoeRle++;
                    }
                }

                // huffman encoding
                {
                    for (int idxCoeRle = 0; idxCoeRle < numCoeRle; ++idxCoeRle)
                    {
                        int idxLaC = idxChn == 0 ? 0 : 1;
                        int enmCoe = idxCoeRle == 0 ? (int)jpg_enmCoe_t::DC : (int)jpg_enmCoe_t::AC;
                        int enmCoeRle = (*m_enmCoeRle)[idxChn][idxCoeRle];
                        int datCoeRle = (*m_datCoeRle)[idxChn][idxCoeRle];
                        int numBitCoeRle = enmCoeRle % (1 << JPG_H_C_DATA_SYMBOL_SIZ_WD);
                        int numBit = (*m_numBitDict)[enmCoe][idxLaC][enmCoeRle];
                        int datBit = (*m_datBitDict)[enmCoe][idxLaC][enmCoeRle];
                        numBit += numBitCoeRle;
                        datBit = (datBit << numBitCoeRle) + datCoeRle;
                        addBody(datBit, numBit);
                    }
                }
            }
        }
    }
}

// procPost
void JPG_H_C::procPost()
{
    // basic
    JPG_UNIT::procPost();

    // for the last mcu in each frame
    if (m_cfg->idxMcuX == m_cfg->numMcuX - 1 && m_cfg->idxMcuY == m_cfg->numMcuY - 1 && m_cfg->idxMcuR == m_cfg->numMcuR - 1) {
        // stuff
        if (m_numBitFrame % 8 > 0) {
            int lenStuff = 8 - m_numBitFrame % 8;
            int datStuff = (1 << lenStuff) - 1;
            addBody(datStuff, lenStuff);
        }
        m_numRbsFrame = m_numBitFrame >> 3 ;

        //!!! START OF FBS
        // add header
        addHeader();
        memcpy(m_datFbsFrame + m_numFbsFrame, m_datRbsFrame, m_numRbsFrame);
        m_numFbsFrame += m_numRbsFrame;

        // EOI
        setDat16(JPG_DATA_HDR_EOI);
        //!!! END OF FBS
    }
}
