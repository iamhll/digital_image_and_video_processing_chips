//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_cfg_type.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : configuration related header (type)
    //
//------------------------------------------------------------------------------
    //
    //  *** auto-generated, please do NOT modify it mannually ***
    //
//------------------------------------------------------------------------------

#ifndef __JPG_CFG_TYPE_HPP__
#define __JPG_CFG_TYPE_HPP__

//*** INCLUDE ******************************************************************
    #include "jpg_common.hpp"


//*** TYPE *********************************************************************
    struct jpg_cfg_t {
        // changeable
        int         idxFrame;
        int         idxMcuX;
        int         idxMcuY;
        int         idxMcuR;
        int         trueSizMcuX;
        int         trueSizMcuY;
        int         idxBsCfg;

        // top
        string      strFileInp;
        string      strFileOut;
        string      strPrefixDump;
        string      strPrefixLoad;
        bool        flgDumpCsv;
        bool        flgDumpYuvRec;
        bool        flgDumpDbg;
        bool        flgDistortion[2];
        int         numFrame;
        int         numChn;
        int         idxFrameBgn;
        int         sizFrameX;
        int         sizFrameY;
        int         sizMcu;
        int         enmModRun;
        int         enmFormat;
        int         enmProc;
        int         enmInfo;
        int         enmDump;

        // descreteCosineTransform
        bool        dctFlgProc;
        int         dctEnmLoad;
        int         dctEnmDump;

        // huffmanEncoding
        bool        h_cFlgProc;
        int         h_cEnmLoad;
        int         h_cEnmDump;

        // interface
        bool        itfFlgProc;
        int         itfEnmLoad;
        int         itfEnmDump;

        // derived
        int         numMcuX;
        int         numMcuY;
        int         numMcuR;
        int         numLaC;
        int         trueSizFrameX;
        int         trueSizFrameY;

        // =
        jpg_cfg_t &operator=(const jpg_cfg_t &cfg) {
            // changeable
            idxFrame            = cfg.idxFrame     ;
            idxMcuX             = cfg.idxMcuX      ;
            idxMcuY             = cfg.idxMcuY      ;
            idxMcuR             = cfg.idxMcuR      ;
            trueSizMcuX         = cfg.trueSizMcuX  ;
            trueSizMcuY         = cfg.trueSizMcuY  ;
            idxBsCfg            = cfg.idxBsCfg     ;

            // top
            strFileInp          = cfg.strFileInp   ;
            strFileOut          = cfg.strFileOut   ;
            strPrefixDump       = cfg.strPrefixDump;
            strPrefixLoad       = cfg.strPrefixLoad;
            flgDumpCsv          = cfg.flgDumpCsv   ;
            flgDumpYuvRec       = cfg.flgDumpYuvRec;
            flgDumpDbg          = cfg.flgDumpDbg   ;
            memcpy(flgDistortion, cfg.flgDistortion, sizeof(flgDistortion));
            numFrame            = cfg.numFrame     ;
            numChn              = cfg.numChn       ;
            idxFrameBgn         = cfg.idxFrameBgn  ;
            sizFrameX           = cfg.sizFrameX    ;
            sizFrameY           = cfg.sizFrameY    ;
            sizMcu              = cfg.sizMcu       ;
            enmModRun           = cfg.enmModRun    ;
            enmFormat           = cfg.enmFormat    ;
            enmProc             = cfg.enmProc      ;
            enmInfo             = cfg.enmInfo      ;
            enmDump             = cfg.enmDump      ;

            // descreteCosineTransform
            dctFlgProc          = cfg.dctFlgProc   ;
            dctEnmLoad          = cfg.dctEnmLoad   ;
            dctEnmDump          = cfg.dctEnmDump   ;

            // huffmanEncoding
            h_cFlgProc          = cfg.h_cFlgProc   ;
            h_cEnmLoad          = cfg.h_cEnmLoad   ;
            h_cEnmDump          = cfg.h_cEnmDump   ;

            // interface
            itfFlgProc          = cfg.itfFlgProc   ;
            itfEnmLoad          = cfg.itfEnmLoad   ;
            itfEnmDump          = cfg.itfEnmDump   ;

            // derived
            numMcuX             = cfg.numMcuX      ;
            numMcuY             = cfg.numMcuY      ;
            numMcuR             = cfg.numMcuR      ;
            numLaC              = cfg.numLaC       ;
            trueSizFrameX       = cfg.trueSizFrameX;
            trueSizFrameY       = cfg.trueSizFrameY;
            return *this;
        }
    };

#endif /* __JPG_CFG_TYPE_HPP__ */
