#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <iostream> //헤더 추가
#include <chrono> //헤더 추가 
#include <unistd.h> //헤더 추가
#include <fstream> //헤더 추가
#include <thread> //헤더 추가

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>


#include <epicsExport.h>
#include "cpuPortDriver.h"


typedef struct stJiffies
{
        int user;
        int nice;
        int system;
        int idle;
}stJiffies;

//struct rusage r_usage;
//int *p = 0;


cpuPortDriver::cpuPortDriver(const char *portName) //두번째 인수 삭제
	: asynPortDriver(portName,
                    1, /* maxAddr */
                    asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask | asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask,  /* Interrupt mask */
                    ASYN_MULTIDEVICE, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0)
{
	createParam(CPUSTRING, asynParamFloat64, &CPU);
	createParam(MEMSTRING, asynParamFloat64, &MEM);
      //  createParam(CNTSTRING, asynParamFloat64, &CNT);


//	while(true){ //1에서 true로 변경
//		counter++;
//		sleep(1000); //소문자 s로 변경(리눅스 전용)

//		}


	
	//setIntegerParam(counter, 0); //초기값 추가
	//setIntegerParam(CPU, 0); //초기값 추가
}

asynStatus cpuPortDriver::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
	asynStatus status = asynSuccess; //return 할 변수
	int function = pasynUser -> reason;
	if (function == CPU){
//		counter += 1;		 
//		*value = counter; //변수 주소에서 역참조값으로 변경

	    	FILE *pStat = NULL;
	        char cpuID[6] = {0};

        	stJiffies curJiffies;
	        stJiffies prevJiffies;

		pStat = fopen("/proc/stat", "r");
                fscanf(pStat, "%s %d %d %d %d", cpuID, &curJiffies.user,
                        &curJiffies.nice, &curJiffies.system, &curJiffies.idle);

                stJiffies diffJiffies;
                diffJiffies.user =  curJiffies.user - prevJiffies.user;
                diffJiffies.nice =  curJiffies.nice - prevJiffies.nice;
                diffJiffies.system =  curJiffies.system - prevJiffies.system;
                diffJiffies.idle =  curJiffies.idle - prevJiffies.idle;

                int totalJiffies = diffJiffies.user + diffJiffies.nice + diffJiffies.system + diffJiffies.idle;

//                printf("Cpu usage : %.2f%%\n", 100.0f * ( 1.0-(diffJiffies.idle / (double) totalJiffies) ) );

                prevJiffies = curJiffies;
                fclose(pStat);

		*value = 100.0f * ( 1.0-(diffJiffies.idle / (double) totalJiffies) );

		
	}

	else if (function == MEM)
	{
		
        // Linux 시스템에서 주 메모리 정보를 얻기 위해 /proc/meminfo 파일 읽기
        std::ifstream memInfo("/proc/meminfo");
        
        if (memInfo.is_open()) {
            std::string line;
            while (std::getline(memInfo, line)) {
                if (line.find("MemAvailable:") != std::string::npos) {
                //    std::cout << "usable memory: " << line << std::endl;
				*value = std::stof(line.substr(14, line.length() - 17));
					
                }
            }
            memInfo.close();
        }

        
    
	}

        /*else if (function == CNT)
        {
                int counter = 0;

                while (1) {
        
                counter++;
                //std::cout << "counter: " << counter << std::endl;   
                *value = counter;
                
                }
        }*/
	return status; //return 없다고 에러나와서 추가
}

extern "C" {
	int cpuPortDriverConfigure(const char *portName){
		new cpuPortDriver(portName);
		
		return (asynSuccess);
}

static const iocshArg initArg0 = {"portName", iocshArgString};
static const iocshArg *const initArgs[] = {&initArg0};
static const iocshFuncDef initFuncDef = {"cpuPortDriverConfigure", 1, initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
	cpuPortDriverConfigure(args[0].sval);
}

void cpuAsynPortDriverRegister(void)
{
	iocshRegister(&initFuncDef, initCallFunc);
}

epicsExportRegistrar(cpuAsynPortDriverRegister);

}
