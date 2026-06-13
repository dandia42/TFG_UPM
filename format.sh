#!/bin/bash

# Apply clang-format with Google style to .cpp and .h
find ./src -regex '.*\.\(cpp\|h\)' -exec clang-format -i -style=file {} \;
find ./include -regex '.*\.\(cpp\|h\)' -exec clang-format -i -style=file {} \;

echo "Format Google style."
