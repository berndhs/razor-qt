#!/bin/bash

function pack_archive () {
  local PREFIX FILES TARFILE
  PREFIX=$1
  TARFILE=$2
  FILES=$(git ls-files)
  tar  \
    --transform="s+^+${PREFIX}+"  \
    -jcf  ${TARFILE} \
    ${FILES}
}

function list_contains_count () {
  local FIRST REST
  FIRST=$1
  shift
  REST=$*
  LIST_COUNT=0
  for X in $REST; do
    if [ "X$X" == "X${FIRST}" ]; then
      LIST_COUNT=$((LIST_COUNT + 1))
    fi
  done
}

ALL_ARGS=$*
NAME=razorqt
PROJECT=${NAME}
CHANGELOG=${NAME}.changes
HARMAT_TEMP_DIR=${NAME}.orig
VERSION=$(sh version.sh)
echo "version is " $VERSION

ARCHIVE=${NAME}-${VERSION}
TARFILE=${ARCHIVE}.tar.bz2

pack_archive ${ARCHIVE}/ ${TARFILE}

RPM_PACKDIR=rpm_packaging

list_contains_count obs $ALL_ARGS
WANT_OBS=$LIST_COUNT
echo "WANT_OSC is $WANT_OBS"

echo ${VERSION} > ${RPM_PACKDIR}/pack-version
echo ${NAME} > ${RPM_PACKDIR}/pack-name

cp ${TARFILE} ${CHANGELOG} ${RPM_PACKDIR}
(
  cd ${RPM_PACKDIR}
  make spec
)
echo "done making spec file"
if [ 0$WANT_OBS -ge 1 ]; then
  echo "obs copy: "
  for OBS_DIR in suse_obs meego_obs ; do
    echo "Copy -------> " ${OBS_DIR}
    set -x
    cp ${TARFILE} \
       ${RPM_PACKDIR}/${PROJECT}.spec \
       ${CHANGELOG} \
       ${OBS_DIR}/ 
    set +x
  done
else
  echo "no obs copy requested"
fi

set +x


