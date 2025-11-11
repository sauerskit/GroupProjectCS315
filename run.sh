#! /bin/bash
rm output.zip

make && echo -e "\n==============================\nExecution:\n" && ./compression testFiles/asv.txt 1

ls -l output.zip | awk '{print $5}'
