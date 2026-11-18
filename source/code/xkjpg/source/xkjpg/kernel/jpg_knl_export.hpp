//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_knl_export.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : kernel related headers (export)
    //
//------------------------------------------------------------------------------

#ifndef __JPG_KNL_EXPORT_HPP__
#define __JPG_KNL_EXPORT_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_cfg.hpp"


//*** CLASS ********************************************************************
// JPG_KNL
class JPG_KNL;

// JPG_KNL_EXPORT
class JPG_KNL_EXPORT
{
//--- PRIVATE VARIABLE -----------------
private:
    // module
        JPG_KNL *m_knl;


//--- PUBLIC FUNCTION ------------------
public:
    // basic
    JPG_KNL_EXPORT();
    ~JPG_KNL_EXPORT();
    void connect(jpg_cfg_t &cfg);
    void oneShot();

    // display
    void dspRunTime();
};

#endif /* __JPG_KNL_EXPORT_HPP__ */
