/*
  ESPiLight - pilight 433.92 MHz protocols library for Arduino
  Copyright (c) 2016 Puuu.  All right reserved.

  Project home: https://github.com/puuu/espilight/
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with library. If not, see <http://www.gnu.org/licenses/>
*/

#include <ESPiLight.h>

struct protocols_t *protocols = NULL;

volatile PulseTrain_t ESPiLight::_pulseTrains[RECEIVER_BUFFER_SIZE];
boolean ESPiLight::_enabledReceiver;
volatile int ESPiLight::_actualPulseTrain = 0;
int ESPiLight::_avaiablePulseTrain = 0;
volatile unsigned long ESPiLight::_lastChange = 0; // Timestamp of previous edge
volatile uint8_t ESPiLight::_nrpulses = 0;

unsigned int ESPiLight::minrawlen = 5;
unsigned int ESPiLight::maxrawlen = MAXPULSESTREAMLENGTH;
unsigned int ESPiLight::mingaplen = 5100;
unsigned int ESPiLight::maxgaplen = 10000;

void ESPiLight::initReceiver(byte inputPin) {
  int interrupt = digitalPinToInterrupt(inputPin);

  resetReceiver();
  _enabledReceiver = true;

  if (interrupt >= 0) {
    attachInterrupt(interrupt, interruptHandler, CHANGE);
  }
}

int ESPiLight::receivePulseTrain(uint16_t *pulses) {
  int i = 0;
  int length = nextPulseTrainLength();

  if(length > 0) {
    volatile PulseTrain_t &pulseTrain = _pulseTrains[_avaiablePulseTrain];
    _avaiablePulseTrain = (_avaiablePulseTrain + 1)%RECEIVER_BUFFER_SIZE;
    for(i=0;i<length;i++) {
      pulses[i] = pulseTrain.pulses[i];
    }
    pulseTrain.length = 0;
  }
  return length;
}

int ESPiLight::nextPulseTrainLength() {
  return _pulseTrains[_avaiablePulseTrain].length;
}

void ESPiLight::interruptHandler() {
  if (!_enabledReceiver) {
    return;
  }

  unsigned long now = micros();
  unsigned int duration = 0;

  volatile PulseTrain_t &pulseTrain = _pulseTrains[_actualPulseTrain];
  volatile uint16_t *codes = pulseTrain.pulses;

  if(pulseTrain.length == 0) {
    duration = now - _lastChange;
    /* We first do some filtering (same as pilight BPF) */
    if(duration > MIN_PULSELENGTH) {
      if(duration < MAX_PULSELENGTH) {
	/* All codes are buffered */
	codes[_nrpulses] = duration;
	_nrpulses = (_nrpulses+1)%MAXPULSESTREAMLENGTH;
	/* Let's match footers */
	if(duration > mingaplen) {
	  //Serial.print('g');
	  /* Only match minimal length pulse streams */
	  if(_nrpulses >= minrawlen && _nrpulses <= maxrawlen) {
	    //Serial.print(_nrpulses);
	    //Serial.print('l');
	    pulseTrain.length = _nrpulses;
	    _actualPulseTrain = (_actualPulseTrain + 1)%RECEIVER_BUFFER_SIZE;
	  }
	  _nrpulses = 0;
	}
      }
      _lastChange = now;
    }
  }
  return;
}

void ESPiLight::resetReceiver() {
  int i = 0;
  for(i=0;i<RECEIVER_BUFFER_SIZE;i++) {
    _pulseTrains[i].length = 0;
  }
  _actualPulseTrain = 0;
  _nrpulses = 0;
}

void ESPiLight::loop() {
  int length = 0;
  uint16_t pulses[MAXPULSESTREAMLENGTH];

  length = receivePulseTrain(pulses);
  
  if(length > 0) {
    parsePulseTrain(pulses, length);
  }
}

ESPiLight::ESPiLight(byte outputPin) {
  _outputPin = outputPin;
  _rawCallback = NULL;

  pinMode(_outputPin, OUTPUT);
  digitalWrite(_outputPin, LOW);

}

void ESPiLight::setPulseTrainCallBack(PulseTrainCallBack rawCallback) {
  _rawCallback = rawCallback;
}

void ESPiLight::sendPulseTrain(const uint16_t *pulses, int length, int repeats) {
  boolean receiverState = _enabledReceiver;
  int r = 0, x = 0;
  if(_outputPin >= 0) {
    //disableReceiver()
    for(r=0;r<repeats;r++) {
      for(x=0;x<length;x+=2) {
	digitalWrite(_outputPin, HIGH);
	delayMicroseconds(pulses[x]);
	digitalWrite(_outputPin, LOW);
	if(x+1 < length) {
	  delayMicroseconds(pulses[x+1]);
	}
      }
    }
    digitalWrite(_outputPin, LOW);
    //if (receiverState) enableReceiver();
  }
  return;
}

int ESPiLight::parsePulseTrain(uint16_t *pulses, int length) {
  if (_rawCallback != NULL) {
    (_rawCallback)(pulses, length);
  }
  return 0;
}


String ESPiLight::pulseTrainToString(const uint16_t *codes, int length) {
  int i = 0, x = 0, match = 0;
  int diff=0;
  int plstypes[MAX_PULSE_TYPES];
  String data("");

  if (length <= 0) {
    return String("");
  }

  for(x=0;x<MAX_PULSE_TYPES;x++) {
    plstypes[x] = 0;
  }

  data.reserve(6+length);
  //Serial.print("pulseTrainToString: ");
  int p = 0;
  data += "c:";
  for(i=0;i<length;i++) {
    match = 0;
    for(x=0;x<MAX_PULSE_TYPES;x++) {
      /* We device these numbers by 10 to normalize them a bit */
      diff = (plstypes[x]/50)-(codes[i]/50);
      if((diff >= -2) && (diff <= 2)) {
	/* Write numbers */
	data += (char)('0'+((char)x));
	match = 1;
	break;
      }
    }
    if(match == 0) {
      plstypes[p++] = codes[i];
      data += (char)('0'+((char)(p-1)));
      if(p>=MAX_PULSE_TYPES) {
    	//Serial.println("RX: too many pulse types");
    	return String("\0");
      }
    }
  }
  data += ";p:";
  for(i=0;i<p;i++) {
    data += plstypes[i];
    if(i+1 < p) {
      data += ',';
    }
  }
  data += '@';
  return data;
}

int ESPiLight::stringToPulseTrain(const String &data, uint16_t *codes, int maxlength) {
  int start = 0, end = 0, pulse_index;
  int i = 0;
  int plstypes[MAX_PULSE_TYPES];
  char buf[10];

  for(i=0;i<MAX_PULSE_TYPES;i++) {
    plstypes[i] = 0;
  }

  int scode = data.indexOf('c')+2;
  int spulse = data.indexOf('p')+2;
  if(scode > 0 && scode < data.length() &&
     spulse > 0 && spulse < data.length()) {
    int nrpulses = 0;
    start = spulse;
    end = data.indexOf(',', start);
    while(end > 0) {
      plstypes[nrpulses++] = data.substring(start, end).toInt();
      start = end+1;
      end = data.indexOf(',', start);
    }
    end = data.indexOf(';', start);
    if(end<0)
      end = data.indexOf('@', start);
    if(end<0)
      return -2;
    plstypes[nrpulses++] = data.substring(start, end).toInt();

    int codelen = 0;
    for(i = scode; i < data.length(); i++) {
      if((data[i] == ';') || (data[i] == '@'))
	break;
      if(i >= maxlength)
	break;
      pulse_index = data[i] - '0';
      if((pulse_index < 0) || (pulse_index >= nrpulses))
	return -3;
      codes[codelen++] = plstypes[pulse_index];
    }
    return codelen;
  }
  return -1;
}