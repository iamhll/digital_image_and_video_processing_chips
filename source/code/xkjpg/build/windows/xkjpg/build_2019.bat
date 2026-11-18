::------------------------------------------------------------------------------
    ::
    ::  Filename       : build.bat
    ::  Author         : Huang Leilei
    ::  Status         : phase 000
    ::  Created        : 2026-09-17
    ::  Description    : create a windows building environment for xkjpg
    ::
::------------------------------------------------------------------------------

::*** MAIN BODY ****************************************************************
cmake ../../../source/xkjpg/top -G "Visual Studio 16 2019"
pause
