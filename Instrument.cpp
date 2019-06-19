/*
 * Instrument.cpp
 *
 * Created: 24.05.2019 09:25:18
 *  Author: koehn
 */ 
#include <stdio.h>
#include <math.h>

#include <random>

#include "Instrument.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Instrument::LFO::LFO()
{
  reset();
}

Instrument::LFO::~LFO()
{
}

void Instrument::LFO::reset()
{
  type        = Type_None;
  pos         = 0;
  normValue   = 0.0;
}

void Instrument::LFO::create(Type type)
{
  reset();
  this->type= type;
}

void Instrument::LFO::setFreq(unsigned int freq, unsigned int samplingRate)
{
  frequence= freq;
  period   = (unsigned int)((float)samplingRate * 1000000.0 / frequence);
}

void Instrument::LFO::sync()
{
  pos= 0;
}

void Instrument::LFO::clock()
{
  if ( pos > period )
    pos= 0;

  unsigned int half= period / 2;

  switch (type)
  {
    case Type_Sinus:
    {
      float norm = (float) ( 2 * 3.14159265 / period );
      //float piPos= (float) ( ( (float)pos + ( (float)pos * 3.0/4.0 ) )  * norm );
      //normValue= (float) ( sin(piPos) * 0.5 + 0.5 );

      normValue= (float) sin(norm * (float)pos);
      break;
    }

    case Type_Triangle:
    {
      float m= (float)(2.0 / (float)period);

      if ( pos < half )
        normValue= pos * m;
      else
        normValue= (float)( pos * ( -1.0 * m ) + 2.0 );

      break;
    }

    case Type_Sawtooth:
    {
      float m= (float) ( 1.0 / (float)period );

      normValue= pos * m;

      break;
    }

    case Type_InvSawtooth:
    {
      float m= (float) ( 1.0 / (float)period );

      normValue= (float) ( pos * ( -1.0 * m ) + 1.0 );

      break;
    }

    case Type_Pulse:
      if ( pos < half )
        normValue= 1.0;
      else
        normValue= 0.0;
      break;

    default:
      break;
  }

  pos++;

}

float Instrument::LFO::getValue()
{
  return normValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Instrument::Control::Control()
{
  amplitude   = 0;
  offset      = 0;
  samplingRate= 0;

  lfo         = 0;
  
  reset();
}

void Instrument::Control::reset()
{
  if ( lfo != 0 )
    delete lfo;

  lfo= 0;

  type = Type_None;
  value= 0;
}

void Instrument::Control::setAmplitude(int amplitude)
{
  this->amplitude= amplitude;
}

void Instrument::Control::setSamplingRate(unsigned int samplingRate)
{
  this->samplingRate= samplingRate;
}

void Instrument::Control::setOffset(int offset)
{
  this->offset= offset;
}

void Instrument::Control::setLFO(LFO::Type type)
{
  reset();

  lfo= new LFO();
  lfo->create(type);
  this->type= Type_Lfo;
}

void Instrument::Control::setLFOFrequence(unsigned int frequence)
{
  if ( lfo != 0 )
    lfo->setFreq(frequence,samplingRate);
}

void Instrument::Control::setRandom()
{
  reset();
  type= Type_Random;
}

void Instrument::Control::setList()
{
  reset();
  type= Type_List;
}

void Instrument::Control::sync()
{
  switch (type)
  {
    case Type_Lfo:
      lfo->sync();
      break;
    case Type_Random:
      break;
    case Type_List:
      break;
    case Type_None:
    default:
      break;
  }
}

void Instrument::Control::clock()
{
  switch (type)
  {
    case Type_Lfo:
      lfo->clock();
      value= (int)( amplitude * lfo->getValue()) + offset;
      break;
    case Type_Random:
      value= (int) ( ( rand() /  ( (RAND_MAX + 1)  * ( 2 * amplitude ) ) ) - amplitude);
      break;
    case Type_List:
      value= 0;
      break;
    case Type_None:
    default:
      value= 0;
      break;
  }
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
  for ( unsigned int i= 0; i < SID_VOICE_NUM_REGS; i++ )
    regs[i]= 0;
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

void Instrument::activate(SIDIF *sidif, unsigned char voice)
{
  this->sidif= sidif;
  this->voice= voice;
  sidif->voices[this->voice].setRegs(&regs);
}

void Instrument::deactivate()
{
  sidif->voices[this->voice].getRegs(&regs);
}  

SIDIF::Voice *Instrument::getVoice()
{
  return &sidif->voices[this->voice];
}
 
Instrument::Control *Instrument::getControl(ControlType type)
{
  return &controls[type];
}

void Instrument::noteOn(unsigned char note)
{
  unsigned int fn= getFreqNum(note);

  SIDIF::Voice *voice= getVoice();
  if ( voice != 0 )
  {
    voice->setFrequence(fn);
    voice->setGate(true);
  }
}

void Instrument::noteOff()
{
  SIDIF::Voice *voice= getVoice();
  if ( voice != 0 )
    voice->setGate(false);
}

void Instrument::sync()
{
  for (int i = 0; i < 3; i++)
    controls[i].sync();
}

void Instrument::clock()
{
  SIDIF::Voice *voice= getVoice();

  if ( voice == 0 )
    return;

  for (int i = 0; i < 3; i++)
  {
    Control *ctrl= &controls[i];

    if ( !ctrl->isActive() )
      continue;

    ctrl->clock();

    int value= ctrl->getValue();

    switch ((Instrument::ControlType)i)
    {
      case ControlType_Frequency: 
        voice->setFrequence(value);
        break;
      case ControlType_PWM      :
        voice->setPulseWidth(value);
        break;
      case ControlType_Shape    :
        voice->setWaveform((SIDIF::Voice::eWaveform) value);
      default:
        break;
    }
  }
}
