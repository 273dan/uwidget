#!/bin/bash
if [[ $(basename $(pwd)) == "scripts" ]]; then
  echo "please run this with 'make amalgamate' from the project root"
  exit
fi

TARGET_DIR="./single_header/uwidget"
TARGET_NAME="uwidget.hpp"
TARGET="$TARGET_DIR/$TARGET_NAME"
INCLUDE_DIR="./include/uwidget"

SYS_INCLUDE_REGEX='^\s*#include\s*<'
ALL_INCLUDE_REGEX='^\s*#include\s*'


mkdir -p $TARGET_DIR

echo "// uwidget.hpp -- generated $(date +'%D %H:%M:%S')" > $TARGET

echo "#pragma once" >> $TARGET
echo "" >> $TARGET

grep -h -E "$SYS_INCLUDE_REGEX" $INCLUDE_DIR/* | sort -u >> $TARGET

append_header() {
  local file="$1"
  echo "// begin $(basename $file) -----------------" >> $TARGET
  grep -v -E "${ALL_INCLUDE_REGEX}" ${file} \
  | grep -v "^\s*#pragma\s*once" \
  >> $TARGET

  echo "// end $(basename $file) -----------------" >> $TARGET
}

append_header "$INCLUDE_DIR/operation.hpp"
append_header "$INCLUDE_DIR/policy.hpp"
append_header "$INCLUDE_DIR/widget_exception.hpp"
append_header "$INCLUDE_DIR/detail.hpp"
append_header "$INCLUDE_DIR/widget.hpp"

echo "amalgamated header file generated at $TARGET"



