#!/usr/bin/env bash

PROXYGEN_VERION="2018.08.20.00"

## Run this script to build proxygen and run the tests. If you want to
## install proxygen to use in another C++ project on this machine, run
## the sibling file `reinstall.sh`.

# Parse args
JOBS=8
USAGE="./deps.sh [-j num_jobs]"
while [ "$1" != "" ]; do
  case $1 in
    -j | --jobs ) shift
                  JOBS=$1
                  ;;
    * )           echo $USAGE
                  exit 1
esac
shift
done


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
pushd ${DIR}/scratch

if [ ! -d proxygen-${PROXYGEN_VERION} ]; then
    echo "downloading proxygen"
    curl -Lo proxygen.tar.gz https://github.com/facebook/proxygen/archive/v${PROXYGEN_VERION}.tar.gz
    tar -xjvpf proxygen.tar.gz
fi

pushd proxygen-${PROXYGEN_VERION}/proxygen

set -e
start_dir=$(pwd)
trap 'cd $start_dir' EXIT

folly_rev=$(sed 's/Subproject commit //' "$start_dir"/../build/deps/github_hashes/facebook/folly-rev.txt)
wangle_rev=$(sed 's/Subproject commit //' "$start_dir"/../build/deps/github_hashes/facebook/wangle-rev.txt)

popd

# Some extra dependencies
declare -a packages=("git" "cmake" "pkg-config" "double-conversion" "glog" "gflags" "boost" "libevent" "xz" "snappy" "lz4" "openssl" "automake" "md5sha1sum")
for i in "${packages[@]}"
do
   echo "$i"
   if brew ls --versions $i > /dev/null; then
    echo "package: $i already installed"
   else
      echo "installing package: $i"
      brew install $i
   fi
done


# Get folly
if [ ! -e folly/folly ]; then
    echo "Cloning folly"
    git clone https://github.com/facebook/folly
fi
pushd folly
git fetch
git checkout "$folly_rev"

# Build folly
mkdir -p _build
pushd _build
cmake configure .. -DFOLLY_USE_JEMALLOC=OFF -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl -DBUILD_SHARED_LIBS=ON -DCOMPILER_HAS_F_ALIGNED_NEW=OFF
make -j$JOBS
sudo make install

if test $? -ne 0; then
  echo "fatal: folly build failed"
  exit -1
fi
popd
popd

# Get wangle
if [ ! -e wangle/wangle ]; then
    echo "Cloning wangle"
    git clone https://github.com/facebook/wangle
fi
pushd wangle
git fetch
git checkout "$wangle_rev"

# Build wangle
mkdir -p _build
pushd _build
cmake configure ../wangle -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl -DBUILD_SHARED_LIBS=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON
make -j$JOBS
sudo make install

if test $? -ne 0; then
  echo "fatal: wangle build failed"
  exit -1
fi
popd
popd
pushd proxygen-${PROXYGEN_VERION}/proxygen

# Build proxygen
patch -p0 <<'EOF'
--- configure.ac
+++ configure.ac
@@ -206,7 +206,7 @@ if test x"$HAVE_GPERF" != x"yes"; then
 fi

 LIBS="$LIBS $BOOST_LDFLAGS -lpthread -pthread -lfolly -lglog"
-LIBS="$LIBS -ldouble-conversion -lboost_system -lboost_thread"
+LIBS="$LIBS -ldouble-conversion -lboost_system -lboost_thread-mt"

 AM_CONDITIONAL([HAVE_STD_THREAD], [test "$ac_cv_header_features" = "yes"])
 AM_CONDITIONAL([HAVE_X86_64], [test "$build_cpu" = "x86_64"])
EOF
patch -p0 <<'EOF'
--- lib/test/Makefile.am
+++ lib/test/Makefile.am
@@ -9,7 +9,7 @@ release-1.8.0.zip:
 # `make` would run `sha1sum` **before** wget.  We'd re-extract the archive
 # without `touch`, since the contents' timestamps are older than the zip's.
 googletest-release-1.8.0/googletest/src/gtest-all.cc: release-1.8.0.zip
-	[ "$(shell sha1sum release-1.8.0.zip | awk '{print $$1}')" == \
+	[ "$(shell shasum release-1.8.0.zip | awk '{print $$1}')" == \
 	    "667f873ab7a4d246062565fad32fb6d8e203ee73" ]
 	unzip release-1.8.0.zip
 	touch googletest-release-1.8.0/googletest/src/gtest-all.cc
EOF
autoreconf -ivf
LDFLAGS="-L/usr/local/lib -L/usr/local/opt/boost/lib -L/usr/local/opt/openssl/lib" CPPFLAGS="-I/usr/local/include -I/usr/local/opt/boost/include -I/usr/local/opt/openssl/include" ./configure
make -j$JOBS

# Run tests
#LD_LIBRARY_PATH=/usr/local/lib make check

# Install the libs
sudo make install
popd
