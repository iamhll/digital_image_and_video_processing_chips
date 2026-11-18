//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_common_tool_encoding.cpp
    //  Author         : Li Yanzhong
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : common related variables (NOT headers) (encoding tool)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_common.hpp"


//*** MAIN BODY ****************************************************************
int jpg_m_datCoeQtz[JPG_NUMB_LAC][JPG_SIZE_BLK][JPG_SIZE_BLK];
int jpg_m_datCoeQtzZigzag[JPG_NUMB_LAC][JPG_NUMB_PXL_BLK];
int jpg_m_numSymbolDht[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_SIZE_CODE_MAX  ];
int jpg_m_datSymbolDht[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_NUMB_SYMBOL_MAX];