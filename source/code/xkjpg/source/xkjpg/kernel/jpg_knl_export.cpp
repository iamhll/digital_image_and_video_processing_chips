//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_knl_export.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : kernel related codes (export)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_knl.hpp"
#include "jpg_knl_export.hpp"


//*** CLASS ********************************************************************
// JPG_KNL_EXPORT
JPG_KNL_EXPORT::JPG_KNL_EXPORT()
{
    m_knl = new JPG_KNL;
}

// ~JPG_KNL_EXPORT
JPG_KNL_EXPORT::~JPG_KNL_EXPORT()
{
    delete m_knl;
}

// connect
void JPG_KNL_EXPORT::connect(
jpg_cfg_t &cfg)
{
    m_knl->connect(cfg);
}

// oneShot
void JPG_KNL_EXPORT::oneShot()
{
    m_knl->oneShot();
}

// dspRunTime
void JPG_KNL_EXPORT::dspRunTime()
{
    m_knl->dspRunTime();
}
