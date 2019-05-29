/*
 * Instrument.h
 *
 * Created: 24.05.2019 09:25:06
 *  Author: koehn
 */ 


#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include "SID.h"
  
class Instrument
{
public:
  ///////////////////////////////////////////////////////////////////////
  struct Key {
    const unsigned int freq;
    const char         name[3];
  };
  
  ///////////////////////////////////////////////////////////////////////
  class LFO
  {
    public:
    enum Type {
      Type_Sinus,
      Type_Triangle,
      Type_Sawtooth,
      Type_InvSawtooth,
      Type_Pulse,
    };
  
    protected:
      Type          type;
      unsigned int  amplitude;
      int           offset;
      unsigned int  samplingRate;
      unsigned int  frequence;
      unsigned int  period;
      unsigned int  pos;
      float         normValue;
  
    public:
      void  reset();

      void  create(Type type, unsigned long amplitude, unsigned int offset, unsigned int samplingRate);
  
      void  setFreq(unsigned int freq);
      void  clock();

      void setValue(unsigned int value);  
      int getValue();
  
      LFO();
      virtual ~LFO();
  };  
  
  ///////////////////////////////////////////////////////////////////////
  class Control
  {
    public:
      enum Type {
        Type_Frequency= 0,
        Type_PWM      = 1,
        Type_Shape    = 2
      };
    
    protected:
      Type          type;
      LFO          *lfo;
      unsigned int  value;
      
    public:
      void switchLFO(int on);
      LFO *getLFO();
      void setValue(unsigned int value);  
      unsigned int getValue();

      Control();
  };
  
protected:
  SID           *sid;
  unsigned char  voice;
  Control        controls[3];
  static Key     keys[12];
  VoiceRegisters regs;

public:
  static void          getNoteName  (unsigned char note, char *name);
  static unsigned int  getFreqNum   (unsigned char note);
  static unsigned long calcFreq     (unsigned int  freqNum);
  static unsigned int  calcFreqNum  (unsigned long freq);  
  
public:
  void activate(SID *sid, unsigned char voice);
  void deactivate();
 
  Control *getControl(Control::Type type);

  void noteOn (unsigned char note);
  void noteOff();
  
  void clock();
  
  Instrument();
  virtual ~Instrument();
};

#endif /* INSTRUMENT_H_ */