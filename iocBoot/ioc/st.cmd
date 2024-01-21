#!../../bin/linux-x86_64/cpu

#- You may have to change cpu to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/cpu.dbd"
cpu_registerRecordDeviceDriver pdbbase

## Load record instances
#dbLoadTemplate "db/user.substitutions"
dbLoadRecords "db/cpu.db", "user=kimyunho, PORT=AA, ADDR=0, TIMEOUT=1"
#dbLoadRecords "db/dbSubExample.db", "user=kimyunho"

cpuPortDriverConfigure("AA")

#- Set this to see messages from mySub
#-var mySubDebug 1

#- Run this to trace the stages of iocInit
#-traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncExample, "user=kimyunho"
