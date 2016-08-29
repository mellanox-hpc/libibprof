#!/bin/bash -xeE

if [ ! -d "$WORKSPACE" ]; then
    WORKSPACE=$PWD
    BUILD_NUMBER=1
fi

echo "Source location at $WORKSPACE"
cd $WORKSPACE

cppflags=""
make_opt="-j$(nproc)"
PKG_HOME=$WORKSPACE/install

if [ -d /opt/mellanox/hcoll ]; then
    cppflags="$cppflags -I/opt/mellanox/hcoll/include -DUSE_HCOL=1"
fi

if [ -d /opt/mellanox/mxm ]; then
    cppflags="$cppflags -I/opt/mellanox/mxm/include -DUSE_MXM=1"
fi

# Build from soures all available modules
echo "Build from soures"
./autogen.sh 
./configure CPPFLAGS="$cppflags" --prefix=$PKG_HOME
make $make_opt install
make $make_opt dist

if [ -x /usr/bin/dpkg-buildpackage ]; then
    if [ -d debian ]; then
        dpkg-buildpackage -us -uc
    else
        echo NO DEBIAN SUPPORT
    fi
else
  make $make_opt srcrpm 
  make $make_opt binrpm
fi

if [ -n "$JENKINS_RUN_TESTS" ]; then


  # Build using Intel compiler
  echo "Build with icc"
  module load intel/ics
  make $make_opt distclean 
  ./autogen.sh 
  ./configure CC=icc 
  make $make_opt all
  module unload intel/ics

cat >ibprof_sample.c<<'EOF'
#include <stdlib.h>

#include "ibprof_api.h"

#define TURNS 10

void foo(void)
{
        int i = 0;
        while (i++ < TURNS)
                sleep(1);
}

int main(void)
{
        ibprof_interval_start(10, "foo");
        foo();
        ibprof_interval_end(10);
        return 0;
}
EOF

  # Test user defined checkpoints
  echo "Test a user defined checkpoints"
  gcc -o $PKG_HOME/ibprof_sample ibprof_sample.c -I$PKG_HOME/include -L$PKG_HOME/lib -libprof -pthread -ldl
  LD_LIBRARY_PATH=$PKG_HOME/lib timeout 1m $PKG_HOME/ibprof_sample
  ret=$?
  rm -f ibprof_sample.tap
  echo "1..1" > ibprof_sample.tap
  if [ $ret -gt 0 ]; then
      echo "not ok 1 ibprof_sample" >> ibprof_sample.tap
  else
      echo "ok 1 ibprof_sample" >> ibprof_sample.tap
  fi
  rm -f $PKG_HOME/ibprof_sample

cat >mpi_hello.c<<'EOF'
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER     0

int main (int argc, char *argv[])
{
int  numtasks, taskid, len;
char hostname[MPI_MAX_PROCESSOR_NAME];
int  partner, message;
MPI_Status status;
MPI_Request reqs[2];

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Get_processor_name(hostname, &len);
printf ("Hello from task %d on %s!\n", taskid, hostname);
if (taskid == MASTER)
   printf("MASTER: Number of MPI tasks is: %d\n",numtasks);

/* determine partner and then send/receive with partner */
if (taskid < numtasks/2) 
  partner = numtasks/2 + taskid;
else if (taskid >= numtasks/2) 
  partner = taskid - numtasks/2;

MPI_Irecv(&message, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &reqs[0]);
MPI_Isend(&taskid, 1, MPI_INT, partner, 1, MPI_COMM_WORLD, &reqs[1]);

/* now block until requests are complete */
MPI_Waitall(2, reqs, &status);

/* print partner info and exit*/
printf("Task %d is partner with %d\n",taskid,message);

MPI_Finalize();

}
EOF

  # Test OMPI with MXM
  echo "Test OMPI with MXM"
  module load hpcx-gcc
  mkdir -p $PKG_HOME/bin
  mpicc -o $PKG_HOME/bin/mpi_hello mpi_hello.c
  PRELOAD=$PKG_HOME/lib/libibprof.so:$MXM_DIR/lib/libmxm.so
  hca_port=1
  test_idx=1
  HOSTNAME=$(hostname)
  NP=8
  mpirun_hosts=$( printf "${HOSTNAME}"; eval printf "%0.s,${HOSTNAME}" {1..$((NP-1))} )
  
  ndevs=$(ibstat -l | wc -w)
  rm -f mpi_hello.tap
  echo "1..$ndevs" > mpi_hello.tap
  set +e
  for dev in $(ibstat -l); do
      hca="${dev}:${hca_port}"
      LD_PRELOAD=$PRELOAD timeout 10m $MPI_HOME/bin/mpirun -x LD_PRELOAD -np $NP -H $mpirun_hosts -mca pml cm -mca mtl mxm -mca mtl_mxm_np 0 -x MXM_IB_PORTS=$hca -x MXM_TLS=ud,self $PKG_HOME/bin/mpi_hello
      ret=$?
      if [ $ret -gt 0 ]; then
          echo "not ok $test_idx mpi_hello $dev" >> mpi_hello.tap
      else
          echo "ok $test_idx mpi_hello $dev" >> mpi_hello.tap
      fi
      test_idx=$(($test_idx+1))
  done
  sеt -e
  # Test IBV
  echo "Test IBV"
  rm -f ibv_devinfo.tap
  LD_PRELOAD=$PRELOAD ibv_devinfo
  ret=$?
  echo "1..1" > ibv_devinfo.tap
  if [ $ret -gt 0 ]; then
      echo "not ok 1 ibv_devinfo" >> ibv_devinfo.tap
  else
      echo "ok 1 ibv_devinfo" >> ibv_devinfo.tap
  fi

  # Test Error-injection (0% of errors)
  echo "Test Error-injection with 0% of errors"
  rm -f ibv_err0.tap
  env IBPROF_ERR_PERCENT=0 IBPROF_MODE=USE_IBV=2 LD_PRELOAD=$PRELOAD ibv_devinfo
  ret=$?
  echo "1..1" > ibv_err0.tap
  if [ $ret -gt 0 ]; then
      echo "not ok 1 ibv_devinfo IBPROF_ERR_PERCENT=0" >> ibv_err0.tap
  else
      echo "ok 1 ibv_devinfo IBPROF_ERR_PERCENT=0" >> ibv_err0.tap
  fi

  # Test Error-injection (100% of errors)
  echo "Test Error-injection with 100% of errors"
  rm -f ibv_err1.tap
set +e
  env IBPROF_ERR_PERCENT=100 IBPROF_MODE=USE_IBV=2 LD_PRELOAD=$PRELOAD ibv_devinfo

  ret=$?
set -e
  echo "1..1" > ibv_err1.tap
  if [ $ret -eq 0 ]; then
      echo "not ok 1 ibv_devinfo IBPROF_ERR_PERCENT=100" >> ibv_err1.tap
  else
      echo "ok 1 ibv_devinfo IBPROF_ERR_PERCENT=100" >> ibv_err1.tap
  fi

  # Test COV
  module load tools/cov
  cov_build_id="cov_build_${BUILD_NUMBER}"
  cov_build="$WORKSPACE/$cov_build_id"
  rm -rf $cov_build
  make $make_opt clean
  make $make_opt distclean
  ./autogen.sh 
  ./configure
  cov-build --dir $cov_build make $make_opt all
  cov-analyze --dir $cov_build
  nerrors=$(cov-format-errors --dir $cov_build | awk '/Processing [0-9]+ errors?/ { print $2 }')

  index_html=$(cd $cov_build && find . -name index.html | cut -c 3-)
  cov_url="$JOB_URL/ws/$cov_build_id/${index_html}"

  echo "1..1" > coverity.tap
  if [ $nerrors -eq 0 ] ; then
      echo ok 1 Coverity found no issues >> coverity.tap
      nerrors=0
  else
      echo "not ok 1 Coverity Detected $nerrors failures # $cov_url" >> coverity.tap

      gh_cov_msg="$WORKSPACE/cov_file_${BUILD_NUMBER}.txt"
      echo "" > $gh_cov_msg
      echo "* Coverity found $nerrors errors, check $cov_url" >> $gh_cov_msg

      if [ -n "$ghprbPullId" -a -f "$gh_cov_msg" ]; then
          gh pr $ghprbPullId --comment "$(cat $gh_cov_msg)"
      fi
  fi
  module unload tools/cov

fi
