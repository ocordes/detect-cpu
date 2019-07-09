#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* this code is heavily inspired by
  https://github.com/Mysticial/FeatureDetector
*/

/* technical information about the cpuid and feature
  extraction one can get from:

  https://dox.ipxe.org/cpuid_8h_source.html
  https://hjlebbink.github.io/x86doc/html/CPUID.html
*/

/* tranlating the CPU features into gcc ARCH definitions
  based on this document:

  https://gcc.gnu.org/onlinedocs/gcc/x86-Options.html

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


char cpu_id_str[13];


typedef struct {
  int id;
  char *vendor;
} _vendor_strings;


#define CPU_UNKNOWN   0
#define CPU_Intel     1
#define CPU_AMD       2
#define CPU_Centauer  3
#define CPU_Cyrix     4
#define CPU_Hygon     5
#define CPU_Transmeta 6
#define CPU_NSC       7
#define CPU_NexGen    8
#define CPU_Rise      9
#define CPU_SiS       10
#define CPU_UMC       11
#define CPU_VIA       12
#define CPU_Vortex    13


/* CPU feature flags */

/* EAX=1: Processor Info and Feature Bits */
int HW_FPU = 0;
int HW_VME = 0;
int HW_DE = 0;
int HW_PSE = 0;
int HW_TSC = 0;
int HW_MSR = 0;
int HW_PAE = 0;
int HW_MCE = 0;
int HW_CX8 = 0;
int HW_APIC = 0;

int HW_SEP = 0;
int HW_MTRR = 0;
int HW_PGE = 0;
int HW_MCA = 0;
int HW_CMOV = 0;
int HW_PAT = 0;
int HW_PSE36 = 0;
int HW_PSN = 0;
int HW_CLFLUSH = 0;

int HW_DS = 0;
int HW_ACPI = 0;
int HW_MMX = 0;
int HW_FXSR = 0;
int HW_SSE = 0;
int HW_SSE2 = 0;
int HW_SS = 0;
int HW_HTT = 0;
int HW_TM = 0;
int HW_IA64 = 0;
int HW_PBE = 0;

int HW_SSE3 = 0;
int HW_PCLMUL = 0;
int HW_DTES64 = 0;
int HW_MONITOR = 0;
int HW_DS_CPL = 0;
int HW_VMX = 0;
int HW_SMX = 0;
int HW_EST = 0;
int HW_TM2 = 0;
int HW_SSSE3 = 0;
int HW_CNXT_ID = 0;
int HW_SDBG = 0;
int HW_FMA = 0;
int HW_CX16 = 0;
int HW_XTPR = 0;
int HW_PDCM = 0;
int HW_PCID = 0;
int HW_DCA = 0;
int HW_SSE41 = 0;
int HW_SSE42 = 0;
int HW_X2APIC = 0;
int HW_MOVBE = 0;
int HW_POPCNT = 0;
int HW_TSC_DEADLINE = 0;
int HW_AES = 0;
int HW_XSAVE = 0;
int HW_OSXSAVE = 0;
int HW_AVX = 0;
int HW_F16C = 0;
int HW_RDRND = 0;
int HW_HYPERVISOR = 0;



/*  Misc. */
int HW_x64 = 0;
int HW_ABM = 0;      /* Advanced Bit Manipulation */
int HW_BMI = 0;
int HW_BMI2 = 0;
int HW_RDSEED = 0;
int HW_ADX = 0;
int HW_CLFLUSHOPT = 0;
int HW_PREFETCHWT1 = 0;
int HW_PREFETCHW = 0;

/*  SIMD: 128-bit */
int HW_SSE4a = 0;
int HW_SHA = 0;

/*  SIMD: 256-bit */
int HW_FSGSBASE = 0;
int HW_XOP = 0;
int HW_FMA4 = 0;
int HW_AVX2 = 0;

/*  SIMD: 512-bit */
int HW_AVX512F = 0;    /*  AVX512 Foundation */
int HW_AVX512CD = 0;   /*  AVX512 Conflict Detection */
int HW_AVX512PF = 0;   /*  AVX512 Prefetch */
int HW_AVX512ER = 0;   /*  AVX512 Exponential + Reciprocal */
int HW_AVX512VL = 0;   /*  AVX512 Vector Length Extensions */
int HW_AVX512BW = 0;   /*  AVX512 Byte + Word */
int HW_AVX512DQ = 0;   /*  AVX512 Doubleword + Quadword */
int HW_AVX512IFMA = 0; /*  AVX512 Integer 52-bit Fused Multiply-Add */
int HW_AVX512VBMI = 0; /*  AVX512 Vector Byte Manipulation Instructions */


/* Processor Extended State Enumeration Main Leaf (EAX = 0DH, ECX = 0) */
int HW_XSAVEC = 0;
int HW_XSAVES = 0;
int HW_XSAVEOPT = 0;
int HW_XGETBV = 0;

/* Intel CPUs */

typedef struct {
  int id;
  char *arch;
} _cpu_arch;


#define cpu_x86_64            0
#define intel_core2           100
#define intel_nehalem         101
#define intel_westmere        102
#define intel_sandybridge     103
#define intel_ivybridge       104
#define intel_haswell         105
#define intel_broadwell       106
#define intel_skylake         107
#define intel_bonnell         108
#define intel_silvermont      109
#define intel_goldmont        110
#define intel_goldmont_plus   111
#define intel_tremont         112
#define intel_knl             113
#define intel_knm             114
#define intel_skylake_avx512  115
#define intel_cannonlake      116
#define intel_icelake_client  117
#define intel_icelake_server  118
#define intel_cascadelake     119

#define amd_athlon64          200


_cpu_arch cpu_archs[] = {{intel_core2, "core2"},
                         {intel_nehalem, "nehalem"},
                         {intel_westmere, "westmere"},
                         {intel_sandybridge, "sandybridge"},
                         {intel_ivybridge, "ivybridge"},
                         {intel_haswell, "haswell"},
                         {intel_broadwell, "broadwell"},
                         {intel_skylake, "skylake"},
                         {intel_bonnell, "bonnell"},
                         {intel_silvermont, "silvermont"},
                         {intel_goldmont, "goldmont"},
                         {intel_goldmont_plus, "goldmont-plus"},
                         {intel_tremont, "tremont"},
                         {intel_knl, "knl"},
                         {intel_knm, "knm"},
                         {intel_skylake_avx512, "skylake-avx512"},
                         {intel_cannonlake, "cannonlake"},
                         {intel_icelake_client, "icelake-client"},
                         {intel_icelake_server, "icelake-server"},
                         {intel_cascadelake, "cascadelake"},
                         {amd_athlon64, "athlon64"},
                         {cpu_x86_64, NULL}
                       };


int cpu_type;

_vendor_strings vendor_string[14] = { { CPU_Intel, "GenuineIntel"},
                                     { CPU_AMD, "AuthenticAMD"},
                                     { CPU_Centauer, "CentaurHauls"},
                                     { CPU_Cyrix, "CyrixInstead"},
                                     { CPU_Hygon, "HygonGenuine"},
                                     { CPU_Transmeta, "TransmetaCPU"},
                                     { CPU_Transmeta, "TransmetaCPU"},
                                     { CPU_NSC, "Geode by NSC"},
                                     { CPU_NexGen, ""},
                                     { CPU_Rise, ""},
                                     { CPU_UMC, ""},
                                     { CPU_VIA, ""},
                                     { CPU_Vortex, ""},
                                     { CPU_UNKNOWN, NULL }
                                   };


/* cpu_manufacturer_id

the result is stored in that order, a, c, b corresponding to
EBX, EDX, ECX!
*/

void cpu_manufacturer_id(int a, int c, int b)
{
  cpu_id_str[12] = '\0';
  cpu_id_str[0] = (char)(a >> 0) & 0xff;
  cpu_id_str[1] = (char)(a >> 8) & 0xff;
  cpu_id_str[2] = (char)(a >> 16) & 0xff;
  cpu_id_str[3] = (char)(a >> 24) & 0xff;
  cpu_id_str[4] = (char)(b >> 0) & 0xff;
  cpu_id_str[5] = (char)(b >> 8) & 0xff;
  cpu_id_str[6] = (char)(b >> 16) & 0xff;
  cpu_id_str[7] = (char)(b >> 24) & 0xff;
  cpu_id_str[8] = (char)(c >> 0) & 0xff;
  cpu_id_str[9] = (char)(c >> 8) & 0xff;
  cpu_id_str[10] = (char)(c >> 16) & 0xff;
  cpu_id_str[11] = (char)(c >> 24) & 0xff;
}


void set_cpu_type(void)
{
  int i;

  i = 0;

  while (vendor_string[i].id != CPU_UNKNOWN)
  {
    if (strcmp(cpu_id_str, vendor_string[i].vendor) == 0)
      cpu_type = vendor_string[i].id;
      return;

    ++i;
  }

  cpu_type = CPU_UNKNOWN;
}


void get_cpu_flags(void)
{
  int info[4];
  int nIds;
  unsigned nExIds;

  cpuid(info, 0);
  nIds = info[0];   /* the maximum leaf for question with cpuid */

  printf("%x\n", nIds);

  /* read the vendor string */
  cpu_manufacturer_id(info[1], info[2], info[3]);
  set_cpu_type();

  cpuid(info, 0x80000000);
  nExIds = info[0];

  /* Detect Features */
  if (nIds >= 0x00000001)
  {
    cpuid(info,0x00000001);
    HW_FPU     = (info[3] & ((int)1 << 0)) != 0;
	  HW_VME     = (info[3] & ((int)1 << 1)) != 0;
	  HW_DE      = (info[3] & ((int)1 << 2)) != 0;
	  HW_PSE     = (info[3] & ((int)1 << 3)) != 0;
	  HW_TSC     = (info[3] & ((int)1 << 4)) != 0;
	  HW_MSR     = (info[3] & ((int)1 << 5)) != 0;
	  HW_PAE     = (info[3] & ((int)1 << 6)) != 0;
	  HW_MCE     = (info[3] & ((int)1 << 7)) != 0;
	  HW_CX8     = (info[3] & ((int)1 << 8)) != 0;
	  HW_APIC    = (info[3] & ((int)1 << 9)) != 0;

	  HW_SEP     = (info[3] & ((int)1 << 11)) != 0;
    HW_MTRR    = (info[3] & ((int)1 << 12)) != 0;
    HW_PGE     = (info[3] & ((int)1 << 13)) != 0;
    HW_MCA     = (info[3] & ((int)1 << 14)) != 0;
    HW_CMOV    = (info[3] & ((int)1 << 15)) != 0;
    HW_PAT     = (info[3] & ((int)1 << 16)) != 0;
    HW_PSE36   = (info[3] & ((int)1 << 17)) != 0;
    HW_PSN     = (info[3] & ((int)1 << 18)) != 0;
    HW_CLFLUSH = (info[3] & ((int)1 << 19)) != 0;

	  HW_DS      = (info[3] & ((int)1 << 21)) != 0;
    HW_ACPI    = (info[3] & ((int)1 << 22)) != 0;
    HW_MMX     = (info[3] & ((int)1 << 23)) != 0;
    HW_FXSR    = (info[3] & ((int)1 << 24)) != 0;
    HW_SSE     = (info[3] & ((int)1 << 25)) != 0;
    HW_SSE2    = (info[3] & ((int)1 << 26)) != 0;
    HW_SS      = (info[3] & ((int)1 << 27)) != 0;
    HW_HTT     = (info[3] & ((int)1 << 28)) != 0;
    HW_TM      = (info[3] & ((int)1 << 29)) != 0;
    HW_IA64    = (info[3] & ((int)1 << 30)) != 0;
    HW_PBE     = (info[3] & ((int)1 << 31)) != 0;


    HW_SSE3    = (info[2] & ((int)1 <<  0)) != 0;
    HW_PCLMUL  = (info[2] & ((int)1 <<  1)) != 0;
    HW_DTES64  = (info[2] & ((int)1 <<  2)) != 0;
    HW_MONITOR = (info[2] & ((int)1 <<  3)) != 0;
    HW_DS_CPL  = (info[2] & ((int)1 <<  4)) != 0;
    HW_VMX     = (info[2] & ((int)1 <<  5)) != 0;
    HW_SMX     = (info[2] & ((int)1 <<  6)) != 0;
    HW_EST     = (info[2] & ((int)1 <<  7)) != 0;
    HW_TM2     = (info[2] & ((int)1 <<  8)) != 0;
    HW_SSSE3   = (info[2] & ((int)1 <<  9)) != 0;
    HW_CNXT_ID = (info[2] & ((int)1 << 10)) != 0;
    HW_SDBG    = (info[2] & ((int)1 << 11)) != 0;
    HW_FMA     = (info[2] & ((int)1 << 12)) != 0;
    HW_CX16    = (info[2] & ((int)1 << 13)) != 0;
    HW_XTPR    = (info[2] & ((int)1 << 14)) != 0;
    HW_PDCM    = (info[2] & ((int)1 << 15)) != 0;
    HW_PCID    = (info[2] & ((int)1 << 17)) != 0;
    HW_DCA     = (info[2] & ((int)1 << 18)) != 0;
    HW_SSE41   = (info[2] & ((int)1 << 19)) != 0;
    HW_SSE42   = (info[2] & ((int)1 << 20)) != 0;
    HW_X2APIC  = (info[2] & ((int)1 << 21)) != 0;
    HW_MOVBE   = (info[2] & ((int)1 << 22)) != 0;
    HW_POPCNT  = (info[2] & ((int)1 << 23)) != 0;
    HW_TSC_DEADLINE = (info[2] & ((int)1 << 24)) != 0;
    HW_AES     = (info[2] & ((int)1 << 25)) != 0;
    HW_XSAVE   = (info[2] & ((int)1 << 26)) != 0;
    HW_OSXSAVE = (info[2] & ((int)1 << 27)) != 0;
    HW_AVX     = (info[2] & ((int)1 << 28)) != 0;
    HW_F16C    = (info[2] & ((int)1 << 29)) != 0;
    HW_RDRND   = (info[2] & ((int)1 << 30)) != 0;
    HW_HYPERVISOR = (info[2] & ((int)1 << 31)) != 0;
  }

  if (nIds >= 0x00000007)
  {
    cpuid(info,0x00000007);
    HW_FSGSBASE    = (info[1] & ((int)1 <<  0)) != 0;
    HW_BMI         = (info[1] & ((int)1 <<  3)) != 0;
    HW_AVX2        = (info[1] & ((int)1 <<  5)) != 0;
    HW_BMI2        = (info[1] & ((int)1 <<  8)) != 0;
    HW_RDSEED      = (info[1] & ((int)1 << 18)) != 0;
    HW_ADX         = (info[1] & ((int)1 << 19)) != 0;
    HW_CLFLUSHOPT  = (info[1] & ((int)1 << 23)) != 0;
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

  if (nIds >= 0x0000000d)
  {
    cpuid(info,0x0000000d);

    HW_XSAVEOPT = (info[0] & ((int)1 << 0)) != 0;
    HW_XSAVEC   = (info[0] & ((int)1 << 1)) != 0;
    HW_XGETBV   = ((info[0] & ((int)1 << 2)) != 0) && (info[2] == 1);
    HW_XSAVES   = (info[0] & ((int)1 << 3)) != 0;
  }

  if (nExIds >= 0x80000001)
  {
    cpuid(info,0x80000001);
    HW_x64       = (info[3] & ((int)1 << 29)) != 0;
    HW_ABM       = (info[2] & ((int)1 <<  5)) != 0;
    HW_SSE4a     = (info[2] & ((int)1 <<  6)) != 0;
    HW_PREFETCHW = (info[2] & ((int)1 <<  8)) != 0;
    HW_FMA4      = (info[2] & ((int)1 << 16)) != 0;
    HW_XOP       = (info[2] & ((int)1 << 11)) != 0;
  }
}


char *get_arch_name(int arch)
{
  int i;

  i = 0;

  while (cpu_archs[i].id != cpu_x86_64)
  {
    if (cpu_archs[i].id == arch)
      return strdup(cpu_archs[i].arch);
    ++i;
  }

  return strdup("x86-64");
}



int get_gcc_arch_type_intel(void)
{
  if (HW_MMX && HW_SSE && HW_SSE2 && HW_SSE3 && HW_SSSE3)
  {
    if (HW_SSE41 && HW_SSE42 && HW_POPCNT)
    {
      if (HW_AES && HW_PCLMUL)
      {
        if (HW_AVX)
        {
          if (HW_FSGSBASE && HW_RDRND && HW_F16C)
          {
            if (HW_MOVBE && HW_AVX2 && HW_FMA && HW_BMI && HW_BMI2)
            {
              if (HW_RDSEED && HW_ADX && HW_PREFETCHW && HW_CLFLUSHOPT
                  && HW_XSAVEC && HW_XSAVES)
                return intel_skylake;
              if (HW_RDSEED && HW_ADX && HW_PREFETCHW)
                return intel_broadwell;

              return intel_haswell;
            }
            return intel_ivybridge;
          }
          return intel_sandybridge;
        }
        return intel_westmere;
      }
      return intel_nehalem;
    }

    if (HW_MOVBE)
      return intel_bonnell;

    return intel_core2;
  }
  else
    return cpu_x86_64;
}


int get_gcc_arch_type(void)
{
  switch(cpu_type)
  {
    case CPU_Intel:
        return get_gcc_arch_type_intel();
        break;
    case CPU_AMD:
        return cpu_x86_64;
        break;
    default:
        return cpu_x86_64;
        break;
  }
}


void report_cpu_data(void)
{
  printf("vendor: %s\n", cpu_id_str);
  printf("cpu_type=%i\n", cpu_type);
}


void cpu_arch_type(void)
{
  char *s;
  int   arch;

  arch = get_gcc_arch_type();

  s = get_arch_name(arch);

  printf("%s\n", s);
}


void all_cpu_flags(void)
{
  #define mc 5000
  char *s;

  s = (char*) malloc(mc);
  s[0] = '\0';

  if (HW_FPU) strncat(s, "fpu ", mc);
  if (HW_VME) strncat(s, "vme ", mc);
  if (HW_DE) strncat(s, "de ", mc);
  if (HW_PSE) strncat(s, "pse ", mc);
  if (HW_TSC) strncat(s, "tsc ", mc);
  if (HW_MSR) strncat(s, "msr ", mc);
  if (HW_PAE) strncat(s, "pae ", mc);
  if (HW_MCE) strncat(s, "mce ", mc);
  if (HW_CX8) strncat(s, "cx8 ", mc);
  if (HW_APIC) strncat(s, "apic ", mc);

  if (HW_SEP) strncat(s, "sep ", mc);
  if (HW_MTRR) strncat(s, "mtrr ", mc);
  if (HW_PGE) strncat(s, "pge ", mc);
  if (HW_MCA) strncat(s, "mca ", mc);
  if (HW_CMOV) strncat(s, "cmov ", mc);
  if (HW_PAT) strncat(s, "pat ", mc);
  if (HW_PSE36) strncat(s, "pse36 ", mc);
  if (HW_PSN) strncat(s, "psn ", mc);
  if (HW_CLFLUSH) strncat(s, "clflush ", mc);

  if (HW_DS) strncat(s, "ds ", mc);
  if (HW_ACPI) strncat(s, "acpi ", mc);
  if (HW_MMX) strncat(s, "mmx ", mc);
  if (HW_FXSR) strncat(s, "fxsr ", mc);
  if (HW_SSE) strncat(s, "sse ", mc);
  if (HW_SSE2) strncat(s, "sse2 ", mc);
  if (HW_SS) strncat(s, "ss ", mc);
  if (HW_HTT) strncat(s, "htt ", mc);
  if (HW_TM) strncat(s, "tm ", mc);
  if (HW_IA64) strncat(s, "ia64 ", mc);
  if (HW_PBE) strncat(s, "pbe ", mc);

  printf("CPU flags: %s\n", s);
  free(s);
}


int main(int argc, char* argv[])
{
  get_cpu_flags();

  report_cpu_data();

  all_cpu_flags();

  cpu_arch_type();

  return 0;
}
