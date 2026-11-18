//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_cfg.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : configuration related header
    //
//------------------------------------------------------------------------------

#ifndef __JPG_CFG_HPP__
#define __JPG_CFG_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_cfg_type.hpp"


//*** DEFINITION ***************************************************************
    #define JPG_CFG_ENUM_LVL_RLS    (jpg_enmLvlCfg_t::DETAILED)


//*** TYPE *********************************************************************
    // jpg_enmLvlCfg_t
        enum class jpg_enmLvlCfg_t {
            ROUGH = 0,
            DETAILED
        };


//*** FUNCTION *****************************************************************
    // written manually (for all frames)
        int  jpg_cfgIni        (jpg_cfg_t *cfg, int numArg, char *strArg[]);
        int  jpg_cfgIniFromFile(jpg_cfg_t *cfg, int numArg, char *strArg[]);
        int  jpg_cfgIniFromBs  (jpg_cfg_t *cfg, int numArg, char *strArg[]);
        int  jpg_cfgIniFromCli (jpg_cfg_t *cfg, int numArg, char *strArg[]);
        void jpg_cfgDmp        (jpg_cfg_t *cfg);

    // written manually (for each frame)
        void jpg_cfgSet                          (jpg_cfg_t *cfg);
        void jpg_cfgSetNumReorderPics            (jpg_cfg_t *cfg);
        void jpg_cfgSetGopAccordingToNumFrameTrue(jpg_cfg_t *cfg);
        void jpg_cfgSetIdxRefLastGop             (jpg_cfg_t *cfg);

    // written by scripts
        void jpg_cfgRst   (jpg_cfg_t *cfg);
        void jpg_cfgHlp   (jpg_cfg_t *cfg);
        void jpg_cfgMap   (jpg_cfg_t *cfg, const string *strKey, const string *strDat);
        void jpg_cfgL2X   (jpg_cfg_t *cfg);
        int  jpg_cfgChk   (jpg_cfg_t *cfg);
        void jpg_cfgLog   (jpg_cfg_t *cfg);

#endif /* __JPG_CFG_HPP__ */
