#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -soundhw pcspk
#-audiodev alsa,id=speaker -machine pcspk-audiodev=speaker
#-soundhw pcspk