//------------------------------------------------------------------------------
    //
    //  Filename       : jpg.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : top related codes
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2022-12-09 by HLL, LYZ
    //  Description    : fully tidied up
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_cfg.hpp"
#include "jpg_knl_export.hpp"


//*** FUNCTION *****************************************************************
// jpg_logo
void jpg_logo(
jpg_cfg_t cfg)
{
    cout << endl;
    cout << "+-------------------------------+" << endl;
    cout << "| XKJPG     VERSION: 2024.05.08 |" << endl;
    cout << "+-------------------------------+" << endl;
    cout << endl;
}

// main
int main(
int numArg, char *strArg[])
{
    // assertion
    #ifdef FLAG_ASM
    // !!! please disable FLAG_ASM when JPG_DATA_PXL_WD != 8
    assert(JPG_DATA_PXL_WD == 8);
    #endif

    // variables
    jpg_cfg_t cfg;
    int datRet;
    JPG_KNL_EXPORT knlExport;
    #ifdef JPG_KNOB_TIME
    #ifdef __linux__
    timespec tic;
    timespec toc;
    #endif
    #endif

    // init cfg
    datRet = jpg_cfgIni(&cfg, numArg, strArg);
    if (datRet != 0)
        return datRet;

    // dry run
    for (int i = 0; i < numArg; ++i) {
        string strKey = strArg[i];
        if (strKey == "--dryRun") {
            return 0;
        }
    }

    // init seed
    //srand(cfg.datSeed);

    // show logo
    if ((jpg_enmInfo_t)cfg.enmInfo >= jpg_enmInfo_t::TOP)
        jpg_logo(cfg);

    // connect knlExport
    knlExport.connect(cfg);

    // set tic
    #ifdef JPG_KNOB_TIME
    #ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &tic);
    #endif
    #endif

    // run
    for (cfg.idxFrame = 0; cfg.idxFrame < cfg.numFrame; ++cfg.idxFrame) {
        // one shot
        knlExport.oneShot();
    }

    // set toc
    #ifdef JPG_KNOB_TIME
    #ifdef __linux__
    clock_gettime(CLOCK_MONOTONIC, &toc);
    #endif
    #endif

    // display run time
    #ifdef JPG_KNOB_TIME
    #ifdef __linux__
    {
        unsigned long datDlt = toc.tv_sec * 1000 + toc.tv_nsec / 1000 / 1000
            -                  tic.tv_sec * 1000 - tic.tv_nsec / 1000 / 1000
        ;
        cout << "run time                   is " << setprecision(3) << (double)datDlt / 1000 << " s" << endl;
    }
    knlExport.dspRunTime();
    #endif
    #endif

    // return
    return 0;
}
