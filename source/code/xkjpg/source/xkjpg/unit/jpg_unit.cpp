//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_unit.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : unit related codes (top)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_unit.hpp"


//*** FUNCTION *****************************************************************
// JPG_UNIT
JPG_UNIT::JPG_UNIT()
{
    // pixel
    {
        // ori
        m_datOriMcu = (jpg_pxl_t (*)[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU])
            new        jpg_pxl_t    [JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU];
        memset(*m_datOriMcu, 0, sizeof(*m_datOriMcu));

        // coe
        m_datCoeMcu = (jpg_coe_t (*)[JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU])
            new        jpg_coe_t    [JPG_NUMB_CHN][JPG_SIZE_MCU][JPG_SIZE_MCU];
        memset(*m_datCoeMcu, 0, sizeof(*m_datCoeMcu));
    }
}

// ~JPG_UNIT
JPG_UNIT::~JPG_UNIT()
{
    // pixel
    {
        delete[] m_datOriMcu;

        delete[] m_datCoeMcu;
    }
}

// connect
void JPG_UNIT::connect(
jpg_cfg_t &cfg, JPG_FTH &fth, JPG_DMP &dmp)
{
    m_cfg         = &cfg;
    m_cfgWritable = &cfg;
    m_fth         = &fth;
    m_dmp         = &dmp;
}

// oneShot
void JPG_UNIT::oneShot()
{
    updTic(enmFunc_t::MAIN);
    load();
    proc();
    dump();
    updToc(enmFunc_t::MAIN);
}

// load
void JPG_UNIT::load()
{
    static bool flag = 1;
    if (flag) {
        flag = 0;
        if (m_cfg == NULL) {
            cerr << "[error from c model] invalid configuration structure assigned in module " << m_strTag.c_str() + JPG_SIZE_PREFIX << "!" << endl;
            assert(0);
        }
    }
    switch (m_cfgEnmLoad) {
    case jpg_enmLoad_t::PIPE:
        if ((jpg_enmInfo_t)m_cfg->enmInfo >= jpg_enmInfo_t::UNIT)
            cout << "  FRAME [" << setw(ceil(log10(m_cfg->numFrame))) << setfill('0') << m_cfg->idxFrame << "]"
                <<    " UNIT [" << setw(ceil(log10(m_cfg->numMcuY ))) << setfill('0') << m_cfg->idxMcuY  << "," << setw(ceil(log10(m_cfg->numMcuX))) << setfill('0') << m_cfg->idxMcuX << "]"
                <<    " LOAD [PIPE]"
            << endl;
        loadPipe();
    break;
    case jpg_enmLoad_t::FILE:
        if ((jpg_enmInfo_t)m_cfg->enmInfo >= jpg_enmInfo_t::UNIT)
            cout << "  FRAME [" << setw(ceil(log10(m_cfg->numFrame))) << setfill('0') << m_cfg->idxFrame << "]"
                <<    " UNIT [" << setw(ceil(log10(m_cfg->numMcuY ))) << setfill('0') << m_cfg->idxMcuY  << "," << setw(ceil(log10(m_cfg->numMcuX))) << setfill('0') << m_cfg->idxMcuX << "]"
                <<    " LOAD [FILE]"
            << endl;
        loadFile();
    break;
    case jpg_enmLoad_t::RANDOM:
        if ((jpg_enmInfo_t)m_cfg->enmInfo >= jpg_enmInfo_t::UNIT)
            cout << "  FRAME [" << setw(ceil(log10(m_cfg->numFrame))) << setfill('0') << m_cfg->idxFrame << "]"
                <<    " UNIT [" << setw(ceil(log10(m_cfg->numMcuY ))) << setfill('0') << m_cfg->idxMcuY  << "," << setw(ceil(log10(m_cfg->numMcuX))) << setfill('0') << m_cfg->idxMcuX << "]"
                <<    " LOAD [RAND]"
            << endl;
        loadRand();
    break;
    default:
        assert(0);
    break;
    }
}

// proc
void JPG_UNIT::proc()
{
    if (m_cfgFlgProc) {
        if ((jpg_enmInfo_t)m_cfg->enmInfo >= jpg_enmInfo_t::UNIT)
            cout << "  FRAME [" << setw(ceil(log10(m_cfg->numFrame))) << setfill('0') << m_cfg->idxFrame << "]"
                <<    " UNIT [" << setw(ceil(log10(m_cfg->numMcuY ))) << setfill('0') << m_cfg->idxMcuY  << "," << setw(ceil(log10(m_cfg->numMcuX))) << setfill('0') << m_cfg->idxMcuX << "]"
                <<    " LOAD [NORM]"
            << endl;
        procMain();
    }
    else {
        if ((jpg_enmInfo_t)m_cfg->enmInfo >= jpg_enmInfo_t::UNIT)
            cout << "  FRAME [" << setw(ceil(log10(m_cfg->numFrame))) << setfill('0') << m_cfg->idxFrame << "]"
                <<    " UNIT [" << setw(ceil(log10(m_cfg->numMcuY ))) << setfill('0') << m_cfg->idxMcuY  << "," << setw(ceil(log10(m_cfg->numMcuX))) << setfill('0') << m_cfg->idxMcuX << "]"
                <<    " LOAD [SKIP]"
            << endl;
    }
}

// dump
void JPG_UNIT::dump()
{
    dumpFile();
}
