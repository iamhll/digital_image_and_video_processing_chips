#!/bin/bash
#-------------------------------------------------------------------------------
    #
    #  Filename       : run.sh
    #  Author         : Huang Leilei
    #  Status         : phase 000
    #  Created        : 2026-09-17
    #  Description    : perform single run on xkjpg
    #
#-------------------------------------------------------------------------------

#*** PARAMETER *****************************************************************
CSTR_DIR_PRJ=../../..
CSTR_USAGE="[information from $0] usage: ./run.sh [-B <building parameters>] [-R <running parameters>] [-E <early termination flag>]"


#*** MAIN BODY *****************************************************************
# clean
rm -f xkjpg
rm -f xkjpg.bin
rm -f xkjpg.yuv
rm -f xkjpg.dec.yuv

# get building parameters B, running parameters R or encoding only flag E
while getopts ":B:R:E:" opt
do
    case $opt in
    B)
        B=$OPTARG
    ;;
    R)
        R=$OPTARG
    ;;
    E)
        E=$OPTARG
    ;;
    ?)
        echo "[error from $0] wrong parameter [$*]!" >& 2;
        echo $CSTR_USAGE >& 2
        exit 1
    ;;
    esac
done

# build
./build.sh $B
if [ $? -ne 0 ]
then
    echo "[error from $0] build failed!" >& 2;
    exit 1;
fi

# compile
make -j8
if [ $? -ne 0 ]
then
    echo "[error from $0] compile failed!" >& 2;
    exit 1;
fi

# run encoding
./xkjpg -c xkjpg.cfg --enmModRun 0 --enmProc 1 $R | tee run_enc.log
if [ $? -ne 0 ]
then
    echo "[error from $0] run failed!" >& 2;
    exit 1;
fi

# early terminate
if [ "$E" == 2 ]
then
    exit 0
fi

# check
ffmpeg -v 0 -i xkjpg_0.jpg xkjpg.dec.yuv -vsync passthrough
#diff -q xkjpg.yuv xkjpg.dec.yuv

# calculate md5sum
md5sum xkjpg_0.jpg   | tee -a run_enc.log
md5sum xkjpg.yuv     | tee -a run_enc.log
md5sum xkjpg.dec.yuv | tee -a run_enc.log

# early terminate
if [ "$E" == 1 ]
then
    exit 0
fi

# clean
rm -f xkjpg.dec.yuv

# run decoding
./xkjpg -c xkjpg.cfg --enmModRun 1 --enmProc 1 --strFileInp xkjpg --strFileOut xkjpg.dec.yuv $R | tee run_dec.log
if [ $? -ne 0 ]
then
    echo "[error from $0] run failed!" >& 2;
    exit 1;
fi

# check
diff -q xkjpg.yuv xkjpg.dec.yuv

# calculate md5sum
md5sum xkjpg.yuv     | tee -a run_dec.log
md5sum xkjpg.dec.yuv | tee -a run_dec.log

# return
exit 0;
