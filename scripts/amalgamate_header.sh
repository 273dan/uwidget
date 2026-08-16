#!/bin/bash
if [[ $(basename $(pwd)) == "scripts" ]]; then
  echo "please run this with 'make amalgamate' from the project root"
  exit
fi

TARGET_DIR="./single_header/uwidget"
TARGET_NAME="uwidget.hpp"
TARGET="$TARGET_DIR/$TARGET_NAME"
TEMP_TARGET=$TARGET.tmp
INCLUDE_DIR="./include/uwidget"

SYS_INCLUDE_REGEX='^\s*#include\s*<'
ALL_INCLUDE_REGEX='^\s*#include\s*'


mkdir -p $TARGET_DIR

echo "// uwidget.hpp -- generated $(date +'%D %H:%M:%S')" > $TEMP_TARGET

echo "#pragma once" >> $TEMP_TARGET
echo "" >> $TEMP_TARGET

grep -h -E "$SYS_INCLUDE_REGEX" $INCLUDE_DIR/* | sort -u >> $TEMP_TARGET

append_header() {
  local file="$1"
  echo "// begin $(basename $file) -----------------" >> $TEMP_TARGET
  grep -v -E "${ALL_INCLUDE_REGEX}" ${file} \
  | grep -v "^\s*#pragma\s*once" \
  >> $TEMP_TARGET

  echo "// end $(basename $file) -----------------" >> $TEMP_TARGET
}

append_header "$INCLUDE_DIR/operation.hpp"
append_header "$INCLUDE_DIR/policy.hpp"
append_header "$INCLUDE_DIR/widget_exception.hpp"
append_header "$INCLUDE_DIR/detail.hpp"
append_header "$INCLUDE_DIR/widget.hpp"

if [[ -f "$TARGET" ]] && diff -q -I "^\s*//\suwidget\.hpp" "$TARGET" "$TEMP_TARGET"; then
  echo "no changes to amalgamated header"
  rm $TEMP_TARGET
else
  mv $TEMP_TARGET $TARGET
  echo "amalgamated header file generated at $TARGET"
fi





