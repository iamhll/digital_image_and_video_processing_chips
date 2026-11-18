//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_knl_dump.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : kernel related codes (dump)
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_knl.hpp"


//*** FUNCTION *****************************************************************
// dumpFile
void JPG_KNL::dumpFile()
{
    // dumpFileRec,Bin,Csv
    if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::DEC || m_cfg->flgDumpYuvRec)
        dumpFileRec();
    if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC)
        dumpFileBin();
    if (m_cfg->flgDumpCsv)
        dumpFileCsv();

    // dumpDatOriFrame
    if (   ((jpg_enmDump_t)m_cfg->enmDump    >= jpg_enmDump_t::INOUT)
        || ((jpg_enmDump_t)m_cfg->itfEnmDump >= jpg_enmDump_t::INOUT)
    ) {
        for (int c = 0; c < JPG_NUMB_CHN; ++c)
            dumpDatOriFrame(c);
    }

    // dumpDatRecFrame
    if (   ((jpg_enmDump_t)m_cfg->enmDump    >= jpg_enmDump_t::INOUT)
        || ((jpg_enmDump_t)m_cfg->itfEnmDump >= jpg_enmDump_t::INOUT)
    ) {
        for (int c = 0; c < JPG_NUMB_CHN; ++c)
            dumpDatRecFrame(c);
    }

    if (m_cfg->idxFrame == 0) {
        if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::DEC 
        && (  (jpg_enmDump_t)m_cfg->enmDump   >= jpg_enmDump_t::INOUT)) {
            for (int idxLaC = 0; idxLaC < JPG_NUMB_LAC; ++idxLaC) {
                for (int idxTypCoe = 0; idxTypCoe < (int)jpg_enmCoe_t::NUMB; ++idxTypCoe) {
                    dumpDatSymbolDht(idxLaC, idxTypCoe);
                }
            }
        }
    }
}

// dumpFileRec
void JPG_KNL::dumpFileRec()
{
    // open file
    if (!m_dumpFileRecFlg) {
        m_dumpFileRecFlg = 1;
        string strFul;
        if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC)
            strFul = m_cfg->strPrefixDump + "xkjpg.yuv";
        else
            strFul = m_cfg->strFileOut;
        m_dumpFileRecFpt = fopen(strFul.c_str(), "wb");
    }

    // dump
    for (int idxChn = 0; idxChn < m_cfg->numChn; idxChn++) {
        int datShift = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
        for(int i = 0 ;i < m_cfg->trueSizFrameY >> datShift; i++)
            fwrite((*m_datRecFrame)[idxChn][i], sizeof(jpg_pxl_t), m_cfg->trueSizFrameX >> datShift, m_dumpFileRecFpt);
    }

    // close file
    if (m_cfg->numFrame == m_cfg->idxFrame - 1)
        fclose(m_dumpFileRecFpt);
}

// dumpFileBin
void JPG_KNL::dumpFileBin()
{
    // open file
    string strFileOut = m_cfg->strFileOut + "_" + to_string(m_cfg->idxFrame) + ".jpg";
    m_dumpFileBinFpt = fopen(strFileOut.c_str(),"wb");

    // dump
    assert(fwrite(m_datFbsFrame, sizeof(uint8_t), m_numFbsFrame, m_dumpFileBinFpt) == (size_t)m_numFbsFrame);

    // close file
    fclose(m_dumpFileBinFpt);
}

// dumpFileCsv
void JPG_KNL::dumpFileCsv()
{
    // variables
    static double datPsnrFull[3] = {0};
    static double datSsimFull[3] = {0};
    static double datBitFull     =  0 ;
    static int    cntFrameFull   =  0 ;
    int           datWeightY     = (1 << JPG_DATA_SHF_CH) * (1 << JPG_DATA_SHF_CH) ;
    int           datWeightYuv   = datWeightY + 1 + 1;

    // open file
    if (m_cfg->flgDumpCsv) {
        if (!m_dumpFileCsvFlg) {
            m_dumpFileCsvFlg = 1;
            string strFul = m_cfg->strPrefixDump + "xkjpg.csv";
            m_dumpFileCsvFpt = fopen(strFul.c_str(), "w");
            fprintf(m_dumpFileCsvFpt, "Number, PSNR AVE, PSNR Y, PSNR U, PSNR V, MS-SSIM AVE, MS-SSIM Y, MS-SSIM U, MS-SSIM V, Bit\n");
        }
    }

    // calculate PSNR
    double datPsnr[JPG_NUMB_CHN] = {0};
    if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC && m_cfg->flgDistortion[0]) {
        double datSe[JPG_NUMB_CHN] = {0};
        for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn) {
            int datShf = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
            for (int j = 0; j < m_cfg->trueSizFrameY >> datShf; ++j) {
                for (int i = 0; i < m_cfg->trueSizFrameX >> datShf; ++i) {
                    int datDif = (*m_datOriFrame)[idxChn][j][i] - (*m_datRecFrame)[idxChn][j][i];
                    datSe[idxChn] += datDif * datDif;
                }
            }
            double datMse = datSe[idxChn] / ((m_cfg->trueSizFrameX >> datShf) * (m_cfg->trueSizFrameY >> datShf));
            datPsnr[idxChn] = 20 * log10(((1 << JPG_DATA_PXL_WD) - 1) / (sqrt(datMse)));
            datPsnr[idxChn] = datPsnr[idxChn] < 60 ? datPsnr[idxChn] : 60 ;
        }
    }

    // calculate MS-SSIM
    double datSsim[JPG_NUMB_CHN] = {0};
    if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC && m_cfg->flgDistortion[1]) {
        for (int idxChn = 0; idxChn < m_cfg->numChn; ++idxChn)
            datSsim[idxChn] = 0;
    }

    // get bit
    double datBit = m_numFbsFrame * 8;

    // save
    cntFrameFull += 1;
    for (int c = 0; c < m_cfg->numChn; ++c) {
        datPsnrFull[c] += datPsnr[c];
        datSsimFull[c] += datSsim[c];
    }
    datBitFull += datBit;

    // to file
    if (m_cfg->flgDumpCsv) {
        fprintf(m_dumpFileCsvFpt, "%d, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3f\n"
            , m_cfg->idxFrame
            , (datPsnr[0] * datWeightY + datPsnr[1] + datPsnr[2]) / datWeightYuv, datPsnr[0], datPsnr[1], datPsnr[2]
            , (datSsim[0] * datWeightY + datSsim[1] + datSsim[2]) / datWeightYuv, datSsim[0], datSsim[1], datSsim[2]
            , datBit
        );
    }

    // to standard output
    if ((jpg_enmInfo_t)m_cfg->enmInfo >= jpg_enmInfo_t::TOP) {
        // head
        if (m_cfg->idxFrame == 0) {
            // line 0
            printf("+--------");
            if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC)
                printf("----------------------------------------------------------------------------");
            printf("+");
            cout << endl;

            // line 1
            printf("|        ");
            if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC)
                printf("    PSNR     PSNR     PSNR     PSNR   MS-SSIM   MS-SSIM   MS-SSIM   MS-SSIM |");
            else
                printf("|");
            cout << endl;

            // line 2
            printf("| Number ");
            if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC)
                printf("     AVE        Y        U        V       AVE         Y         U         V | Bit");
            else
                printf("|");
            cout << endl;

            // line 3
            printf("+--------");
            if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC)
                printf("----------------------------------------------------------------------------");
            printf("+");
            cout << endl;
        }

        // body
        printf("| %06d "
            , m_cfg->idxFrame
        );
        if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC) {
            printf("| %6.3lf | %6.3lf | %6.3lf | %6.3lf | %7.5lf | %7.5lf | %7.5lf | %7.5lf | %-.3lf"
                , (datPsnr[0] * datWeightY + datPsnr[1] + datPsnr[2]) / datWeightYuv, datPsnr[0], datPsnr[1], datPsnr[2]
                , (datSsim[0] * datWeightY + datSsim[1] + datSsim[2]) / datWeightYuv, datSsim[0], datSsim[1], datSsim[2]
                , datBit
            );
        }
        else {
            printf("|");
        }
        cout << endl;

        // tail
        if (m_cfg->idxFrame == m_cfg->numFrame - 1) {
            // line 0
            printf("+--------");
            if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC)
                printf("----------------------------------------------------------------------------");
            printf("+");
            cout << endl;

            // line 1
            if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC) {
                printf("* unit of PSNR is dB");
                cout << endl;
            }

            // line 2
            cout << endl;

            // line 3 to 6
            if ((jpg_enmModRun_t)m_cfg->enmModRun == jpg_enmModRun_t::ENC) {
                printf("%06d frames have been encoded,\n", cntFrameFull);
                printf("averaged PSNR of which are %6.3lf(A) / %6.3lf(Y) / %6.3lf(U) / %6.3lf(V) dB,\n"
                    , (   datPsnrFull[0] * datWeightY + datPsnrFull[1] + datPsnrFull[2]
                      ) / datWeightYuv   / cntFrameFull
                    ,     datPsnrFull[0] / cntFrameFull
                    ,     datPsnrFull[1] / cntFrameFull
                    ,     datPsnrFull[2] / cntFrameFull
                );
                printf("averaged MS-SSIM of which are %7.5lf(A) / %7.5lf(Y) / %7.5lf(U) / %7.5lf(V),\n"
                    , (   datSsimFull[0] * datWeightY + datSsimFull[1] + datSsimFull[2]
                      ) / datWeightYuv   / cntFrameFull
                    ,     datSsimFull[0] / cntFrameFull
                    ,     datSsimFull[1] / cntFrameFull
                    ,     datSsimFull[2] / cntFrameFull
                );
                printf("averaged bits of which is %.3lf.\n"
                    ,     datBitFull / cntFrameFull
                );
            }
            else {
                printf("%06d frames have been decoded.\n", cntFrameFull);
            }
            cout << endl;
        }
    }
}

// dumpDatOriFrame
void JPG_KNL::dumpDatOriFrame(
int idxChn)
{
    int    datShf    = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
    string strObj    = "datOriFrame";
    string strIdx[3] = {"Y", "U", "V"};
    string strTyp    = "S0I8F0";
    string strPst    = "dat";
    int    datMsk    = 0xff;
    string strFmt    = "%02x";
    int    siz0      = m_cfg->trueSizFrameX >> datShf;
    int    siz1      = m_cfg->trueSizFrameY >> datShf;
    int    idxDlt    = 1;
    JPG_FUNC_DUMP_D_2(m_dumpDatOriFrameFlg[idxChn]
        ,             m_dumpDatOriFrameFpt[idxChn]
        ,             strObj, strIdx[idxChn], strTyp, strPst
        ,             (*m_datOriFrame)[idxChn]
        ,             datMsk, strFmt
        ,             siz1, idxDlt
        ,             siz0, idxDlt
    );
}

// dumpDatRecFrame
void JPG_KNL::dumpDatRecFrame(
int idxChn)
{
    int    datShf    = idxChn == 0 ? 0 : JPG_DATA_SHF_CH;
    string strObj    = "datRecFrame";
    string strIdx[3] = {"Y", "U", "V"};
    string strTyp    = "S0I8F0";
    string strPst    = "dat";
    int    datMsk    = 0xff;
    string strFmt    = "%02x";
    int    siz0      = m_cfg->sizFrameX >> datShf;
    int    siz1      = m_cfg->sizFrameY >> datShf;
    int    idxDlt    = 1;
    JPG_FUNC_DUMP_D_2(m_dumpDatRecFrameFlg[idxChn]
        ,             m_dumpDatRecFrameFpt[idxChn]
        ,             strObj, strIdx[idxChn], strTyp, strPst
        ,             (*m_datRecFrame)[idxChn]
        ,             datMsk, strFmt
        ,             siz1, idxDlt
        ,             siz0, idxDlt
    );
}

// dumpDatSymbolDht
void JPG_KNL::dumpDatSymbolDht(
int idxLaC, int idxTypeCoe)
{
    string strObj      = "datSymbolDhtDec";
    string strIdx_0[2] = {"DC", "AC"};
    string strIdx_1[2] = {"LU", "CH"};
    string strIdx      = strIdx_0[idxTypeCoe] + "_" + strIdx_1[idxLaC];
    string strTyp    = "S0I8F0";
    string strPst    = "dat";
    int    datMsk    = 0xff;
    string strFmt    = "%02x";
    int    idxDlt    = 1;
    for (int i = 0; i < JPG_NUMB_SYMBOL_MAX; ++i) {
        JPG_FUNC_DUMP_D_0(m_dumpDatSymbolDhtFlg[idxLaC * JPG_NUMB_LAC + idxTypeCoe]
            ,             m_dumpDatSymbolDhtFpt[idxLaC * JPG_NUMB_LAC + idxTypeCoe]
            ,             strObj, strIdx, strTyp, strPst
            ,             jpg_m_datSymbolDht[idxTypeCoe][idxLaC][i]
            ,             datMsk, strFmt
        );
    }
}