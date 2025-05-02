#!/bin/bash

INC_FILES=$(find ./inc -type f \( -name "*.h" \))
SRC_FILES=$(find ./src -type f \( -name "*.cpp" \))

vim makefile $INC_FILES $SRC_FILES
