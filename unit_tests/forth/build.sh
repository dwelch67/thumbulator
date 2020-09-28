#/bin/sh

# Build a forth image and include the correct bootloader.

LOCAL=../../local/bin
# FORTHC=${LOCAL}/xArmCortexDevOSX
FORTHC=xArmCortexHfp_x64_mac.mo # 64-Bit Compiler for Catalina

README=ReadMe.md

# Set up the docgen dirs.
# [[ -d docgen ]] || mkdir docgen 
# [[ -d docgen/autogen ]] || mkdir docgen/autogen 

# If there is an existing readme.md, move it.
[[ -f $README ]] && mv $README $README.old
touch $README

DATE=$(date +%Y%m%d)

# Run this in batch mode.
${FORTHC} /PauseOff include testbed.ctl || exit 6


