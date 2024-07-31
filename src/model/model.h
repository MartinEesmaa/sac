#ifndef MODEL_H
#define MODEL_H

// probability precision
#define PBITS   (15)
#define PSCALE  (1<<PBITS)
#define PSCALEh (PSCALE>>1)
#define PSCALEm (PSCALE-1)

// weight precision
#define WBITS   (16)
#define WSCALE  (1<<WBITS)
#define WSCALEh (WSCALE>>1)

template <class T, class U, class V> T clamp(T val, U min, V max) {return val<static_cast<T>(min)?static_cast<T>(min):(val>static_cast<T>(max)?static_cast<T>(max):val);}

#endif // MODEL_H
