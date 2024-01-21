TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

# build an ioc application

PROD_IOC = cpu
# cpu.dbd will be created and installed
DBD += cpu.dbd
cpu_DBD += cpuPortDriver.dbd
cpu_DBD += base.dbd

# cpu_registerRecordDeviceDriver.cpp will be created from cpu.dbd
cpu_SRCS += cpu_registerRecordDeviceDriver.cpp
cpu_SRCS_DEFAULT += cpuMain.cpp
cpu_SRCS += cpuPortDriver.cpp
cpu_SRCS_vxWorks += -nil-

# Add vxWorks support stuff
cpu_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Add the support libraries needed by this application
cpu_LIBS += asyn

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file:
ifneq ($(SNCSEQ),)
    cpu_DBD += sncExample.dbd
    cpu_SRCS += sncExample.stt
    cpu_LIBS += seq pv
endif

# Finally include the libraries from base
cpu_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

