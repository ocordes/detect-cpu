#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* this code is heavily inspired by
  https://github.com/Mysticial/FeatureDetector
*/


/* read
  https://dox.ipxe.org/cpuid_8h_source.html
*/


#ifdef _WIN32

/*  Windows */
#define cpuid(info, x)    __cpuidex(info, x, 0)

#else

/*  GCC Intrinsics */
#include <cpuid.h>
void cpuid(int info[4], int InfoType){
    __cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
}

#endif


/*  Misc. */
int HW_MMX;
int HW_x64;
int HW_ABM;      /* Advanced Bit Manipulation */
int HW_RDRAND;
int HW_BMI1;
int HW_BMI2;
int HW_ADX;
int HW_PREFETCHWT1;

/*  SIMD: 128-bit */
int HW_SSE;
int HW_SSE2;
int HW_SSE3;
int HW_SSSE3;
int HW_SSE41;
int HW_SSE42;
int HW_SSE4a;
int HW_AES;
int HW_SHA;

/*  SIMD: 256-bit */
int HW_AVX;
int HW_XOP;
int HW_FMA3;
int HW_FMA4;
int HW_AVX2;

/*  SIMD: 512-bit */
int HW_AVX512F;    /*  AVX512 Foundation */
int HW_AVX512CD;   /*  AVX512 Conflict Detection */
int HW_AVX512PF;   /*  AVX512 Prefetch */
int HW_AVX512ER;   /*  AVX512 Exponential + Reciprocal */
int HW_AVX512VL;   /*  AVX512 Vector Length Extensions */
int HW_AVX512BW;   /*  AVX512 Byte + Word */
int HW_AVX512DQ;   /*  AVX512 Doubleword + Quadword */
int HW_AVX512IFMA; /*  AVX512 Integer 52-bit Fused Multiply-Add */
int HW_AVX512VBMI; /*  AVX512 Vector Byte Manipulation Instructions */


void get_cpu_flags(void)
{
  int info[4];
  int nIds;
  unsigned nExIds;

  cpuid(info, 0);
  nIds = info[0];

  cpuid(info, 0x80000000);
  nExIds = info[0];

  /* Detect Features */
  if (nIds >= 0x00000001)
  {
    cpuid(info,0x00000001);
    HW_MMX    = (info[3] & ((int)1 << 23)) != 0;
    HW_SSE    = (info[3] & ((int)1 << 25)) != 0;
    HW_SSE2   = (info[3] & ((int)1 << 26)) != 0;
    HW_SSE3   = (info[2] & ((int)1 <<  0)) != 0;

    HW_SSSE3  = (info[2] & ((int)1 <<  9)) != 0;
    HW_SSE41  = (info[2] & ((int)1 << 19)) != 0;
    HW_SSE42  = (info[2] & ((int)1 << 20)) != 0;
    HW_AES    = (info[2] & ((int)1 << 25)) != 0;

    HW_AVX    = (info[2] & ((int)1 << 28)) != 0;
    HW_FMA3   = (info[2] & ((int)1 << 12)) != 0;

    HW_RDRAND = (info[2] & ((int)1 << 30)) != 0;
  }

  if (nIds >= 0x00000007)
  {
    cpuid(info,0x00000007);
    HW_AVX2   = (info[1] & ((int)1 <<  5)) != 0;

    HW_BMI1        = (info[1] & ((int)1 <<  3)) != 0;
    HW_BMI2        = (info[1] & ((int)1 <<  8)) != 0;
    HW_ADX         = (info[1] & ((int)1 << 19)) != 0;
    HW_SHA         = (info[1] & ((int)1 << 29)) != 0;
    HW_PREFETCHWT1 = (info[2] & ((int)1 <<  0)) != 0;

    HW_AVX512F     = (info[1] & ((int)1 << 16)) != 0;
    HW_AVX512CD    = (info[1] & ((int)1 << 28)) != 0;
    HW_AVX512PF    = (info[1] & ((int)1 << 26)) != 0;
    HW_AVX512ER    = (info[1] & ((int)1 << 27)) != 0;
    HW_AVX512VL    = (info[1] & ((int)1 << 31)) != 0;
    HW_AVX512BW    = (info[1] & ((int)1 << 30)) != 0;
    HW_AVX512DQ    = (info[1] & ((int)1 << 17)) != 0;
    HW_AVX512IFMA  = (info[1] & ((int)1 << 21)) != 0;
    HW_AVX512VBMI  = (info[2] & ((int)1 <<  1)) != 0;
  }

  if (nExIds >= 0x80000001)
  {
    cpuid(info,0x80000001);
    HW_x64   = (info[3] & ((int)1 << 29)) != 0;
    HW_ABM   = (info[2] & ((int)1 <<  5)) != 0;
    HW_SSE4a = (info[2] & ((int)1 <<  6)) != 0;
    HW_FMA4  = (info[2] & ((int)1 << 16)) != 0;
    HW_XOP   = (info[2] & ((int)1 << 11)) != 0;
  }
}


int main(int argc, char* argv[])
{
  get_cpu_flags();

  return 0;
}
