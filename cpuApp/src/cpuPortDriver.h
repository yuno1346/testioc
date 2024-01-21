#include "asynPortDriver.h"

#define CPUSTRING "CPU_RUN"
#define MEMSTRING "MEM_RUN"
//#define CNTSTRING "CNT_RUN"

class cpuPortDriver : public asynPortDriver {

public:
	cpuPortDriver(const char *portName);
		
	virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);
	void task(void);

protected: //멤버 접근 범위 수정
	int CPU;
	int counter;
	int MEM;
	//int CNT;
}; //세미콜론 추가
