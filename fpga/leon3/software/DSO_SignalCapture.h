/****************************************************************************
 * Project        : Welec W2000A
 *****************************************************************************
 * File           : DSO_SignalCapture.h
 * Author         : Alexander Lindert <alexander_lindert at gmx.at>
 * Date           : 20.04.2009
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
#ifndef DSO_SIGNALCAPTURE_H
#define DSO_SIGNALCAPTURE_H

#include "DSO_Main.h"
#include "DSO_SFR.h"
#include "types.h"
#include "crc.h"

typedef enum eSrc2
{
	normal, pwm_offset, gnd, lowpass
}
Src2;

typedef struct
{
	uint32_t myVperDiv;
	uint32_t AC;
	uint32_t Specific;
	int32_t DA_Offset;
	uint32_t BW_Limit;
	Src2 Mode;
} SetAnalog;

typedef struct TriggerSettings
{
	eTriggerTypes type;
	uint32_t edge;
	uint32_t channel;
	int32_t level;
	int32_t prefetch;
	int32_t schmitt;
	int32_t pulse;
}
sTriggerSettings;

uint32_t InitSignalCapture();

uint32_t FastMode(const uint32_t SamplingFrequncy, const uint32_t CPUFrequency);
uint32_t IsFastMode();

uint32_t SetTriggerInput(const uint32_t noChannels, const uint32_t SampleSize,
							const uint32_t SamplingFrequency,
							const uint32_t CPUFrequency,
							const uint32_t AACFilterStart,
							const uint32_t AACFilterStop, const uint32_t Ch0,
							const uint32_t Ch1, const uint32_t Ch2,
							const uint32_t Ch3);

/* reference time in samples*/
uint32_t SetTrigger(const uint32_t Trigger, const uint32_t ExtTrigger,
					const uint32_t TriggerChannel,
					const uint32_t TriggerPrefetchSamples,
					const int LowReference, const uint32_t LowReferenceTime,
					const int HighReference, const uint32_t HighReferenceTime);

/* time in ms */
#ifdef BOARD_COMPILATION
#define COIL_SWITCH_TIME     1000
#else
#define COIL_SWITCH_TIME     0
#endif

uint32_t SetDACOffset(uint32_t Ch, int32_t Offset);

/*
 uint32_t SetAnalogInputRange(
 const uint32_t NoCh,
 const SetAnalog * Settings);
 */

uint32_t SetAnalogInputRange(uint32_t ch, SetAnalog *settings);

/* returns read DWORDS*/
uint32_t CaptureData(const uint32_t WaitTime, /* just a integer */
uint32_t Start, uint32_t ForceFastMode, uint32_t CaptureSize, /* size in DWORDs*/
uint32_t * RawData);

int FastCapture(const uint32_t WaitTime, /* just a integer */
uint32_t CaptureSize, /* size in DWORDs*/
uint32_t * RawData);

#endif
