//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_knl_proc.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : kernel related codes (proc)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_knl.hpp"


//*** FUNCTION *****************************************************************
// procMain
void JPG_KNL::procMain()
{
    updTic(enmFunc_t::PROC_PREV); procPrev(); updToc(enmFunc_t::PROC_PREV);
    updTic(enmFunc_t::PROC_CORE); procCore(); updToc(enmFunc_t::PROC_CORE);
    updTic(enmFunc_t::PROC_POST); procPost(); updToc(enmFunc_t::PROC_POST);
}

// procPrev
void JPG_KNL::procPrev()
{
    //...
}

// procCore
void JPG_KNL::procCore()
{
    // loop each mcu
    for (m_cfg->idxMcuY = 0; m_cfg->idxMcuY < m_cfg->numMcuY; ++m_cfg->idxMcuY) {
        for (m_cfg->idxMcuR = 0; m_cfg->idxMcuR < m_cfg->numMcuR; ++m_cfg->idxMcuR) {
            for (m_cfg->idxMcuX = 0; m_cfg->idxMcuX < m_cfg->numMcuX; ++m_cfg->idxMcuX) {
                // cfg
                updTic(enmFunc_t::PROC_CORE_MISC);
                procCoreCfg();
                updToc(enmFunc_t::PROC_CORE_MISC);

                // encode
                if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC) {
                    // dct
                    m_dct.oneShot();

                    // h_c
                    m_h_c.oneShot();
                    m_h_c.cpyNumRbsFrame(&m_numRbsFrame);
                    m_h_c.cpyNumFbsFrame(&m_numFbsFrame);
                }
                else if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::DEC) {
                    // h_d
                    m_h_d.oneShot();

                    // dct
                    m_dct.oneShot();
                }
            }
        }
    }
}

// procCoreCfg
void JPG_KNL::procCoreCfg()
{
    // configuration (true size)
    bool flgBoundryX = (m_cfg->idxMcuX == m_cfg->sizFrameX / m_cfg->sizMcu) && (m_cfg->sizFrameX % m_cfg->sizMcu != 0);
    bool flgBoundryY = (m_cfg->idxMcuY == m_cfg->sizFrameY / m_cfg->sizMcu) && (m_cfg->sizFrameY % m_cfg->sizMcu != 0);
    int  datBoundryX = m_cfg->sizFrameX % m_cfg->sizMcu + (m_cfg->sizFrameX % m_cfg->sizMcu == 0) * m_cfg->sizMcu;
    int  datBoundryY = m_cfg->sizFrameY % m_cfg->sizMcu + (m_cfg->sizFrameY % m_cfg->sizMcu == 0) * m_cfg->sizMcu;
    m_cfg->trueSizMcuX = flgBoundryX ? datBoundryX : m_cfg->sizMcu;
    m_cfg->trueSizMcuY = flgBoundryY ? datBoundryY : m_cfg->sizMcu;
}

// procPost
void JPG_KNL::procPost()
{
    //...
}
