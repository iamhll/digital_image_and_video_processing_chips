//------------------------------------------------------------------------------
    //
    //  Filename       : jpg_cfg.cpp
    //  Author         : Huang Leilei
    //  Status         : phase 000
    //  Created        : 2026-09-17
    //  Description    : configuration related codes
    //
//------------------------------------------------------------------------------

//*** INCLUDE ******************************************************************
#include "jpg_cfg.hpp"


//*** FUNCTION *****************************************************************
// jpg_cfgIni
int jpg_cfgIni(jpg_cfg_t *cfg,
int numArg, char *strArg[])
{
    // variables
    int    datRet;
    string strKey;

    // help
    for (int i = 0; i < numArg; ++i) {
        strKey = strArg[i];
        if (strKey == "--help") {
            jpg_cfgHlp(cfg);
            return 1;
        }
    }

    // reset
    jpg_cfgRst(cfg);

    // set from file
    datRet = jpg_cfgIniFromFile(cfg, numArg, strArg);
    if (datRet != 0)
        return datRet;

    // set from command line interface
    datRet = jpg_cfgIniFromCli(cfg, numArg, strArg);
    if (datRet != 0)
        return datRet;

    if ((jpg_enmModRun_t)cfg->enmModRun == jpg_enmModRun_t::DEC) {
        // set from command line interface
        datRet = jpg_cfgIniFromBs(cfg, numArg, strArg);
        if (datRet != 0)
            return datRet;
    }

    // derive parameter
    if ((jpg_enmModRun_t)cfg->enmModRun == jpg_enmModRun_t::ENC) {
        FILE *fpt = fopen(cfg->strFileInp.c_str(), "rb");
        assert(fpt != NULL);
        fseek(fpt, 0, SEEK_END);
        long siz = ftell(fpt);
        int numFrame = siz / (cfg->sizFrameX * cfg->sizFrameY * sizeof(jpg_pxl_t) * (1 + ( cfg->numChn     == 1 ? 0
                                                                                         : JPG_DATA_SHF_CH == 0 ? 2
                                                                                         : JPG_DATA_SHF_CH == 1 ? 0.5
                                                                                         : -1
                                                                                         )
                                                                                    )
                             )
                     - cfg->idxFrameBgn
        ;
        cfg->numFrame = cfg->numFrame < numFrame ? cfg->numFrame : numFrame;
    }
    cfg->numMcuX       = (cfg->sizFrameX + (cfg->sizMcu - 1)) / cfg->sizMcu;
    cfg->numMcuY       = (cfg->sizFrameY + (cfg->sizMcu - 1)) / cfg->sizMcu;
    cfg->numMcuR       = (jpg_enmFormat_t)cfg->enmFormat==jpg_enmFormat_t::YUV422 ? 2 : 1;
    cfg->numLaC        = cfg->numChn == 1 ? 1 : 2;
    cfg->trueSizFrameX = (cfg->sizFrameX +  2 - 1) /  2 *  2;
    cfg->trueSizFrameY = (cfg->sizFrameY +  2 - 1) /  2 *  2;
    cfg->sizFrameX     = (cfg->sizFrameX + 16 - 1) / 16 * 16;
    cfg->sizFrameY     = (cfg->sizFrameY + 16 - 1) / 16 * 16;

    // float2fix (with round operation)
    if ((jpg_enmProc_t)cfg->enmProc == jpg_enmProc_t::HARDWARE)
        jpg_cfgL2X(cfg);

    // check (with script)
    datRet = jpg_cfgChk(cfg);
    if (datRet != 0)
        return datRet;

    // log
    for (int i = 0; i < numArg; ++i) {
        strKey = strArg[i];
        if (strKey == "--show") {
            jpg_cfgLog(cfg);
        }
    }

    // dump
    jpg_cfgDmp(cfg);

    // return
    return 0;
}

// jpg_cfgIniFromFile
int jpg_cfgIniFromFile(jpg_cfg_t *cfg,
int numArg, char *strArg[])
{
    // variables
    string strFileCfg;
    string strKey, strDat;

    // main loop
    for (int idxArg = 0; idxArg < numArg; ++idxArg) {
        strKey = strArg[idxArg];
        if (strKey == "-c" || strKey == "--strFileCfg") {
            // open
            strFileCfg = strArg[idxArg + 1];
            fstream fptCfg;
            fptCfg.open(strFileCfg.c_str());
            if (!fptCfg.is_open()) {
                cout << "ERROR: cannot find configuartion file(" << strFileCfg << ")!" << endl;
                return 1;
            }

            // process
            fptCfg >> strKey;
            while (fptCfg >> strDat) {
                jpg_cfgMap(cfg, &strKey, &strDat);
                strKey = strDat;
            }

            // close
            fptCfg.close();
        }
    }

    // return
    return 0;
}


// jpg_cfgIniFromBs
// input: datFbs
// output: cfg idxFbs
int jpg_cfgIniFromBs(jpg_cfg_t *cfg,
int numArg, char *strArg[])
{
    // variables
    string strFileCfg = cfg->strFileInp + "_0.jpg";
    FILE* fptBs = fopen(strFileCfg.c_str(), "rb");
    cfg->idxBsCfg = 0;
    if(fptBs == NULL) {
        return 1;
    }

    int jpg_m_tmp_datCoeQtzZigzag[JPG_NUMB_LAC][JPG_NUMB_PXL_BLK];
    int jpg_m_tmp_numSymbolDht[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_SIZE_CODE_MAX  ];
    int jpg_m_tmp_datSymbolDht[(int)jpg_enmCoe_t::NUMB][JPG_NUMB_LAC][JPG_NUMB_SYMBOL_MAX];

    // read header
    memset(jpg_m_tmp_datCoeQtzZigzag, 0, sizeof(jpg_m_tmp_datCoeQtzZigzag));
    memset(jpg_m_tmp_numSymbolDht, 0, sizeof(jpg_m_tmp_numSymbolDht));
    memset(jpg_m_tmp_datSymbolDht, 0, sizeof(jpg_m_tmp_datSymbolDht));

    // read SOI
    {
        int datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord != JPG_DATA_HDR_SOI) {
            cout << "SOI check failed!" << endl;
            return 1;
        }
    }

    // read DQT
    {
        int datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord != JPG_DATA_HDR_DQT) {
            cout << "DQT check failed!" << endl;
            return 1;
        }
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        int numDqt = floor((double)(datWord - 2) / (1 + JPG_NUMB_PXL_BLK));
        if (numDqt > 2) {
            cout << "Too many DQT configurations!" << endl;
            return 1;
        }
        for (int idxDqt = 0; idxDqt < numDqt; ++idxDqt) {
            int idxTable = 0;
            assert(fread(&idxTable, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            for (int idxZigzag = 0; idxZigzag < JPG_NUMB_PXL_BLK; ++idxZigzag) {
                int datByte = 0;
                assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
                cfg->idxBsCfg++;
                jpg_m_tmp_datCoeQtzZigzag[idxTable][idxZigzag] = datByte;
            }
        }
    }

    // read DHT
    {
        int datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord != JPG_DATA_HDR_DHT) {
            cout << "DHT check failed!" << endl;
            return 1;
        }
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        for (int sizDht = datWord; sizDht >= JPG_SIZE_CODE_MAX; ) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            sizDht--;
            int enmCoe = datByte >> 4;
            int idxLaC = datByte % (1 << 4);
            if (enmCoe >= (int)jpg_enmCoe_t::NUMB || idxLaC >= JPG_NUMB_LAC) {
                cout << "DHT idx check failed!" << datByte << endl;
                return 1;
            }
            for(int sizCode = 0; sizCode < JPG_SIZE_CODE_MAX; ++sizCode) {
                assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
                cfg->idxBsCfg++;
                sizDht--;
                jpg_m_tmp_numSymbolDht[enmCoe][idxLaC][sizCode] = datByte;
            }
            int numSymbol = 0;
            for(int sizCode = 0; sizCode < JPG_SIZE_CODE_MAX; ++sizCode) {
                for (int idxSymbol = 0; idxSymbol < jpg_m_tmp_numSymbolDht[enmCoe][idxLaC][sizCode]; ++idxSymbol) {
                    assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
                    cfg->idxBsCfg++;
                    sizDht--;
                    jpg_m_tmp_datSymbolDht[enmCoe][idxLaC][numSymbol] = datByte;
                    numSymbol++;
                }
            }
        }
    }
    // read SOF
    {
        int datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord != JPG_DATA_HDR_SOF0) {
            cout << "SOF0 check failed!" << endl;
            return 1;
        }
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord != JPG_DATA_WORD_LOF0) {
            cout << "LOF0 check failed!" << endl;
            return 1;
        }
        int sizPxl = 0;
        assert(fread(&sizPxl, 1, 1, fptBs) == (size_t)1);
        cfg->idxBsCfg++;
        if (sizPxl != JPG_DATA_PXL_WD) {
            cout << "Pixel precision mismatch!" << endl;
            return 1;
        }
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        cfg->sizFrameY = datWord;
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        cfg->sizFrameX = datWord;
        int numChn = 0;
        assert(fread(&numChn, 1, 1, fptBs) == (size_t)1);
        cfg->idxBsCfg++;
        cfg->numChn = numChn;
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord == 0x0122) {
            cfg->enmFormat = (int)jpg_enmFormat_t::YUV420;
        }
        else if (datWord == 0x0121) {
            cfg->enmFormat = (int)jpg_enmFormat_t::YUV422;
        }
        else {
            cout << "Only YUV420/YUV422 Supported!" << endl;
            return 1;
        }
        int idxDqtPick = 0;
        assert(fread(&idxDqtPick, 1, 1, fptBs) == (size_t)1);
        cfg->idxBsCfg++;
        memcpy(jpg_m_datCoeQtzZigzag[0],jpg_m_tmp_datCoeQtzZigzag[idxDqtPick],sizeof(jpg_m_datCoeQtzZigzag[0]));
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord == 0x0211) {
        }
        else {
            cout << "Only YUV420/YUV422 Supported!" << endl;
            return 1;
        }
        assert(fread(&idxDqtPick, 1, 1, fptBs) == (size_t)1);
        cfg->idxBsCfg++;
        memcpy(jpg_m_datCoeQtzZigzag[1],jpg_m_tmp_datCoeQtzZigzag[idxDqtPick],sizeof(jpg_m_datCoeQtzZigzag[0]));
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord == 0x0311) {
        }
        else {
            cout << "Only YUV420/YUV422 Supported!" << endl;
            return 1;
        }
        assert(fread(&idxDqtPick, 1, 1, fptBs) == (size_t)1);// Channel U & V use the same table
        cfg->idxBsCfg++;
    }

    // read SOS
    {
        int datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord != JPG_DATA_HDR_SOS) {
            cout << "SOS check failed!" << endl;
            return 1;
        }
        datWord = 0;
        for (int i = 0; i < 2; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord != JPG_DATA_WORD_LOS) {
            cout << "LOS check failed!" << endl;
            return 1;
        }
        int idxChn = 0;
        int idxLaC = 0;
        assert(fread(&idxChn, 1, 1, fptBs) == (size_t)1);
        cfg->idxBsCfg++;
        if (idxChn != cfg->numChn) {
            cout << "Parameter numChn mismatch!" << endl;
            return 1;
        }
        int idxDhtPick = 0;
        int idxDhtPickDc = 0;
        int idxDhtPickAc = 0;
        for (int numChnCnt = 0; numChnCnt < cfg->numChn; ++numChnCnt) {
            assert(fread(&idxChn, 1, 1, fptBs) == (size_t)1);
            idxLaC = idxChn == 1 ? 0 : 1;
            cfg->idxBsCfg++;
            assert(fread(&idxDhtPick, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            idxDhtPickDc = idxDhtPick >> 4;
            idxDhtPickAc = idxDhtPick % (1 << 4);
            memcpy(jpg_m_numSymbolDht[(int)jpg_enmCoe_t::DC][idxLaC],jpg_m_tmp_numSymbolDht[(int)jpg_enmCoe_t::DC][idxDhtPickDc],sizeof(jpg_m_numSymbolDht[(int)jpg_enmCoe_t::DC][idxLaC]));
            memcpy(jpg_m_datSymbolDht[(int)jpg_enmCoe_t::DC][idxLaC],jpg_m_tmp_datSymbolDht[(int)jpg_enmCoe_t::DC][idxDhtPickDc],sizeof(jpg_m_datSymbolDht[(int)jpg_enmCoe_t::DC][idxLaC]));
            memcpy(jpg_m_numSymbolDht[(int)jpg_enmCoe_t::AC][idxLaC],jpg_m_tmp_numSymbolDht[(int)jpg_enmCoe_t::AC][idxDhtPickAc],sizeof(jpg_m_numSymbolDht[(int)jpg_enmCoe_t::AC][idxLaC]));
            memcpy(jpg_m_datSymbolDht[(int)jpg_enmCoe_t::AC][idxLaC],jpg_m_tmp_datSymbolDht[(int)jpg_enmCoe_t::AC][idxDhtPickAc],sizeof(jpg_m_datSymbolDht[(int)jpg_enmCoe_t::AC][idxLaC]));
        }
        datWord = 0;
        for (int i = 0; i < 3; ++i) {
            int datByte = 0;
            assert(fread(&datByte, 1, 1, fptBs) == (size_t)1);
            cfg->idxBsCfg++;
            datWord = (datWord << 8) + datByte;
        }
        if (datWord != 0x003f00) {
            cout << "Scan parameter {Ss,Se,Ah,Al} check failed!" << endl;
            return 1;
        }
    }
    // fclose(fptBs);
    // return
    return 0;
}

// jpg_cfgIniFromCli
int jpg_cfgIniFromCli(jpg_cfg_t *cfg,
int numArg, char *strArg[])
{
    // process
    for (int idxArg = 1; idxArg < numArg - 1; ++idxArg) {
        // variables
        string strKey = strArg[idxArg];
        string strDat = strArg[idxArg + 1];
        jpg_cfgMap(cfg, &strKey, &strDat);
    }

    // return
    return 0;
}
