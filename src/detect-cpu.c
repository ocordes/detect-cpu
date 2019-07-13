#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* detect-cpu.c

written by: Oliver Cordes 2019-07-01
changed by: Oliver Cordes 2019-07-01

*/


#define __author__ "Oliver Cordes"
#define __version__ "0.1.0"
#define __copyright__ "(C) Copyright 2019"


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

/* define some cpuid calls for any OS */

#ifdef _WIN32

/*  Windows */
#define cpuid(info, x)    __cpuidex(info, x, 0)

#else

/*  GCC Intrinsics */
#include <cpuid.h>
void cpuid(int info[4], int InfoType){
    __cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
}

void cpuidcx(int info[4], int InfoType, int cx) {
    __cpuid_count(InfoType, cx, info[0], info[1], info[2], info[3]);
}

#endif


char cpu_id_str[13];
char cpu_brand[49];
int cpuid_level;
int cpuid_ext_level;


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
#define HW_MWAIT HW_MONITOR
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


/* EAX=80000001h: Extended Processor Info and Feature Bits */

int HW_SYSCALL = 0;
int HW_MP = 0;
int HW_NX = 0;
int HW_MMEXT = 0;
int HW_FXSR_OPT = 0;
int HW_PDPE1GB = 0;
int HW_RDTSCP  = 0;
int HW_LM = 0;
int HW_3DNOWEXT = 0;
int HW_3DNOW = 0;


int HW_LAHF_LM = 0;
int HW_CMP_LEGACY = 0;
int HW_SVM = 0;
int HW_EXTAPIC = 0;
int HW_CR8_LEGACY = 0;
int HW_ABM = 0;
int HW_SSE4A = 0;
int HW_MISALIGNSSE = 0;
int HW_3DNOWPREFETCH = 0;
int HW_OSVW = 0;
int HW_IBS = 0;
int HW_XOP = 0;
int HW_SKINIT = 0;
int HW_WDT = 0;
int HW_LWP = 0;
int HW_FMA4 = 0;
int HW_TCE = 0;
int HW_NODEID_MSR = 0;
int HW_TBM = 0;
int HW_TOPOEXT = 0;
int HW_PERFCTR_CORE = 0;
int HW_PERFCTR_NB = 0;
int HW_DBX = 0;
int HW_PERFTSC = 0;
int HW_PCX_L2I = 0;
int HW_MWAITX = 0;
#define HW_MWAITT HW_WAITX


/* extended feature flags EAX=7 */

int HW_FSGSBASE = 0;
int HW_SGX = 0;
int HW_BMI = 0;
int HW_HLE = 0;
int HW_AVX2 = 0;
int HW_SMEP = 0;
int HW_BMI2 = 0;
int HW_ERMS = 0;
int HW_INVPCID = 0;
int HW_RTM = 0;
int HW_PQM = 0;
int HW_MPX = 0;
int HW_PQE = 0;
int HW_AVX512F = 0;
int HW_AVX512DQ = 0;
int HW_RDSEED = 0;
int HW_ADX = 0;
#define HW_ADCX HW_ADX
int HW_SMAP = 0;
int HW_AVX512IFMA = 0;
int HW_PCOMMIT = 0;
int HW_CLFLUSHOPT = 0;
int HW_CLWB = 0;
int HW_INTEL_PT = 0;
int HW_AVX512PF = 0;
int HW_AVX512ER = 0;
int HW_AVX512CD = 0;
int HW_SHA = 0;
int HW_AVX512BW = 0;
int HW_AVX512VL = 0;


/*  Misc. */
int HW_x64 = 0;

int HW_PREFETCHWT1 = 0;
int HW_PREFETCHW = 0;


/* Processor Extended State Enumeration Main Leaf (EAX = 0DH, ECX = 0) */
int HW_XSAVEC = 0;
int HW_XSAVES = 0;
int HW_XSAVEOPT = 0;
int HW_XGETBV = 0;


/* AMD-defined CPU features, CPUID level 0x80000008 (EBX) */
int HW_CLZERO = 0;




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
#define amd_athlon64_sse3     201
#define amd_amdfam10          202
#define amd_bdver1            203
#define amd_bdver2            204
#define amd_bdver3            205
#define amd_bdver4            206
#define amd_znver1            207
#define amd_znver2            208
#define amd_btver1            209
#define amd_btver2            210


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
                         {amd_athlon64_sse3, "athlon64_sse3"},
                         {amd_amdfam10, "amdfam10"},
                         {amd_bdver1, "bdver1"},
                         {amd_bdver2, "bdver2"},
                         {amd_bdver3, "bdver3"},
                         {amd_bdver4, "bdver4"},
                         {amd_znver1, "znver1"},
                         {amd_znver2, "znver2"},
                         {amd_btver1, "btver1"},
                         {amd_btver2, "btver1"},
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
    {
      cpu_type = vendor_string[i].id;
      return;
    }
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
  cpuid_level = nIds;

  /* read the vendor string */
  cpu_manufacturer_id(info[1], info[2], info[3]);
  set_cpu_type();

  cpuid(info, 0x80000000);
  nExIds = info[0];  /* the maximum leaf for extended couid questions*/
  cpuid_ext_level = nExIds;

  /* Detect Features */
  if (nIds >= 0x00000001)
  {
    cpuid(info, 0x00000001);
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
    cpuid(info, 0x00000007);
    HW_FSGSBASE    = (info[1] & ((int)1 <<  0)) != 0;
    HW_SGX         = (info[1] & ((int)1 <<  2)) != 0;
    HW_BMI         = (info[1] & ((int)1 <<  3)) != 0;
    HW_HLE         = (info[1] & ((int)1 <<  4)) != 0;
    HW_AVX2        = (info[1] & ((int)1 <<  5)) != 0;
    HW_SMEP        = (info[1] & ((int)1 <<  7)) != 0;
    HW_BMI2        = (info[1] & ((int)1 <<  8)) != 0;
    HW_ERMS        = (info[1] & ((int)1 <<  9)) != 0;
    HW_INVPCID     = (info[1] & ((int)1 << 10)) != 0;
    HW_RTM         = (info[1] & ((int)1 << 11)) != 0;
    HW_PQM         = (info[1] & ((int)1 << 12)) != 0;
    HW_MPX         = (info[1] & ((int)1 << 14)) != 0;
    HW_PQE         = (info[1] & ((int)1 << 15)) != 0;
    HW_AVX512F     = (info[1] & ((int)1 << 16)) != 0;
    HW_AVX512DQ    = (info[1] & ((int)1 << 17)) != 0;
    HW_RDSEED      = (info[1] & ((int)1 << 18)) != 0;
    HW_ADX         = (info[1] & ((int)1 << 19)) != 0;
    HW_SMAP        = (info[1] & ((int)1 << 20)) != 0;
    HW_AVX512IFMA  = (info[1] & ((int)1 << 21)) != 0;
    HW_PCOMMIT     = (info[1] & ((int)1 << 22)) != 0;
    HW_CLFLUSHOPT  = (info[1] & ((int)1 << 23)) != 0;
    HW_CLWB        = (info[1] & ((int)1 << 24)) != 0;
    HW_INTEL_PT    = (info[1] & ((int)1 << 25)) != 0;
    HW_AVX512PF    = (info[1] & ((int)1 << 26)) != 0;
    HW_AVX512ER    = (info[1] & ((int)1 << 27)) != 0;
    HW_AVX512CD    = (info[1] & ((int)1 << 28)) != 0;
    HW_SHA         = (info[1] & ((int)1 << 29)) != 0;
    HW_AVX512BW    = (info[1] & ((int)1 << 30)) != 0;
    HW_AVX512VL    = (info[1] & ((int)1 << 31)) != 0;

    HW_PREFETCHWT1 = (info[2] & ((int)1 <<  0)) != 0;
  }

  if (nIds >= 0x0000000d)
  {
    /* EAX=0dh ECX=1 */
    cpuidcx(info, 0x0000000d, 1);

    HW_XSAVEOPT = (info[0] & ((int)1 << 0)) != 0;
    HW_XSAVEC   = (info[0] & ((int)1 << 1)) != 0;
    HW_XGETBV   = ((info[0] & ((int)1 << 2)) != 0) && (info[2] == 1);
    HW_XSAVES   = (info[0] & ((int)1 << 3)) != 0;

  }

  if (nExIds >= 0x80000001)
  {
    cpuid(info, 0x80000001);

    /*
    HW_FPU       = (info[3] & ((int)1 <<  0)) != 0;
    HW_VME       = (info[3] & ((int)1 <<  1)) != 0;
    HW_DE        = (info[3] & ((int)1 <<  2)) != 0;
    HW_PSE       = (info[3] & ((int)1 <<  3)) != 0;
    HW_TSC       = (info[3] & ((int)1 <<  4)) != 0;
    HW_MSR       = (info[3] & ((int)1 <<  5)) != 0;
    HW_PAE       = (info[3] & ((int)1 <<  6)) != 0;
    HW_MCE       = (info[3] & ((int)1 <<  7)) != 0;
    HW_CX8       = (info[3] & ((int)1 <<  8)) != 0;
    HW_APIC      = (info[3] & ((int)1 <<  9)) != 0; */
    HW_SYSCALL   = (info[3] & ((int)1 << 11)) != 0;
    /*
    HW_MTRR      = (info[3] & ((int)1 << 12)) != 0;
    HW_PGE	 = (info[3] & ((int)1 << 13)) != 0;
    HW_MCA 	 = (info[3] & ((int)1 << 14)) != 0;
    HW_CMOV      = (info[3] & ((int)1 << 15)) != 0;
    HW_PAT       = (info[3] & ((int)1 << 16)) != 0;
    HW_PSE36     = (info[3] & ((int)1 << 17)) != 0; */
    HW_MP        = (info[3] & ((int)1 << 19)) != 0;
    HW_NX        = (info[3] & ((int)1 << 20)) != 0;
    HW_MMEXT     = (info[3] & ((int)1 << 22)) != 0;
    /*
    HW_MMX       = (info[3] & ((int)1 << 23)) != 0;
    HW_FXSR      = (info[3] & ((int)1 << 24)) != 0; */
    HW_FXSR_OPT  = (info[3] & ((int)1 << 25)) != 0;
    HW_PDPE1GB   = (info[3] & ((int)1 << 26)) != 0;
    HW_RDTSCP    = (info[3] & ((int)1 << 27)) != 0;
    HW_LM        = (info[3] & ((int)1 << 29)) != 0;
    HW_3DNOWEXT  = (info[3] & ((int)1 << 30)) != 0;
    HW_3DNOW     = (info[3] & ((int)1 << 31)) != 0;

    HW_LAHF_LM   = (info[2] & ((int)1 <<  0)) != 0;
    HW_CMP_LEGACY = (info[2] & ((int)1 <<  1)) != 0;
    HW_SVM       = (info[2] & ((int)1 <<  2)) != 0;
    HW_EXTAPIC   = (info[2] & ((int)1 <<  3)) != 0;
    HW_CR8_LEGACY = (info[2] & ((int)1 <<  4)) != 0;
    HW_ABM       = (info[2] & ((int)1 <<  5)) != 0;
    HW_SSE4A     = (info[2] & ((int)1 <<  6)) != 0;
    HW_MISALIGNSSE = (info[2] & ((int)1 <<  7)) != 0;
    HW_3DNOWPREFETCH = (info[2] & ((int)1 <<  8)) != 0;
    HW_OSVW      = (info[2] & ((int)1 <<  9)) != 0;
    HW_IBS       = (info[2] & ((int)1 << 10)) != 0;
    HW_XOP       = (info[2] & ((int)1 << 11)) != 0;
    HW_SKINIT    = (info[2] & ((int)1 << 12)) != 0;
    HW_WDT       = (info[2] & ((int)1 << 13)) != 0;
    HW_LWP       = (info[2] & ((int)1 << 15)) != 0;
    HW_FMA4      = (info[2] & ((int)1 << 16)) != 0;
    HW_TCE       = (info[2] & ((int)1 << 17)) != 0;
    HW_NODEID_MSR = (info[2] & ((int)1 << 19)) != 0;
    HW_TBM       = (info[2] & ((int)1 << 21)) != 0;
    HW_TOPOEXT   = (info[2] & ((int)1 << 22)) != 0;
    HW_PERFCTR_CORE = (info[2] & ((int)1 << 23)) != 0;
    HW_PERFCTR_NB = (info[2] & ((int)1 << 24)) != 0;
    HW_DBX       = (info[2] & ((int)1 << 26)) != 0;
    HW_PERFTSC   = (info[2] & ((int)1 << 27)) != 0;
    HW_PCX_L2I   = (info[2] & ((int)1 << 28)) != 0;
    HW_MWAITX    = (info[2] & ((int)1 << 29)) != 0;


    /* copy for some backup usage */
    HW_x64       = HW_LM;
    HW_PREFETCHW = HW_3DNOWPREFETCH;
  }

  if (nExIds >= 0x80000004)
  {
    cpu_brand[48] = '\0';
    cpuid(info, 0x80000002);
    cpu_brand[0] = (char)(info[0] >> 0) & 0xff;
    cpu_brand[1] = (char)(info[0] >> 8) & 0xff;
    cpu_brand[2] = (char)(info[0] >> 16) & 0xff;
    cpu_brand[3] = (char)(info[0] >> 24) & 0xff;
    cpu_brand[4] = (char)(info[1] >> 0) & 0xff;
    cpu_brand[5] = (char)(info[1] >> 8) & 0xff;
    cpu_brand[6] = (char)(info[1] >> 16) & 0xff;
    cpu_brand[7] = (char)(info[1] >> 24) & 0xff;
    cpu_brand[8] = (char)(info[2] >> 0) & 0xff;
    cpu_brand[9] = (char)(info[2] >> 8) & 0xff;
    cpu_brand[10] = (char)(info[2] >> 16) & 0xff;
    cpu_brand[11] = (char)(info[2] >> 24) & 0xff;
    cpu_brand[12] = (char)(info[3] >> 0) & 0xff;
    cpu_brand[13] = (char)(info[3] >> 8) & 0xff;
    cpu_brand[14] = (char)(info[3] >> 16) & 0xff;
    cpu_brand[15] = (char)(info[3] >> 24) & 0xff;
    cpuid(info, 0x80000003);
    cpu_brand[16] = (char)(info[0] >> 0) & 0xff;
    cpu_brand[17] = (char)(info[0] >> 8) & 0xff;
    cpu_brand[18] = (char)(info[0] >> 16) & 0xff;
    cpu_brand[19] = (char)(info[0] >> 24) & 0xff;
    cpu_brand[20] = (char)(info[1] >> 0) & 0xff;
    cpu_brand[21] = (char)(info[1] >> 8) & 0xff;
    cpu_brand[22] = (char)(info[1] >> 16) & 0xff;
    cpu_brand[23] = (char)(info[1] >> 24) & 0xff;
    cpu_brand[24] = (char)(info[2] >> 0) & 0xff;
    cpu_brand[25] = (char)(info[2] >> 8) & 0xff;
    cpu_brand[26] = (char)(info[2] >> 16) & 0xff;
    cpu_brand[27] = (char)(info[2] >> 24) & 0xff;
    cpu_brand[28] = (char)(info[3] >> 0) & 0xff;
    cpu_brand[29] = (char)(info[3] >> 8) & 0xff;
    cpu_brand[30] = (char)(info[3] >> 16) & 0xff;
    cpu_brand[31] = (char)(info[3] >> 24) & 0xff;
    cpuid(info, 0x80000004);
    cpu_brand[32] = (char)(info[0] >> 0) & 0xff;
    cpu_brand[33] = (char)(info[0] >> 8) & 0xff;
    cpu_brand[34] = (char)(info[0] >> 16) & 0xff;
    cpu_brand[35] = (char)(info[0] >> 24) & 0xff;
    cpu_brand[36] = (char)(info[1] >> 0) & 0xff;
    cpu_brand[37] = (char)(info[1] >> 8) & 0xff;
    cpu_brand[38] = (char)(info[1] >> 16) & 0xff;
    cpu_brand[39] = (char)(info[1] >> 24) & 0xff;
    cpu_brand[40] = (char)(info[2] >> 0) & 0xff;
    cpu_brand[41] = (char)(info[2] >> 8) & 0xff;
    cpu_brand[42] = (char)(info[2] >> 16) & 0xff;
    cpu_brand[43] = (char)(info[2] >> 24) & 0xff;
    cpu_brand[44] = (char)(info[3] >> 0) & 0xff;
    cpu_brand[45] = (char)(info[3] >> 8) & 0xff;
    cpu_brand[46] = (char)(info[3] >> 16) & 0xff;
    cpu_brand[47] = (char)(info[3] >> 24) & 0xff;
  }

  if (nExIds >= 0x80000006)
  {
    cpuid(info, 0x80000006);
  }

  if (nExIds >= 0x80000008)
  {
    cpuid(info, 0x80000008);
    HW_CLZERO    = (info[1] & ((int)1 <<  0)) != 0;
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


int get_gcc_arch_type_amd(void)
{
  if (HW_BMI && HW_BMI2 && HW_F16C && HW_FMA && HW_FSGSBASE && HW_AVX
      && HW_AVX2 && HW_ADCX && HW_RDSEED && HW_MWAITX && HW_SHA && HW_CLZERO
      && HW_AES && HW_PCLMUL && HW_CX16 && HW_MOVBE && HW_MMX && HW_SSE
      && HW_SSE2 && HW_SSE3 && HW_SSE4A && HW_SSSE3 && HW_SSE41 && HW_SSE42
      && HW_ABM && HW_XSAVEC && HW_XSAVES && HW_CLFLUSHOPT && HW_POPCNT)
  {
    if (HW_CLWB)
      return amd_znver2;
    else
      return amd_znver1;
  }


  if (HW_FMA4 && HW_AVX && HW_XOP && HW_LWP && HW_AES && HW_PCLMUL && HW_CX16
     && HW_MMX && HW_SSE && HW_SSE2 && HW_SSE3 && HW_SSE4A && HW_SSSE3
     && HW_SSE41 && HW_SSE42 && HW_ABM)
  {
    if (HW_BMI && HW_TBM && HW_F16C && HW_FMA)
    {
      if (HW_FSGSBASE)
      {
        if (HW_BMI2 && HW_AVX2 && HW_MOVBE)
          return amd_bdver4;
        else
          return amd_bdver3;
      }
      else
        return amd_bdver2;
    }
    else
      return amd_bdver1;
  }

  if (HW_MMX && HW_SSE && HW_SSE2 && HW_3DNOW && HW_3DNOWEXT)
  {
    if (HW_SSE3)
      if (HW_SSE4A && HW_ABM)
        return amd_amdfam10;
      else
        return amd_athlon64_sse3;
    else
      return amd_athlon64;
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
        return get_gcc_arch_type_amd();
        break;
    default:
        return cpu_x86_64;
        break;
  }
}


void report_cpu_data(void)
{
  printf("Vendor         : %s\n", cpu_id_str);
  printf("Brand          : %s\n", cpu_brand);
  printf("cpuid level    : 0x%x\n", cpuid_level);
  printf("cpuid ext level: 0x%x\n", cpuid_ext_level);
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
  if (HW_HTT) strncat(s, "ht ", mc);
  if (HW_TM) strncat(s, "tm ", mc);
  if (HW_IA64) strncat(s, "ia64 ", mc);
  if (HW_PBE) strncat(s, "pbe ", mc);

  if (HW_SYSCALL) strncat(s, "syscall ", mc);
  if (HW_MP) strncat(s, "mp ", mc);
  if (HW_NX) strncat(s, "nx ", mc);
  if (HW_MMEXT) strncat(s, "mmext ", mc);
  if (HW_FXSR_OPT) strncat(s, "fxsr_opt ", mc);
  if (HW_PDPE1GB) strncat(s, "pdpe1gb ", mc);
  if (HW_RDTSCP) strncat(s, "rdtscp ", mc);
  if (HW_LM) strncat(s, "lm ", mc);
  if (HW_3DNOWEXT) strncat(s, "3dnowext ", mc);
  if (HW_3DNOW) strncat(s, "3dnow ", mc);


  if (HW_LAHF_LM) strncat(s, "lahf_lm ", mc);
  if (HW_CMP_LEGACY) strncat(s, "cmp_legacy ", mc);
  if (HW_SVM) strncat(s, "svm ", mc);
  if (HW_EXTAPIC) strncat(s, "extapic ", mc);
  if (HW_CR8_LEGACY) strncat(s, "cr8_legacy ", mc);
  if (HW_ABM) strncat(s, "abm ", mc);
  if (HW_SSE4A) strncat(s, "sse4a ", mc);
  if (HW_MISALIGNSSE) strncat(s, "misalignsse ", mc);
  if (HW_3DNOWPREFETCH) strncat(s, "3dnowprefetch ", mc);
  if (HW_OSVW) strncat(s, "osvw ", mc);
  if (HW_IBS) strncat(s, "ibs ", mc);
  if (HW_XOP) strncat(s, "xop ", mc);
  if (HW_SKINIT) strncat(s, "skinit ", mc);
  if (HW_WDT) strncat(s, "wdt ", mc);
  if (HW_LWP) strncat(s, "lwp ", mc);
  if (HW_FMA4) strncat(s, "fma4 ", mc);
  if (HW_TCE) strncat(s, "tce ", mc);
  if (HW_NODEID_MSR) strncat(s, "nodeid_msr ", mc);
  if (HW_TBM) strncat(s, "tbm ", mc);
  if (HW_TOPOEXT) strncat(s, "topoext ", mc);
  if (HW_PERFCTR_CORE) strncat(s, "perfctr_core ", mc);
  if (HW_PERFCTR_NB) strncat(s, "perfctr_nb ", mc);
  if (HW_DBX) strncat(s, "dbx ", mc);
  if (HW_PERFTSC) strncat(s, "perftsc ", mc);
  if (HW_PCX_L2I) strncat(s, "pcx_l2i ", mc);
  if (HW_MWAITX) strncat(s, "mwaitx ", mc);

  if (HW_SSE3) strncat(s, "sse3 ", mc);
  if (HW_PCLMUL) strncat(s, "pclmul ", mc);
  if (HW_DTES64) strncat(s, "dtes64 ", mc);
  if (HW_MONITOR) strncat(s, "monitor ", mc);
  if (HW_DS_CPL) strncat(s, "ds_cpl ", mc);
  if (HW_VMX) strncat(s, "vmx ", mc);
  if (HW_SMX) strncat(s, "smx ", mc);
  if (HW_EST) strncat(s, "est ", mc);
  if (HW_TM2) strncat(s, "tm2 ", mc);
  if (HW_SSSE3) strncat(s, "ssse3 ", mc);
  if (HW_CNXT_ID) strncat(s, "cnxt_id ", mc);
  if (HW_SDBG) strncat(s, "sdbg ", mc);
  if (HW_FMA) strncat(s, "fma ", mc);
  if (HW_CX16) strncat(s, "cx16 ", mc);
  if (HW_XTPR) strncat(s, "xtpr ", mc);
  if (HW_PDCM) strncat(s, "pdcm ", mc);
  if (HW_PCID) strncat(s, "pcid ", mc);
  if (HW_DCA) strncat(s, "dca ", mc);
  if (HW_SSE41) strncat(s, "sse41 ", mc);
  if (HW_SSE42) strncat(s, "sse42 ", mc);
  if (HW_X2APIC) strncat(s, "x2apic ", mc);
  if (HW_MOVBE) strncat(s, "movbe ", mc);
  if (HW_POPCNT) strncat(s, "popcnt ", mc);
  if (HW_TSC_DEADLINE) strncat(s, "tsc_deadline ", mc);
  if (HW_AES) strncat(s, "aes ", mc);
  if (HW_XSAVE) strncat(s, "xsave ", mc);
  if (HW_OSXSAVE) strncat(s, "osxsave ", mc);
  if (HW_AVX) strncat(s, "avx ", mc);
  if (HW_F16C) strncat(s, "f16c ", mc);
  if (HW_RDRND) strncat(s, "rdrnd ", mc);
  if (HW_HYPERVISOR) strncat(s, "hypervisor ", mc);

  if (HW_FSGSBASE) strncat(s, "fsgsbase ", mc);
  if (HW_SGX) strncat(s, "sgx ", mc);
  if (HW_BMI) strncat(s, "bmi ", mc);
  if (HW_HLE) strncat(s, "hle ", mc);
  if (HW_AVX2) strncat(s, "avx2 ", mc);
  if (HW_SMEP) strncat(s, "smep ", mc);
  if (HW_BMI2) strncat(s, "bmi2 ", mc);
  if (HW_ERMS) strncat(s, "erms", mc);
  if (HW_INVPCID) strncat(s, "invpcid ", mc);
  if (HW_RTM) strncat(s, "rtm ", mc);
  if (HW_PQM) strncat(s, "pqm ", mc);
  if (HW_MPX) strncat(s, "mpx ", mc);
  if (HW_PQE) strncat(s, "pqe ", mc);
  if (HW_AVX512F) strncat(s, "avx512f ", mc);
  if (HW_AVX512DQ) strncat(s, "avx512dq ", mc);
  if (HW_RDSEED) strncat(s, "rdseed ", mc);
  if (HW_ADX) strncat(s, "adx ", mc);
  if (HW_SMAP) strncat(s, "smap ", mc);
  if (HW_AVX512IFMA) strncat(s, "avx512ifma ", mc);
  if (HW_PCOMMIT) strncat(s, "pcommit ", mc);
  if (HW_CLFLUSHOPT) strncat(s, "clflushopt ", mc);
  if (HW_CLWB) strncat(s, "clwb ", mc);
  if (HW_INTEL_PT) strncat(s, "intel_pt ", mc);
  if (HW_AVX512PF) strncat(s, "avx512pf ", mc);
  if (HW_AVX512ER) strncat(s, "avx512er ", mc);
  if (HW_AVX512CD) strncat(s, "avx512cd ", mc);
  if (HW_SHA) strncat(s, "sha ", mc);
  if (HW_AVX512BW) strncat(s, "avx512bw ", mc);
  if (HW_AVX512VL) strncat(s, "avx512vl ", mc);


  if (HW_CLZERO) strncat(s, "clzero ", mc);

  if (HW_XSAVEOPT) strncat(s, "xsaveopt ", mc);
  if (HW_XSAVEC) strncat(s, "xsavec ", mc);
  if (HW_XGETBV) strncat(s, "xgetbv ", mc);
  if (HW_XSAVES) strncat(s, "xsaves ", mc);

  printf("Flags          : %s\n", s);
  free(s);
}


#define action_arch 0
#define action_info 1

int main(int argc, char* argv[])
{
    int ch;

    int action = action_arch;

    while ((ch = getopt(argc, argv, "av")) != -1)
        switch(ch)
        {
          case 'a':
            action = action_info;
            break;
          case 'v':
            break;
        }

    /* detect all flags */
    get_cpu_flags();

    switch(action)
    {
      case action_arch:
        cpu_arch_type();
        break;
      case action_info:
        report_cpu_data();
        all_cpu_flags();
        break;
    }


    return 0;
}
