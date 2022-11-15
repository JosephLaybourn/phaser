// PhaserDemo.cpp
// -- driver for PhaserEffect
// cs246 11/19
//
// usage:
//   PhaserDemo [<file>]
// where:
//   <file> -- (optional) name of a WAVE file
// note:
//   if no file is specified, white noise is used

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <portaudio.h>
#include "PhaserEffect.h"
using namespace std;


/////////////////////////////////////////////////////////////////
// wrapper for effect
/////////////////////////////////////////////////////////////////
class PhaserDemo {
  public:
    PhaserDemo(float R, short *samples, unsigned count, bool stereo);
    static int onWrite(const void*,void*,unsigned long,
                       const PaStreamCallbackTimeInfo*,
                       PaStreamCallbackFlags,void*);
  private:
    short *samples;
    unsigned count, index;
    bool stereo;
    float rate;
    PhaserEffect phaser;
};


PhaserDemo::PhaserDemo(float R, short *s, unsigned cnt, bool st)
    : samples(s),
      count(cnt),
      index(0),
      stereo(st),
      rate(R) {
}


int PhaserDemo::onWrite(const void *vin, void *vout, unsigned long frames,
                        const PaStreamCallbackTimeInfo *tinfo,
                        PaStreamCallbackFlags flags, void *user) {
  const float IMAX = 1.0f/float(1<<15);

  PhaserDemo& demo = *reinterpret_cast<PhaserDemo*>(user);
  float *out = reinterpret_cast<float*>(vout);

  if (demo.stereo) {
    for (unsigned long i=0; i < frames; ++i) {
      float L = IMAX * demo.samples[2*demo.index+0],
            R = IMAX * demo.samples[2*demo.index+1],
            x = 0.5f*(L + R),
            y = demo.phaser(x);
      out[2*i+0] = y;
      out[2*i+1] = y;
      demo.index = (demo.index + 1) % demo.count;
    }
  }

  else {
    for (unsigned long i=0; i < frames; ++i) {
      float x = IMAX * demo.samples[demo.index];
      out[i] = demo.phaser(x);
      demo.index = (demo.index + 1) % demo.count;
    }
  }

  return paContinue;
}


/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  const int DEF_RATE = 44100;
  const float MAX = float((1<<15) - 1);

  if (argc != 1 && argc != 2)
    return 0;

  short *samples;
  unsigned channels,
           rate,
           count,
           size;

  // read in input file
  if (argc == 2) {
    fstream in(argv[1],ios_base::binary|ios_base::in);
    char header[44];
    in.read(header,44);
    channels = *reinterpret_cast<unsigned short*>(header+22),
    rate = *reinterpret_cast<unsigned*>(header+24),
    size = *reinterpret_cast<unsigned*>(header+40),
    count = size/(2*channels);
    samples = new short[channels*count];
    in.read(reinterpret_cast<char*>(samples),size);
    if (!in) {
      cout << "bad file: " << argv[1] << endl;
      return -1;
    }
  }

  // otherwise, generate white noise
  else {
    channels = 1;
    rate = DEF_RATE;
    count = rate;
    size = 2*count;
    samples = new short[count];
    for (int i=0; i < count; ++i)
      samples[i] = MAX * (1 - 2*float(rand())/float(RAND_MAX));
  }

  PhaserDemo effect(rate,samples,count,channels==2);

  Pa_Initialize();

  PaStreamParameters output_params;
  output_params.device = Pa_GetDefaultOutputDevice();
  output_params.channelCount = channels;
  output_params.sampleFormat = paFloat32;
  output_params.suggestedLatency = Pa_GetDeviceInfo(output_params.device)
                                   ->defaultLowOutputLatency;
  output_params.hostApiSpecificStreamInfo = 0;
  if (output_params.device == paNoDevice) {
    cout << "no output device" << endl;
    Pa_Terminate();
    return -2;
  }

  PaStream *stream;
  PaError error = Pa_OpenStream(&stream,0,&output_params,rate,0,0,
                                PhaserDemo::onWrite,&effect);
  if (error != paNoError) {
    cout << "failed to open audio stream" << endl;
    Pa_Terminate();
    return -3;
  }
  Pa_StartStream(stream);

  cin.get();

  Pa_StopStream(stream);
  Pa_CloseStream(stream);
  Pa_Terminate();

  delete[] samples;
  return 0;
}

