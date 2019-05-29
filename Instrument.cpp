/*
 * Instrument.cpp
 *
 * Created: 24.05.2019 09:25:18
 *  Author: koehn
 */ 
#include "Instrument.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Instrument::LFO::LFO()
{
}

Instrument::LFO::~LFO()
{
}

void Instrument::LFO::reset()
{
}

void Instrument::LFO::create(Type type, unsigned long range, unsigned int offset, unsigned int clockFreq)
{
}

void Instrument::LFO::setFreq(unsigned int freq)
{
}

void Instrument::LFO::clock()
{
}

void Instrument::LFO::setValue(unsigned int value)
{
}

int Instrument::LFO::getValue()
{
  return (( normValue * range ) + offset );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Instrument::LFO *Instrument::Control::getLFO()
{
  return lfo;
}

void Instrument::Control::setValue(unsigned int value)
{
  if ( lfo != 0 )
  lfo->setValue(value);
  else
  this->value= value;
}

unsigned int Instrument::Control::getValue()
{
  if ( lfo != 0 )
  return lfo->getValue();
  else
  return this->value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Instrument::Key Instrument::keys[]= {
  { 0x892B, "C"  },
  { 0x9153, "C#" },
  { 0x99F7, "D"  },
  { 0xA31F, "D#" },
  { 0xACD2, "E"  },
  { 0xB719, "F"  },
  { 0xC1FC, "F#" },
  { 0xC085, "G"  },
  { 0x0980, "G#" },
  { 0xE6B0, "A"  },
  { 0xF467, "A#" },
  { 0x02F0, "B"  }
};

Instrument::Instrument()
{
}

Instrument::~Instrument()
{
}

unsigned int Instrument::getFreqNum(unsigned char note)
{
  int  pos   = note % 12;
  char octave= note / 12;
  char shift = 7 - octave;
  
  unsigned int fn= keys[pos].freq;
  
  if ( shift > 0 ) 
    fn= fn >> shift;
    
  if ( pos == 11 )
    fn= fn | 0x8000;

  return fn;  
}

void Instrument::getNoteName(unsigned char note, char *name)
{
  int  pos   = note % 12;
  char octave= note / 12;

  name[0]= keys[pos].name[0];
  name[1]= 0x30 + octave;
  name[2]= keys[pos].name[1];
  name[3]= 0;
}

unsigned long Instrument::calcFreq(unsigned int freqNum)
{
  return (unsigned long)((float)freqNum * 0.0596); // Fout= (Fn * 0.0596) Hz
}

unsigned int Instrument::calcFreqNum(unsigned long freq)
{
  return (unsigned int)((float)freq / 0.0596); // Fn  = ( Fout / 0.0596 ) Hz
}

void Instrument::activate(SID *sid, unsigned char voice)
{
  this->sid  = sid;
  this->voice= voice;
  sid->voices[this->voice].setRegs(&regs);
}

void Instrument::deactivate()
{
  sid->voices[this->voice].getRegs(&regs);
}  
 
Instrument::Control *Instrument::getControl(Control::Type type)
{
  return 0;
}

void Instrument::noteOn(unsigned char note)
{
  unsigned int fn= getFreqNum(note);
  sid->voices[voice].setFrequence(fn);
}

void Instrument::noteOff()
{
  
}

void Instrument::clock()
{
  
}
