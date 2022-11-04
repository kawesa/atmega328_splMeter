#ifndef FFTRADIX4_H_
#define FFTRADIX4_H_

#ifdef __cplusplus
 extern "C" {
#endif
  //functions
  extern void rev_bin(int *fr, int fft_n);
  extern void fft_radix4_I(int *fr, int *fi, int ldn);
#ifdef __cplusplus
}
#endif

#endif
