/*
 * Instrument.h
 *
 * Created: 24.05.2019 09:25:06
 *  Author: koehn
 */ 


#ifndef INSTRUMENT_H_
#define INSTRUMENT_H_

#include "SIDIF.h"

class Instrument
{
public:
  ///////////////////////////////////////////////////////////////////////
  struct Key {
    const unsigned int freq;
    const char         name[3];
  };

  enum ControlType {
    ControlType_Frequency= 0,
    ControlType_PWM      = 1,
    ControlType_Shape    = 2
  };
  
  ///////////////////////////////////////////////////////////////////////
  class LFO
  {
    public:
    enum Type {
      Type_None,
      Type_Sinus,
      Type_Triangle,
      Type_Sawtooth,
      Type_InvSawtooth,
      Type_Pulse,
    };
  
    protected:
      Type          type;
      unsigned int  frequence;
      unsigned int  period;
      unsigned int  pos;
      float         normValue;
  
    public:
      void  create(Type type);
  
      void  setFreq(unsigned int freq, unsigned int samplingRate);

      void  sync();
      void  clock();

      float getValue();

      void  reset();

      LFO();
      virtual ~LFO();
  };  
  
  ///////////////////////////////////////////////////////////////////////
  class Control
  {
    public:
      enum Type {
        Type_None,
        Type_Lfo,
        Type_Random,
        Type_List
      };

    protected:
      Type          type;
      unsigned int  samplingRate;

      LFO          *lfo;

      int           amplitude;
      int           offset;
      int           value;
      
    public:
      void setAmplitude(int amplitude);
      void setSamplingRate(unsigned int samplingRate);
      void setOffset(int offset);

      void setLFO(LFO::Type type);
      void setLFOFrequence(unsigned int frequence);

      void setRandom();
      void setList();

      void sync();
      void clock();

      inline bool isActive() { return (type != Type_None); }
      inline int  getValue() { return value; }

      void reset();

      Control();
  };
  
protected:
  SIDIF         *sidif;
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
  void activate(SIDIF *sidif, unsigned char voice);
  void deactivate();

  SIDIF::Voice *getVoice();
  Control *getControl(ControlType type);

  void noteOn (unsigned char note);
  void noteOff();
  
  void sync();
  void clock();
  
  Instrument();
  virtual ~Instrument();
};

#endif /* INSTRUMENT_H_ */