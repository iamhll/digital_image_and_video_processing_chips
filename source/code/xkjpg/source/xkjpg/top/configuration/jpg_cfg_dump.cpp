//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_cfg_dump.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : configuration related codes (dump)
    //
//------------------------------------------------------------------------------
    //
    //  Modified       : 2023-07-27 by HLL
    //  Description    : data width optimized
    //  Modified       : 2023-07-27 by HLL
    //  Description    : frame size used for reg dump fixed to 16384
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_cfg.hpp"

//*** FUNCTION *****************************************************************
// jpg_cfgDmp
void jpg_cfgDmp(
jpg_cfg_t *cfg)
{
    string str = cfg->strPrefixDump
        +        "dut_setting.vh"
    ;
    FILE *fpt = fopen(str.c_str(), "w");

    // HEADER
    fprintf(fpt, "// skip checkFileHeader\n");

    // JPG_TOP
    {
        fprintf(fpt, "\n");
        fprintf(fpt, "// JPG_TOP\n");
        fprintf(fpt, "`define DUT_C_JPG_NUMB_FRAME                'd%d\n", cfg->numFrame);
        fprintf(fpt, "`define DUT_C_JPG_SIZE_FRAME_X              'd%d\n", cfg->sizFrameX);
        fprintf(fpt, "`define DUT_C_JPG_SIZE_FRAME_Y              'd%d\n", cfg->sizFrameY);
        fprintf(fpt, "`define DUT_C_JPG_SIZE_FRAME_X_FOR_REG    %d'd%d\n", JPG_FUNC_LOG2(16384), cfg->sizFrameX - 1);
        fprintf(fpt, "`define DUT_C_JPG_SIZE_FRAME_Y_FOR_REG    %d'd%d\n", JPG_FUNC_LOG2(16384), cfg->sizFrameY - 1);
        switch ((jpg_enmFormat_t)cfg->enmFormat) {
        case jpg_enmFormat_t::YUV420:
            fprintf(fpt, "`define DUT_C_JPG_ENUM_FORMAT               `JPG_ENUM_FORMAT_YUV420\n");
        break;
        case jpg_enmFormat_t::YUV422:
            fprintf(fpt, "`define DUT_C_JPG_ENUM_FORMAT               `JPG_ENUM_FORMAT_YUV422\n");
        break;
        }
    }
}
