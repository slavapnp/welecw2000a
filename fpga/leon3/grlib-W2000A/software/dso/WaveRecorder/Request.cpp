/****************************************************************************
* Project        : Welec W2000A
*****************************************************************************
* File           : Request.cpp
* Author		 : Alexander Lindert <alexander_lindert at gmx.at>
* Date           : 28.06.2009
*****************************************************************************
* Description	 : 
*****************************************************************************

*  Copyright (c) 2009, Alexander Lindert

*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.

*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.

*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*  For commercial applications where source-code distribution is not
*  desirable or possible, I offer low-cost commercial IP licenses.
*  Please contact me per mail.

*****************************************************************************
* Remarks		: -
* Revision		: 0
****************************************************************************/
#include "Request.h"
#include "Communication.h"
#include "DSO_Remote.h"

Request::Request(Communication * Comm)
: mComm(Comm){}
Request::~Request(){
	delete mComm;
}

uint32_t Request::InitComm(
		char * Device, 
		const uint32_t TimeoutMS, 
		const uint32_t Baudrate,
		char * IPAddr)  
{
	return mComm->Init(Device,TimeoutMS,Baudrate,IPAddr);
}

uint32_t Request::SendTriggerInput (
			const uint32_t noChannels, 
			const uint32_t SampleSize, 
			const uint32_t SamplingFrequency,
			const uint32_t AACFilterStart,
			const uint32_t AACFilterStop,
			const uint32_t Ch0, 
			const uint32_t Ch1, 
			const uint32_t Ch2, 
			const uint32_t Ch3){
	uint32_t data[] = {		
		SET_TRIGGER_INPUT, noChannels, SampleSize, SamplingFrequency, 
		AACFilterStart,AACFilterStop, Ch0, Ch1, Ch2, Ch3};
	mComm->Send(data,sizeof(data)/sizeof(uint32_t));
	return mComm->GetACK();
}

uint32_t Request::SendTrigger(
		const uint32_t Trigger, 
		const uint32_t ExtTrigger,
		const uint32_t TriggerChannel,
		const uint32_t TriggerPrefetchSamples,
		const int  LowReference,
		const uint32_t  LowReferenceTime,
		const int HighReference,
		const uint32_t HighReferenceTime){
	uint32_t data[] = {
		SET_TRIGGER, Trigger, ExtTrigger, TriggerChannel, TriggerPrefetchSamples,
		LowReference, LowReferenceTime, HighReference, HighReferenceTime};
	mComm->Send(data,sizeof(data)/sizeof(uint32_t));
	return mComm->GetACK(); 
}


typedef struct {
	uint32_t cmd;
	uint32_t NoCh;
	SetAnalog data[4];
} xAnalog;

uint32_t Request::SendAnalogInput(
			const uint32_t NoCh, 
			const SetAnalog * Settings){
	uint32_t i = 0;
	xAnalog crcA;
	for(i = 0; i < NoCh; ++i){
		crcA.data[i] = Settings[i];
	}
	crcA.cmd = SET_ANALOG_INPUT;
	crcA.NoCh = NoCh;
	mComm->Send((uint32_t*)&crcA, 2+(NoCh*sizeof(SetAnalog)/sizeof(uint32_t)));
	return mComm->GetACK();
}

uint32_t Request::ReceiveSamples(
			const uint32_t WaitTime, /* just a integer */
			const uint32_t Start,
			uint32_t CaptureSize,    /* size in DWORDs*/
			uint32_t * FastMode,
			uint32_t * RawData){
	uint32_t data[] ={CAPTURE_DATA, WaitTime, Start, *FastMode, CaptureSize};
	PrintSFR();
	if (mComm->Send(data,sizeof(data)/sizeof(uint32_t))){
		uint32_t * SendData = new uint32_t[CaptureSize+2];
		
		uint32_t Length = mComm->Receive(SendData,CaptureSize+2);
		if (Length < 2){
			return 0;
		}
		memcpy(RawData,&SendData[2],Length-2);
		uint32_t Ret = SendData[1];
		*FastMode = SendData[0];
		delete SendData;
		return Ret;
	} else {
		return 0;
	}

}

void Request::PrintSFR(){
	uint32_t data[DSO_REG_SIZE/sizeof(uint32_t)+2] = 
		{LOAD_DWORDS, DSO_SFR_BASE_ADDR, DSO_REG_SIZE/sizeof(uint32_t)};
	uint32_t Length = 3;
	uint32_t Dummy = 0;
	mComm->Send(data,Length);
	
	data[0] = DSO_REG_SIZE/sizeof(uint32_t)+1;/* length */
	Length = mComm->Receive(data,data[0]);
	if (Length > 1){
		PrintDesc(&data[1], data[0]);
	}
}

void Request::PrintDesc(uint32_t *Data, uint32_t Length){
	static const char Desc[32][40] = {
             "DEVICEADDR                0 = %d\n",
			 "INTERRUPTADDR             1 = %#x\n",
			 "INTERRUPTMASKADDR         2 = %#x\n",
			 "NOT IMPLEMENTED           3 = %#x\n",
			 "SAMPLINGFREQADDR          4 = %#x\n",
			 "FILTERENABLEADDR          5 = %#x\n",
			 "EXTTRIGGERSRCADDR         6 = %#x\n",
			 "EXTTRIGGERPWMADDR         7 = %#x\n",
			 "INPUTCH0ADDR              8 = %#x\n",
			 "INPUTCH1ADDR              9 = %#x\n",
			 "INPUTCH2ADDR             10 = %#x\n",
			 "INPUTCH3ADDR             11 = %#x\n",
			 "TRIGGERONCHADDR          12 = %#x\n",
			 "TRIGGERONCEADDR          13 = %#x\n",
             "TRIGGERPREFETCHADDR      14 = %d\n",
			 "TRIGGERSTORAGEMODEADDR   15 = %#x\n",
			 "TRIGGERREADOFFSETADDR    16 = %#x\n",
			 "TRIGGERTYPEADDR          17 = %#x\n",
			 "TRIGGERLOWVALUEADDR      18 = %d\n",
			 "TRIGGERLOWTIMEADDR       19 = %d\n",
			 "TRIGGERHIGHVALUEADDR     20 = %d\n",
			 "TRIGGERHIGHTIMEADDR      21 = %d\n",
             "TRIGGERSTATUSREGISTER    22 = %d\n",
			 "TRIGGERCURRENTADDR       23 = %#x\n",
			 "CONFIGADCENABLE          24 = %#x\n",
			 "LEDADDR                  25 = %#x\n",
			 "KEYADDR0                 26 = %#x\n",
			 "KEYADDR1                 27 = %#x\n",
			 "ANALOGSETTINGSPWMADDR    28 = %#x\n",
			 "ANALOGSETTINGSBANK7      29 = %#x\n", 
			 "ANALOGSETTINGSBANK6      30 = %#x\n", 
			 "ANALOGSETTINGSBANK5      31 = %#x\n"
	};
	if (Length > (DSO_REG_SIZE/sizeof(uint32_t))){
		Length = DSO_REG_SIZE/sizeof(uint32_t);
	}
	printf("\n\n   DSO Special Function Register \n");
	for (uint32_t i = 0; i < Length; ++i){
		printf(Desc[i],Data[i]);
	}
	printf("\n\n");
}