========================= =============== ====== ===== ======== ===== ======= =========== ===========================================================================================
 domain                    name            size   key   type     min   max     precision   description
========================= =============== ====== ===== ======== ===== ======= =========== ===========================================================================================
 virtual                   strFileCfg      /      c     /        /     /       /           full location of the configuration file
 virtual                   help            /      /     /        /     /       /           to print help information
 virtual                   show            /      /     /        /     /       /           to show configurations
 virtual                   dryRun          /      /     /        /     /       /           to check configurations without actual running
 changeable                idxFrame        1      /     int      /     /       /           index of the current frame
 changeable                idxMcuX         1      /     int      /     /       /           index of the current MCU in X (horizontal) direction
 changeable                idxMcuY         1      /     int      /     /       /           index of the current MCU in Y (vertical) direction
 changeable                idxMcuR         1      /     int      /     /       /           index of the current MCU in Y (scaler) direction
 changeable                trueSizMcuX     1      /     int      /     /       /           true size of the current MCU in X (horizontal) direction
 changeable                trueSizMcuY     1      /     int      /     /       /           true size of the current MCU in Y (vertical) direction
 changeable                idxBsCfg        1      /     int      /     /       /           read index pointer of full bitstream
 top                       strFileInp      1      i     string   /     /       /           full location of the input file
 top                       strFileOut      1      o     string   /     /       /           full location of the output file
 top                       strPrefixDump   1      d     string   /     /       /           dump prefix
 top                       strPrefixLoad   1      l     string   /     /       /           load prefixon)
 top                       flgDumpCsv      1      /     bool     /     /       /           enable flag for the dump of csv-format log
 top                       flgDumpYuvRec   1      /     bool     /     /       /           enable flag for the dump of reconstructed YUV
 top                       flgDumpDbg      1      /     bool     /     /       /           enable flag for the dump of debug information
 top                       flgDistortion   2      /     bool     /     /       /           enable flag for the distortion calculation (<index> 0/1: PSNR/MS-SSIM)
 top                       numFrame        1      f     int      1     /       /           number of frames to be processed
 top                       numChn          1      /     int      1     3       /           number of channels (Y/U/V)
 top                       idxFrameBgn     1      /     int      0     /       /           index of the first frame to be processed
 top                       sizFrameX       1      w     int      32    16384   /           size of frames in X (horizontal) direction
 top                       sizFrameY       1      h     int      32    16384   /           size of frames in Y (vertical) direction
 top                       sizMcu          1      /     int      16    16      /           size of the MCU
 top                       enmModRun       1      /     int      0     1       /           enumerator for the running mode (0: encode; 1: decode)
 top                       enmFormat       1      /     int      0     1       /           enumerator for color format (0: YUV420; 1: YUV422)
 top                       enmProc         1      e     int      0     1       /           enumerator for the processing behavior (<value> 0: software 1: hardware)
 top                       enmInfo         1      v     int      0     5       /           enumerator for the information level (<value> 0: none 1: top 2: kernel 3: unit 4: common)
 top                       enmDump         1      /     int      0     2       /           enumerator for data dumping (<value> 0: none 1: inout 2: intermediate)
 descreteCosineTransform   dctFlgProc      1      /     bool     /     /       /           enable flag for processing
 descreteCosineTransform   dctEnmLoad      1      /     int      0     2       /           enumerator for load source (<value> 0: pipeline; 2: file; 3: random)
 descreteCosineTransform   dctEnmDump      1      /     int      0     2       /           enumerator for data dump (<value> 0: none 1: inout 2: intermediate)
 huffmanEncoding           h_cFlgProc      1      /     bool     /     /       /           enable flag for processing
 huffmanEncoding           h_cEnmLoad      1      /     int      0     2       /           enumerator for load source (<value> 0: pipeline; 2: file; 3: random)
 huffmanEncoding           h_cEnmDump      1      /     int      0     2       /           enumerator for data dump (<value> 0: none 1: inout 2: intermediate)
 interface                 itfFlgProc      1      /     bool     /     /       /           enable flag for processing
 interface                 itfEnmLoad      1      /     int      0     2       /           enumerator for load source (<value> 0: pipeline; 2: file; 3: random)
 interface                 itfEnmDump      1      /     int      0     2       /           enumerator for data dump (<value> 0: none 1: inout 2: intermediate)
 derived                   numMcuX         1      /     int      /     /       /           number of MCUs in X (horizontal) direction of one frame
 derived                   numMcuY         1      /     int      /     /       /           number of MCUs in Y (vertical) direction of one frame
 derived                   numMcuR         1      /     int      /     /       /           number of MCUs in Y (scaler) direction of one frame
 derived                   numLaC          1      /     int      /     /       /           number of channels (luma/chroma)
 derived                   trueSizFrameX   1      w     int      32    16384   /           true size of frames in X (horizontal) direction
 derived                   trueSizFrameY   1      h     int      32    16384   /           true size of frames in Y (vertical) direction
========================= =============== ====== ===== ======== ===== ======= =========== ===========================================================================================
