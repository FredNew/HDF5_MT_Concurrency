#! /bin/sh
#
# Copyright by The HDF Group.
# All rights reserved.
#
# This file is part of HDF5.  The full HDF5 copyright notice, including
# terms governing use, modification, and redistribution, is contained in
# the COPYING file, which can be found at the root of the source code
# distribution tree, or in https://www.hdfgroup.org/licenses.
# If you do not have access to either file, you may request a copy from
# help@hdfgroup.org.
#

# Tests for the h5dwalk tool

#
#

srcdir=.

# Determine which filters are available
USE_FILTER_SZIP="no"
USE_FILTER_DEFLATE="yes"


TESTNAME=h5dwalk

EXIT_SUCCESS=0
EXIT_FAILURE=1

THIS_DIR="`pwd`"
ROOTDIR="`cd ../../../..; pwd`"
cd "$THIS_DIR"
TOP_DIR="$ROOTDIR"


H5DWALK=../../h5dwalk/h5dwalk
H5DWALK_BIN="$TOP_DIR/utils/tools/h5dwalk/h5dwalk"


H5DUMP="$TOP_DIR/src/h5dump/h5dump"
H5DUMP_BIN="$TOP_DIR/tools/src/h5dump/h5dump"

RM='rm -rf'
CMP='cmp -s'
DIFF='diff -c'
CP='cp'
DIRNAME='dirname'
LS='ls'
AWK='awk'
WC='wc'

nerrors=0
verbose=yes

export LD_LIBRARY_PATH=

# source dirs
SRC_TOOLS="$TOP_DIR/tools/test"

# testfiles source dirs for tools
SRC_H5LS_TESTFILES="$SRC_TOOLS/test/h5ls/testfiles"
SRC_H5DUMP_TESTFILES="$SRC_TOOLS/test/h5dump/testfiles"
SRC_H5DIFF_TESTFILES="$SRC_TOOLS/h5diff/testfiles"
SRC_H5COPY_TESTFILES="$SRC_TOOLS/h5copy/testfiles"
SRC_H5REPACK_TESTFILES="$SRC_TOOLS/h5repack/testfiles"
SRC_H5JAM_TESTFILES="$SRC_TOOLS/h5jam/testfiles"
SRC_H5DWALK_TESTFILES="$SRC_TOOLS/h5dwalk/testfiles"
SRC_H5IMPORT_TESTFILES="$SRC_TOOLS/h5import/testfiles"

TESTDIR=./tmpdw
test -d $TESTDIR || mkdir $TESTDIR

echo "SRC_H5DIFF_TESTFILES = $SRC_H5DIFF_TESTFILES"
echo "Creating demo files"
. ./copy_demo_files.sh



CLEAN_TESTFILES_AND_TESTDIR()
{
    echo "cleaning logfiles"
    $RM $TESTDIR/*log*
}

# Print a line-line message left justified in a field of 70 characters
# beginning with the word "Testing".
#
TESTING() {
   SPACES="                                                               "
   echo "Testing $* $SPACES" | cut -c1-70 | tr -d '\012'
}

# Run a test and print PASS or *FAIL*.  If a test fails then increment
# the `nerrors' global variable and (if $verbose is set) display the
# difference between the actual output and the expected output. The
# expected output is given as the first argument to this function and
# the actual output file is calculated by replacing the `.ddl' with
# `.out'.  The actual output is not removed if $HDF5_NOCLEANUP has a
# non-zero value.
#
TOOLTEST() {
   expect="$TESTDIR/$1"
   expect_err="$TESTDIR/`basename $1`.err"
   actual="$TESTDIR/`basename $1`.out"
   actual_err="$TESTDIR/`basename $1`.out.err"
   actual_sav=${actual}-sav
   actual_err_sav=${actual_err}-sav
   shift

   # Run test.

   TESTING $H5DWALK $@
   (
      cd $TESTDIR
      $RUNSERIAL $H5DWALK_BIN $@
   ) 1> $actual 2> $actual_err

   # save actual and actual_err in case they are needed later.
   cp $actual $actual_sav
   cp $actual_err $actual_err_sav

   if [ ! -f $expect ]; then
      # Compare error files if the expect file doesn't exist.
      if $CMP $expect_err $actual_err; then
         echo " PASSED"
      else
         echo "*FAILED*"
         echo "    Expected result (*.err) differs from actual result (*.out.err)"
         nerrors="`expr $nerrors + 1`"
         test yes = "$verbose" && $DIFF $expect_err $actual_err |sed 's/^/    /'
      fi
   elif $CMP $expect $actual; then
      echo " PASSED"
   else
      echo "*FAILED*"
      echo "    Expected result (*.ddl) differs from actual result (*.out)"
      nerrors="`expr $nerrors + 1`"
      test yes = "$verbose" && $DIFF $expect $actual |sed 's/^/    /'
   fi

   # Clean up output file
   if test -z "$HDF5_NOCLEANUP"; then
      rm -f $actual $actual_err $actual_sav $actual_err_sav
   fi
}

TOOL_LOGTEST() {
   expect="$TESTDIR/`basename $1`.txt"
   expect_err="$TESTDIR/`basename $1`.err"
   actual="$TESTDIR/`basename $1`.log"
   actual_err="$TESTDIR/`basename $1`.out.err"
   actual_sav=${actual}-sav
   actual_err_sav=${actual_err}-sav
   shift

   echo "running logtest"

   # Run test.
   TESTING $H5DWALK $@
   (
      cd $TESTDIR
      $RUNSERIAL $H5DWALK_BIN $@

   ) 1> $actual 2> $actual_err
   expect_len="`wc -l < $expect`"

   if [ ! -f $actual ]; then
      echo "*FAILED*"
      echo "    The expected .log file is missing"
      echo "    Perhaps the test failed to run?"
   else
      actual_len="`wc -l < $actual`"
      if [ $actual_len -eq $expect_len ]; then
         echo " PASSED"
      else
         echo "*FAILED*"
         echo "    The generated .log file length does not match the expected length.  $actual_len != $expected_len"
      fi
   fi

   # Clean up output file
   if test -z "$HDF5_NOCLEANUP"; then
      rm -f $actual $actual_err $actual_sav $actual_err_sav
   fi
}

TOOL_CHK_LOGLEN() {
   expect=$1
   shift

   echo "running tool_chk_loglen"

   # Run test.
   TESTING $H5DWALK $@
   (
      cd $TESTDIR
      $RUNSERIAL $H5DWALK_BIN $@
   )
	   
   expect_len="`wc -l < $expect`"
   if [ "$expect_len" -gt 0 ]; then
	   echo " PASSED"
   else
       echo "*FAILED*"
       echo "    The generated .log file is empty!."
   fi

   # Clean up output file
   if test -z "$HDF5_NOCLEANUP"; then
      rm -f $expect
   fi
}


# Print a "SKIP" message
SKIP() {
    TESTING $H5DWALK $@6
    echo  " -SKIP-"
}



##############################################################################
##############################################################################
###              T H E   T E S T S                                ###
##############################################################################
##############################################################################

TOOLTEST help-1.txt -h
TOOLTEST help-2.txt --help
TOOL_LOGTEST h5diff_basic1.h5_h5dump -l -T $H5DUMP_BIN -n ./h5diff_basic1.h5
TOOL_CHK_LOGLEN showme-h5dump.log -o `pwd`/showme-h5dump.log -T $H5DUMP_BIN -n `pwd`


#
#
# Clean up temporary files/directories
CLEAN_TESTFILES_AND_TESTDIR

if test $nerrors -eq 0 ; then
    echo "All $TESTNAME tests passed."
    exit $EXIT_SUCCESS
else
    echo "$TESTNAME tests failed with $nerrors errors."
    exit $EXIT_FAILURE
fi

