//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_common_tool_general.hpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : common related inline functions (NOT headers) (general tool)
    //
//------------------------------------------------------------------------------

//*** INLINE *******************************************************************
// jpg_getDatRound
inline double jpg_getDatRound(
double dat, int numBitFrc)
{
    double datScl = pow(2, numBitFrc);
    return round(dat * datScl) / datScl;
}

// jpg_getDatFloor
inline double jpg_getDatFloor(
double dat, int numBitFrc)
{
    double datScl = pow(2, numBitFrc);
    return floor(dat * datScl) / datScl;
}
