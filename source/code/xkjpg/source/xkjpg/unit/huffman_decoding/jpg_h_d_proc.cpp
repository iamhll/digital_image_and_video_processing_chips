//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_h_d_proc.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : huffman_decoding related codes (proc)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_h_d.hpp"


//*** FUNCTION *****************************************************************
// procPrev
void JPG_H_D::procPrev()
{
    // basic
    JPG_UNIT::procPrev();

    static bool flag = 1;
    if (flag) {
        // clear flag
        flag = 0;
    }

    // for the first mcu in each frame
    if (m_cfg->idxMcuX == 0 && m_cfg->idxMcuY == 0 && m_cfg->idxMcuR == 0) {
        // initial prev data
        memset(m_datCoeDcPrev, 0, sizeof(m_datCoeDcPrev));

        //!!! load dqt
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
                        jpg_m_datCoeQtz[idxDqt][idxY][idxX] = jpg_m_datCoeQtzZigzag[idxDqt][idxZigZag];
                        (*m_datCoeQtzInv)[idxDqt][idxZigZag] = (int)floor((1 << JPG_DATA_QTZ_FRC_WD) / (double)jpg_m_datCoeQtz[idxDqt][idxY][idxX]);
                        idxZigZag++;
                    }
                }
                else {
                    for (idxX = 0; idxX <= idx; ++idxX) {
                        idxY = idx - idxX;
                        if (idxX >= JPG_SIZE_BLK || idxY >= JPG_SIZE_BLK)
                            continue;
                        jpg_m_datCoeQtz[idxDqt][idxY][idxX] = jpg_m_datCoeQtzZigzag[idxDqt][idxZigZag];
                        (*m_datCoeQtzInv)[idxDqt][idxZigZag] = (int)floor((1 << JPG_DATA_QTZ_FRC_WD) / (double)jpg_m_datCoeQtz[idxDqt][idxY][idxX]);
                        idxZigZag++;
                    }
                }
            }
        }

        // create huffman table
        memset(*m_datBitDictThresh, 0, sizeof(*m_datBitDictThresh));
        memset(*m_idxBitDictThresh, 0, sizeof(*m_idxBitDictThresh));
        for (int idxCoeType = 0; idxCoeType < (int)jpg_enmCoe_t::NUMB; ++idxCoeType) {
            for (int idxLaC = 0; idxLaC < m_cfg->numLaC; ++idxLaC) {
                int idxSymbolDht = 0;
                int datBitTemp = 0;
                for (int idxLengthDht = 0; idxLengthDht < JPG_H_D_SIZE_CODE_MAX; ++idxLengthDht) {
                    datBitTemp += jpg_m_numSymbolDht[idxCoeType][idxLaC][idxLengthDht];
                    idxSymbolDht += jpg_m_numSymbolDht[idxCoeType][idxLaC][idxLengthDht];
                    (*m_datBitDictThresh)[idxCoeType][idxLaC][idxLengthDht] = datBitTemp;
                    (*m_idxBitDictThresh)[idxCoeType][idxLaC][idxLengthDht] = idxSymbolDht;
                    datBitTemp = datBitTemp << 1;
                }
            }
        }

        m_idxRbsFrame = 0;
        m_datShifter  = 0;
        m_sizShifter  = 0;

        // dumpDatCoeQtz
        // dumpDatBitDictThresh, dumpIdxBitDictThresh
        if (   (jpg_enmDump_t)m_cfg->enmDump    >= jpg_enmDump_t::INOUT
            || (jpg_enmDump_t)m_cfg->itfEnmDump >= jpg_enmDump_t::INOUT
        ) {
            for (int idxLaC = 0; idxLaC < JPG_NUMB_LAC; ++idxLaC) {
                dumpDatCoeQtz(idxLaC);
            }
            for (int enmCoe = 0; enmCoe < (int)jpg_enmCoe_t::NUMB; ++enmCoe) {
                for (int idxLaC = 0; idxLaC < JPG_NUMB_LAC; ++idxLaC) {
                    dumpDatBitDictThresh(enmCoe,idxLaC);
                    dumpIdxBitDictThresh(enmCoe,idxLaC);
                }
            }
        }
    }
}

// procCore
void JPG_H_D::procCore()
{
    for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn) {
        int idxLaC = idxChn == 0 ? 0 : 1;
        int idxSubEndX = idxLaC == 0 ? 2 : 1;
        int idxSubEndY = ((idxChn == 0) && ((jpg_enmFormat_t)m_cfg->enmFormat != jpg_enmFormat_t::YUV422)) ? 2 : 1;
        for (int idxSubY = 0; idxSubY < idxSubEndY; ++idxSubY) {
            for (int idxSubX = 0; idxSubX < idxSubEndX; ++idxSubX) {
                memset(*m_datCoeZigZag, 0, sizeof(*m_datCoeZigZag));

                for (m_numPxlBlk = 0; m_numPxlBlk < JPG_NUMB_PXL_BLK;) {
                    if ((m_sizShifter < JPG_H_D_SIZE_SHIFTER_THRESH)) {
                        addShifter();
                    }
                    decBody(idxChn, idxSubX, idxSubY);
                }

                int idxZigZag = 0;
                for (int idx = 0; idx <= (JPG_SIZE_BLK - 1) * 2; ++idx) {
                    int idxX = 0;
                    int idxY = 0;
                    if (idx % 2) {
                        for (idxY = 0;idxY <= idx; ++idxY) {
                            idxX = idx - idxY;
                            if (idxX >= JPG_SIZE_BLK || idxY >= JPG_SIZE_BLK)
                                continue;
                            (*m_datCoeMcu)[idxChn][idxY + (idxSubY+m_cfg->idxMcuR) * JPG_SIZE_BLK][idxX + idxSubX * JPG_SIZE_BLK] = (*m_datCoeZigZag)[idxZigZag];
                            idxZigZag++;
                        }
                    }
                    else {
                        for (idxX = 0; idxX <= idx; ++idxX) {
                            idxY = idx - idxX;
                            if (idxX >= JPG_SIZE_BLK || idxY >= JPG_SIZE_BLK)
                                continue;
                            (*m_datCoeMcu)[idxChn][idxY + (idxSubY+m_cfg->idxMcuR) * JPG_SIZE_BLK][idxX + idxSubX * JPG_SIZE_BLK] = (*m_datCoeZigZag)[idxZigZag];
                            idxZigZag++;
                        }
                    }
                }
            }
        }
    }
}

// procPost
void JPG_H_D::procPost()
{
    // basic
    JPG_UNIT::procPost();

    // for the last mcu in each frame
    if (m_cfg->idxMcuX == m_cfg->numMcuX - 1 && m_cfg->idxMcuY == m_cfg->numMcuY - 1 && m_cfg->idxMcuR == m_cfg->numMcuR - 1) {
    }
}
