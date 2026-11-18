#!/bin/bash
#-------------------------------------------------------------------------------
    #
    #  Filename       : build.sh
    #  Author         : Huang Leilei
    #  Status         : phase 000
    #  Created        : 2026-09-17
    #  Description    : create a linux building environment for xkjpg
    #
#-------------------------------------------------------------------------------

#*** PARAMETER *****************************************************************
CSTR_DIR_PRJ=../../../
CSTR_USAGE="[information from $0] usage: ./build.sh [<building parameters>]"


#*** MAIN BODY *****************************************************************
cmake $CSTR_DIR_PRJ/source/xkjpg/top $*
exit 0
