//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_dct_proc.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : discrete_cosine_transform related codes (proc)
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2022-12-09 by HLL, LYZ
    //  Description    : fully tidied up
    //  Modified       : 2022-06-07 by LYZ
    //  Description    : DCT coeffcients optimized
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_dct.hpp"


//*** FUNCTION *****************************************************************
// procPrev
void JPG_DCT::procPrev()
{
    // basic
    JPG_UNIT::procPrev();

    // for the first mcu
    static bool flag = 1;
    if (flag) {
        // clear flag
        flag = 0;

        // initialize m_datCoeTsf08
        for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
            for (int idxX = 0; idxX < JPG_SIZE_BLK; ++idxX) {
                double datAmpCoeY = idxY == 0 ? sqrt(1.0 / JPG_SIZE_BLK) : sqrt(2.0 / JPG_SIZE_BLK);
                (*m_datCoeTsf08)[idxY][idxX] = datAmpCoeY * cos(M_PI * (2 * idxX + 1) * idxY / (2 * JPG_SIZE_BLK));
                if (m_cfg->enmProc == (int)jpg_enmProc_t::HARDWARE)
                    (*m_datCoeTsf08)[idxY][idxX] = jpg_getDatRound((*m_datCoeTsf08)[idxY][idxX], JPG_DCT_DATA_DCT_FRC_WD);
            }
        }

        // initialize m_datCoeQtzInv
        for (int idxChn = 0; idxChn < JPG_NUMB_CHN; ++idxChn) {
            for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                for (int idxX = 0; idxX < JPG_SIZE_BLK; ++idxX) {
                    int idxLaC = idxChn == 0 ? 0 : 1;
                    (*m_datCoeQtzInv)[idxChn][idxY][idxX] = 1.0 / jpg_m_datCoeQtz[idxLaC][idxY][idxX];
                    if (m_cfg->enmProc == (int)jpg_enmProc_t::HARDWARE)
                        (*m_datCoeQtzInv)[idxChn][idxY][idxX] = jpg_getDatRound((*m_datCoeQtzInv)[idxChn][idxY][idxX], JPG_DATA_QTZ_FRC_WD);
                }
            }
        }
    }

    //// for the first mcu in each frame
    //if (m_cfg->idxMcuX == 0 && m_cfg->idxMcuY == 0) {
    //    //...
    //}

    // for each mcu
    {
        // upscale
        switch ((jpg_enmFormat_t)m_cfg->enmFormat) {
        case jpg_enmFormat_t::YUV420:
            memcpy(*m_true_datOriMcu, *m_datOriMcu, sizeof(*m_datOriMcu));
        break;
        case jpg_enmFormat_t::YUV422:
            for (int idxChn = 0; idxChn < JPG_NUMB_CHN; ++idxChn) {
                if (idxChn == 0) {
                    memcpy(&(*m_true_datOriMcu)[idxChn], &(*m_datOriMcu)[idxChn], sizeof((*m_datOriMcu)[idxChn]));
                }
                else {
                    for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                        memcpy(&(*m_true_datOriMcu)[idxChn][(idxY << 1)    ], &(*m_datOriMcu)[idxChn][idxY], sizeof((*m_datOriMcu)[idxChn][idxY]));
                        memcpy(&(*m_true_datOriMcu)[idxChn][(idxY << 1) + 1], &(*m_datOriMcu)[idxChn][idxY], sizeof((*m_datOriMcu)[idxChn][idxY]));
                    }
                }
            }
        break;
        }
    }
}

// procCore
void JPG_DCT::procCore()
{
    // mcu entry
    if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC) {
        procCoreMcuFT();
    }
        procCoreMcuBT();
}

// procCoreMcuFT
void JPG_DCT::procCoreMcuFT()
{
    // variables
    double datCoeFT1D[JPG_SIZE_BLK][JPG_SIZE_BLK];
    double datCoeFT2D[JPG_SIZE_BLK][JPG_SIZE_BLK];

    // main loop
    for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn) {
        int idxSubEndX = idxChn == 0 ? 1 : 0;
        int idxSubEndY = ((idxChn == 0) || ((jpg_enmFormat_t)m_cfg->enmFormat == jpg_enmFormat_t::YUV422)) ? 1 : 0;
        for (int idxSubY = 0; idxSubY <= idxSubEndY; ++idxSubY) {
            for (int idxSubX = 0; idxSubX <= idxSubEndX; ++idxSubX) {
                // forward transform 1d
                for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                    for (int idxX = 0; idxX < JPG_SIZE_BLK; ++idxX) {
                        datCoeFT1D[idxX][idxY] = 0;
                        for (int idxM = 0; idxM < JPG_SIZE_BLK; ++idxM) {
                            datCoeFT1D[idxX][idxY] += (*m_datCoeTsf08)[idxX][idxM]
                                *                     (     (jpg_coe_t)(*m_true_datOriMcu)[idxChn][idxSubY * JPG_SIZE_BLK + idxY][idxSubX * JPG_SIZE_BLK + idxM]
                                                        -   JPG_DATA_PXL_HALF
                                                      )
                            ;
                        }
                        if (m_cfg->enmProc == (int)jpg_enmProc_t::HARDWARE)
                            datCoeFT1D[idxX][idxY] = round(datCoeFT1D[idxX][idxY]);
                    }
                }

                // forward transform 2d
                for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                    for (int idxX = 0; idxX < JPG_SIZE_BLK; ++idxX) {
                        datCoeFT2D[idxX][idxY] = 0;
                        for (int idxM = 0; idxM < JPG_SIZE_BLK; ++idxM)
                            datCoeFT2D[idxX][idxY] += (*m_datCoeTsf08)[idxX][idxM] * datCoeFT1D[idxY][idxM];
                        if (m_cfg->enmProc == (int)jpg_enmProc_t::HARDWARE)
                            datCoeFT2D[idxX][idxY] = round(datCoeFT2D[idxX][idxY]);
                    }
                }

                // save data
                for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                    for (int idxX = 0; idxX < JPG_SIZE_BLK; ++idxX) {
                        (*m_datCoeMcu)[idxChn][idxSubY * JPG_SIZE_BLK + idxY][idxSubX * JPG_SIZE_BLK + idxX]
                            = (idxY == 0) && (idxX == 0)
                            ? JPG_FUNC_CLP(JPG_DATA_COE_MIN_DC, JPG_DATA_COE_MAX_DC, round(datCoeFT2D[idxY][idxX] * (*m_datCoeQtzInv)[idxChn][idxY][idxX]))
                            : JPG_FUNC_CLP(JPG_DATA_COE_MIN_AC, JPG_DATA_COE_MAX_AC, round(datCoeFT2D[idxY][idxX] * (*m_datCoeQtzInv)[idxChn][idxY][idxX]))
                        ;
                    }
                }
            }
        }
    }
}

// procCoreMcuBT
void JPG_DCT::procCoreMcuBT()
{
    // variables
    double datCoeBT1D[JPG_SIZE_BLK][JPG_SIZE_BLK];
    double datCoeBT2D[JPG_SIZE_BLK][JPG_SIZE_BLK];

    // main loop
    for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn) {
        int idxSubEndX = idxChn == 0 ? 1 : 0;
        int idxSubEndY = ((idxChn == 0) || ((jpg_enmFormat_t)m_cfg->enmFormat == jpg_enmFormat_t::YUV422)) ? 1 : 0;
        int idxLaC = idxChn == 0 ? 0 : 1;
        for (int idxSubY = 0; idxSubY <= idxSubEndY; ++idxSubY) {
            for (int idxSubX = 0; idxSubX <= idxSubEndX; ++idxSubX) {
                // forward transform 1d
                for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                    for (int idxX = 0; idxX < JPG_SIZE_BLK; ++idxX) {
                        datCoeBT1D[idxX][idxY] = 0;
                        for (int idxM = 0; idxM < JPG_SIZE_BLK; ++idxM) {
                            datCoeBT1D[idxX][idxY] += (*m_datCoeTsf08)[idxM][idxX]
                                *                     (*m_datCoeMcu)[idxChn][idxSubY * JPG_SIZE_BLK + idxY][idxSubX * JPG_SIZE_BLK + idxM]
                                *                      jpg_m_datCoeQtz[idxLaC][idxY][idxM]
                            ;
                        }
                        if (m_cfg->enmProc == (int)jpg_enmProc_t::HARDWARE)
                            datCoeBT1D[idxX][idxY] = round(datCoeBT1D[idxX][idxY]);
                    }
                }

                // forward transform 2d
                for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                    for (int idxX = 0; idxX < JPG_SIZE_BLK; ++idxX) {
                        datCoeBT2D[idxX][idxY] = 0;
                        for (int idxM = 0; idxM < JPG_SIZE_BLK; ++idxM)
                            datCoeBT2D[idxX][idxY] += (*m_datCoeTsf08)[idxM][idxX] * datCoeBT1D[idxY][idxM];
                        if (m_cfg->enmProc == (int)jpg_enmProc_t::HARDWARE)
                            datCoeBT2D[idxX][idxY] = round(datCoeBT2D[idxX][idxY]);
                    }
                }

                // save data
                for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                    for (int idxX = 0; idxX < JPG_SIZE_BLK; ++idxX) {
                        (*m_true_datRecMcu)[idxChn][idxSubY * JPG_SIZE_BLK + idxY][idxSubX * JPG_SIZE_BLK + idxX] =
                            JPG_FUNC_CLP(0, JPG_DATA_PXL_MAX, round(datCoeBT2D[idxY][idxX] + JPG_DATA_PXL_HALF));
                    }
                }
            }
        }
    }
}

// procPost
void JPG_DCT::procPost()
{
    // basic
    JPG_UNIT::procPost();

    //// for the first mcu
    //static bool flag = 1;
    //if (flag) {
    //    // clear flag
    //    flag = 0;

    //    //...
    //}

    //// for the first mcu in each frame
    //if (m_cfg->idxMcuX == 0 && m_cfg->idxMcuY == 0) {
    //    //...
    //}

    // for each mcu
    {
        // downscale
        switch ((jpg_enmFormat_t)m_cfg->enmFormat) {
        case jpg_enmFormat_t::YUV420:
            memcpy(*m_datRecMcu, *m_true_datRecMcu, sizeof(*m_datRecMcu));
        break;
        case jpg_enmFormat_t::YUV422:
            for (int idxChn = 0; idxChn < JPG_NUMB_CHN; ++idxChn) {
                if (idxChn == 0) {
                    memcpy(&(*m_datRecMcu)[idxChn], &(*m_true_datRecMcu)[idxChn], sizeof((*m_datRecMcu)[idxChn]));
                }
                else {
                    for (int idxY = 0; idxY < JPG_SIZE_BLK; ++idxY) {
                        memcpy(&(*m_datRecMcu)[idxChn][idxY], &(*m_true_datRecMcu)[idxChn][idxY << 1], sizeof((*m_datRecMcu)[idxChn][idxY]));
                    }
                }
            }
        break;
        }

        // dump
        m_dmp->setDatRecFrame(*m_cfg, *m_datRecMcu);
    }
}
