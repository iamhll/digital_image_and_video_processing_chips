//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_cfg_proc.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : configuration related codes (proc)
    //
//------------------------------------------------------------------------------
    //
    //  *** auto-generated, please do NOT modify it mannually ***
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_cfg.hpp"


//*** FUNCTION *****************************************************************
// jpg_cfgRst
void jpg_cfgRst(jpg_cfg_t *cfg)
{
    // changeable
    cfg->idxFrame         = 0;
    cfg->idxMcuX          = 0;
    cfg->idxMcuY          = 0;
    cfg->idxMcuR          = 0;
    cfg->trueSizMcuX      = 0;
    cfg->trueSizMcuY      = 0;
    cfg->idxBsCfg         = 0;

    // top
    cfg->strFileInp       = "xkjpg.yuv";
    cfg->strFileOut       = "xkjpg";
    cfg->strPrefixDump    = "";
    cfg->strPrefixLoad    = "";
    cfg->flgDumpCsv       = 1;
    cfg->flgDumpYuvRec    = 1;
    cfg->flgDumpDbg       = 0;
    cfg->flgDistortion[0] = 1;
    cfg->flgDistortion[1] = 0;
    cfg->numFrame         = 10;
    cfg->numChn           = 3;
    cfg->idxFrameBgn      = 0;
    cfg->sizFrameX        = 416;
    cfg->sizFrameY        = 240;
    cfg->sizMcu           = 16;
    cfg->enmModRun        = 0;
    cfg->enmFormat        = 0;
    cfg->enmProc          = 0;
    cfg->enmInfo          = 1;
    cfg->enmDump          = 0;

    // descreteCosineTransform
    cfg->dctFlgProc       = 1;
    cfg->dctEnmLoad       = 0;
    cfg->dctEnmDump       = 0;

    // huffmanEncoding
    cfg->h_cFlgProc       = 1;
    cfg->h_cEnmLoad       = 0;
    cfg->h_cEnmDump       = 0;

    // interface
    cfg->itfFlgProc       = 1;
    cfg->itfEnmLoad       = 0;
    cfg->itfEnmDump       = 0;

    // derived
    cfg->numMcuX          = 0;
    cfg->numMcuY          = 0;
    cfg->numMcuR          = 0;
    cfg->numLaC           = 0;
    cfg->trueSizFrameX    = 416;
    cfg->trueSizFrameY    = 240;
}

// jpg_cfgHlp
void jpg_cfgHlp(
jpg_cfg_t *cfg)
{
    cout << endl;
    cout << "[virtual]" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    strFileCfg       full location of the configuration file" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    help             to print help information" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    show             to show configurations" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    dryRun           to check configurations without actual running" << endl;
    cout << endl;
    cout << "[top]" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    strFileInp       full location of the input file" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    strFileOut       full location of the output file" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    strPrefixDump    dump prefix" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    strPrefixLoad    load prefixon)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    flgDumpCsv       enable flag for the dump of csv-format log" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    flgDumpYuvRec    enable flag for the dump of reconstructed YUV" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    flgDumpDbg       enable flag for the dump of debug information" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    flgDistortion    enable flag for the distortion calculation (<index> 0/1: PSNR/MS-SSIM)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    numFrame         number of frames to be processed" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    numChn           number of channels (Y/U/V)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    idxFrameBgn      index of the first frame to be processed" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    sizFrameX        size of frames in X (horizontal) direction" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    sizFrameY        size of frames in Y (vertical) direction" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    sizMcu           size of the MCU" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    enmModRun        enumerator for the running mode (0: encode; 1: decode)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    enmFormat        enumerator for color format (0: YUV420; 1: YUV422)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   ) cout << "    enmProc          enumerator for the processing behavior (<value> 0: software 1: hardware)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    enmInfo          enumerator for the information level (<value> 0: none 1: top 2: kernel 3: unit 4: common)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    enmDump          enumerator for data dumping (<value> 0: none 1: inout 2: intermediate)" << endl;
    cout << endl;
    cout << "[descreteCosineTransform]" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    dctFlgProc       enable flag for processing" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    dctEnmLoad       enumerator for load source (<value> 0: pipeline; 2: file; 3: random)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    dctEnmDump       enumerator for data dump (<value> 0: none 1: inout 2: intermediate)" << endl;
    cout << endl;
    cout << "[huffmanEncoding]" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    h_cFlgProc       enable flag for processing" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    h_cEnmLoad       enumerator for load source (<value> 0: pipeline; 2: file; 3: random)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    h_cEnmDump       enumerator for data dump (<value> 0: none 1: inout 2: intermediate)" << endl;
    cout << endl;
    cout << "[interface]" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    itfFlgProc       enable flag for processing" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    itfEnmLoad       enumerator for load source (<value> 0: pipeline; 2: file; 3: random)" << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED) cout << "    itfEnmDump       enumerator for data dump (<value> 0: none 1: inout 2: intermediate)" << endl;
}

// jpg_cfgMap
void jpg_cfgMap(jpg_cfg_t *cfg,
const string *strKey, const string *strDat)
{
    // variables
    const char *datAsCStr   = strDat->data();
    int         datAsInt    = atoi(strDat->data());
    double      datAsDouble = atof(strDat->data());
    bool        datAsBool   = datAsInt ? true : false;

    // mapping
    if      (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--strFileInp"     || *strKey == "-i"))    cfg->strFileInp       = datAsCStr;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--strFileOut"     || *strKey == "-o"))    cfg->strFileOut       = datAsCStr;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--strPrefixDump"  || *strKey == "-d"))    cfg->strPrefixDump    = datAsCStr;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--strPrefixLoad"  || *strKey == "-l"))    cfg->strPrefixLoad    = datAsCStr;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--flgDumpCsv"                       ))    cfg->flgDumpCsv       = datAsBool;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--flgDumpYuvRec"                    ))    cfg->flgDumpYuvRec    = datAsBool;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--flgDumpDbg"                       ))    cfg->flgDumpDbg       = datAsBool;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--flgDistortion_0"                  ))    cfg->flgDistortion[0] = datAsBool;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--flgDistortion_1"                  ))    cfg->flgDistortion[1] = datAsBool;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--numFrame"       || *strKey == "-f"))    cfg->numFrame         = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--numChn"                           ))    cfg->numChn           = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--idxFrameBgn"                      ))    cfg->idxFrameBgn      = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--sizFrameX"      || *strKey == "-w"))    cfg->sizFrameX        = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--sizFrameY"      || *strKey == "-h"))    cfg->sizFrameY        = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--sizMcu"                           ))    cfg->sizMcu           = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--enmModRun"                        ))    cfg->enmModRun        = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--enmFormat"                        ))    cfg->enmFormat        = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH    && (*strKey == "--enmProc"        || *strKey == "-e"))    cfg->enmProc          = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--enmInfo"        || *strKey == "-v"))    cfg->enmInfo          = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--enmDump"                          ))    cfg->enmDump          = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--dctFlgProc"                       ))    cfg->dctFlgProc       = datAsBool;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--dctEnmLoad"                       ))    cfg->dctEnmLoad       = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--dctEnmDump"                       ))    cfg->dctEnmDump       = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--h_cFlgProc"                       ))    cfg->h_cFlgProc       = datAsBool;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--h_cEnmLoad"                       ))    cfg->h_cEnmLoad       = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--h_cEnmDump"                       ))    cfg->h_cEnmDump       = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--itfFlgProc"                       ))    cfg->itfFlgProc       = datAsBool;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--itfEnmLoad"                       ))    cfg->itfEnmLoad       = datAsInt;
    else if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED && (*strKey == "--itfEnmDump"                       ))    cfg->itfEnmDump       = datAsInt;
}

// jpg_cfgL2X
void jpg_cfgL2X(jpg_cfg_t *cfg)
{
}

// jpg_cfgChk
int  jpg_cfgChk(
jpg_cfg_t *cfg)
{
    if (cfg->numFrame < 1) {
        cerr << "[error from c model] cfg->numFrame should be within [1, +inf)!" << endl;
        return 1;
    };
    if (cfg->numChn < 1 || cfg->numChn > 3) {
        cerr << "[error from c model] cfg->numChn should be within [1,3]!" << endl;
        return 1;
    };
    if (cfg->idxFrameBgn < 0) {
        cerr << "[error from c model] cfg->idxFrameBgn should be within [0, +inf)!" << endl;
        return 1;
    };
    if (cfg->sizFrameX < 32 || cfg->sizFrameX > 16384) {
        cerr << "[error from c model] cfg->sizFrameX should be within [32,16384]!" << endl;
        return 1;
    };
    if (cfg->sizFrameY < 32 || cfg->sizFrameY > 16384) {
        cerr << "[error from c model] cfg->sizFrameY should be within [32,16384]!" << endl;
        return 1;
    };
    if (cfg->sizMcu < 16 || cfg->sizMcu > 16) {
        cerr << "[error from c model] cfg->sizMcu should be within [16,16]!" << endl;
        return 1;
    };
    if (cfg->enmModRun < 0 || cfg->enmModRun > 1) {
        cerr << "[error from c model] cfg->enmModRun should be within [0,1]!" << endl;
        return 1;
    };
    if (cfg->enmFormat < 0 || cfg->enmFormat > 1) {
        cerr << "[error from c model] cfg->enmFormat should be within [0,1]!" << endl;
        return 1;
    };
    if (cfg->enmProc < 0 || cfg->enmProc > 1) {
        cerr << "[error from c model] cfg->enmProc should be within [0,1]!" << endl;
        return 1;
    };
    if (cfg->enmInfo < 0 || cfg->enmInfo > 5) {
        cerr << "[error from c model] cfg->enmInfo should be within [0,5]!" << endl;
        return 1;
    };
    if (cfg->enmDump < 0 || cfg->enmDump > 2) {
        cerr << "[error from c model] cfg->enmDump should be within [0,2]!" << endl;
        return 1;
    };
    if (cfg->dctEnmLoad < 0 || cfg->dctEnmLoad > 2) {
        cerr << "[error from c model] cfg->dctEnmLoad should be within [0,2]!" << endl;
        return 1;
    };
    if (cfg->dctEnmDump < 0 || cfg->dctEnmDump > 2) {
        cerr << "[error from c model] cfg->dctEnmDump should be within [0,2]!" << endl;
        return 1;
    };
    if (cfg->h_cEnmLoad < 0 || cfg->h_cEnmLoad > 2) {
        cerr << "[error from c model] cfg->h_cEnmLoad should be within [0,2]!" << endl;
        return 1;
    };
    if (cfg->h_cEnmDump < 0 || cfg->h_cEnmDump > 2) {
        cerr << "[error from c model] cfg->h_cEnmDump should be within [0,2]!" << endl;
        return 1;
    };
    if (cfg->itfEnmLoad < 0 || cfg->itfEnmLoad > 2) {
        cerr << "[error from c model] cfg->itfEnmLoad should be within [0,2]!" << endl;
        return 1;
    };
    if (cfg->itfEnmDump < 0 || cfg->itfEnmDump > 2) {
        cerr << "[error from c model] cfg->itfEnmDump should be within [0,2]!" << endl;
        return 1;
    };
    if (cfg->trueSizFrameX < 32 || cfg->trueSizFrameX > 16384) {
        cerr << "[error from c model] cfg->trueSizFrameX should be within [32,16384]!" << endl;
        return 1;
    };
    if (cfg->trueSizFrameY < 32 || cfg->trueSizFrameY > 16384) {
        cerr << "[error from c model] cfg->trueSizFrameY should be within [32,16384]!" << endl;
        return 1;
    };
    return 0;
}

// jpg_cfgLog
void jpg_cfgLog(
jpg_cfg_t *cfg)
{
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--strFileInp         " << cfg->strFileInp.data() << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--strFileOut         " << cfg->strFileOut.data() << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--strPrefixDump      " << cfg->strPrefixDump.data() << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--strPrefixLoad      " << cfg->strPrefixLoad.data() << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--flgDumpCsv         " << cfg->flgDumpCsv << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--flgDumpYuvRec      " << cfg->flgDumpYuvRec << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--flgDumpDbg         " << cfg->flgDumpDbg << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--flgDistortion_0    " << cfg->flgDistortion[0] << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--flgDistortion_1    " << cfg->flgDistortion[1] << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--numFrame           " << cfg->numFrame << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--numChn             " << cfg->numChn << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--idxFrameBgn        " << cfg->idxFrameBgn << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--sizFrameX          " << cfg->sizFrameX << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--sizFrameY          " << cfg->sizFrameY << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--sizMcu             " << cfg->sizMcu << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--enmModRun          " << cfg->enmModRun << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--enmFormat          " << cfg->enmFormat << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::ROUGH   )    cout << "--enmProc            " << cfg->enmProc << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--enmInfo            " << cfg->enmInfo << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--enmDump            " << cfg->enmDump << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--dctFlgProc         " << cfg->dctFlgProc << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--dctEnmLoad         " << cfg->dctEnmLoad << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--dctEnmDump         " << cfg->dctEnmDump << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--h_cFlgProc         " << cfg->h_cFlgProc << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--h_cEnmLoad         " << cfg->h_cEnmLoad << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--h_cEnmDump         " << cfg->h_cEnmDump << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--itfFlgProc         " << cfg->itfFlgProc << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--itfEnmLoad         " << cfg->itfEnmLoad << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--itfEnmDump         " << cfg->itfEnmDump << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--numMcuX            " << cfg->numMcuX << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--numMcuY            " << cfg->numMcuY << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--numMcuR            " << cfg->numMcuR << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--numLaC             " << cfg->numLaC << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--trueSizFrameX      " << cfg->trueSizFrameX << endl;
    if (JPG_CFG_ENUM_LVL_RLS >= jpg_enmLvlCfg_t::DETAILED)    cout << "--trueSizFrameY      " << cfg->trueSizFrameY << endl;
}
