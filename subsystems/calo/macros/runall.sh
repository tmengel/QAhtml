#!/bin/bash

filelist="/sphenix/user/tmengel/offline-qa/myQAhtml/QAhtml/subsystems/calo/macros/run3pp.list"
for file in $(cat $filelist); do
    echo "Processing file: $file"
    root -b -q -l "checkall_calo.C(\"$file\")"
done