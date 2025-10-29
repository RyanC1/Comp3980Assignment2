#include "elf_validator.h"
#include <string.h>

int verify_magic(const uint8_t *magic, char *buf)
{
    char msg[MAX_VALIDATION_MSG];
    int  ret_val;

    if(magic[0] == ELFMAG0 && magic[1] == ELFMAG1 && magic[2] == ELFMAG2 && magic[3] == ELFMAG3)
    {
        strcpy(msg, "0x7FELF");
        ret_val = 0;
    }
    else
    {
        strcpy(msg, "Invalid magic numbers");
        ret_val = -1;
    }

    if(buf != NULL)
    {
        strcpy(buf, msg);
    }
    return ret_val;
}

int verify_class(const uint64_t class, char *buf)
{
    char msg[MAX_VALIDATION_MSG];
    int  ret_val;

    switch(class)
    {
        case ELFCLASS64:
            strcpy(msg, "ELF64");
            ret_val = ELFCLASS64;
            break;
        case ELFCLASS32:
            strcpy(msg, "ELF32");
            ret_val = ELFCLASS32;
            break;
        default:
            strcpy(msg, "Invalid class value");
            ret_val = -1;
            break;
    }

    if(buf != NULL)
    {
        strcpy(buf, msg);
    }
    return ret_val;
}

int verify_data(const uint64_t data, char *buf)
{
    char msg[MAX_VALIDATION_MSG];
    int  ret_val;

    switch(data)
    {
        case ELFDATA2MSB:
            strcpy(msg, "Big Endian");
            ret_val = ELFDATA2MSB;
            break;
        case ELFDATA2LSB:
            strcpy(msg, "Little Endian");
            ret_val = ELFDATA2LSB;
            break;
        default:
            strcpy(msg, "Invalid data encoding value");
            ret_val = -1;
            break;
    }

    if(buf != NULL)
    {
        strcpy(buf, msg);
    }
    return ret_val;
}

int verify_version(const uint64_t version, char *buf)
{
    char msg[MAX_VALIDATION_MSG];
    int  ret_val;

    if(version == EV_CURRENT)
    {
        strcpy(msg, "EV_CURRENT");
        ret_val = EV_CURRENT;
    }
    else
    {
        strcpy(msg, "Invalid version value");
        ret_val = -1;
    }

    if(buf != NULL)
    {
        strcpy(buf, msg);
    }
    return ret_val;
}

int verify_type(const uint64_t type, char *buf)
{
    char msg[MAX_VALIDATION_MSG];
    int  ret_val;

    switch(type)
    {
        case ET_CORE:
            strcpy(msg, "Core (ET_CORE)");
            ret_val = ET_CORE;
            break;
        case ET_DYN:
            strcpy(msg, "Shared Object (ET_DYN)");
            ret_val = ET_DYN;
            break;
        case ET_EXEC:
            strcpy(msg, "Executable (ET_EXEC)");
            ret_val = ET_EXEC;
            break;
        case ET_REL:
            strcpy(msg, "Relocatable (ET_REL)");
            ret_val = ET_REL;
            break;
        default:
            strcpy(msg, "Invalid file type value");
            ret_val = -1;
            break;
    }

    if(buf != NULL)
    {
        strcpy(buf, msg);
    }
    return ret_val;
}

int verify_machine(const uint64_t machine, char *buf)
{
    char msg[MAX_VALIDATION_MSG];
    int  ret_val;

    switch(machine)
    {
        case EM_M32:
            strcpy(msg, "AT&T WE 32100 (EM_M32)");
            ret_val = EM_M32;
            break;
        case EM_SPARC:
            strcpy(msg, "SPARC (EM_SPARC)");
            ret_val = EM_SPARC;
            break;
        case EM_386:
            strcpy(msg, "Intel 80386 (EM_386)");
            ret_val = EM_386;
            break;
        case EM_68K:
            strcpy(msg, "Motorola 68000 (EM_68K)");
            ret_val = EM_68K;
            break;
        case EM_88K:
            strcpy(msg, "Motorola 88000 (EM_88K)");
            ret_val = EM_88K;
            break;
        case EM_IAMCU:
            strcpy(msg, "Intel MCU (EM_IAMCU)");
            ret_val = EM_IAMCU;
            break;
        case EM_860:
            strcpy(msg, "Intel 80860 (EM_860)");
            ret_val = EM_860;
            break;
        case EM_MIPS:
            strcpy(msg, "MIPS I Architecture (EM_MIPS)");
            ret_val = EM_MIPS;
            break;
        case EM_S370:
            strcpy(msg, "IBM System/370 Processor (EM_S370)");
            ret_val = EM_S370;
            break;
        case EM_MIPS_RS3_LE:
            strcpy(msg, "MIPS RS3000 Little-endian (EM_MIPS_RS3_LE)");
            ret_val = EM_MIPS_RS3_LE;
            break;
        case EM_PARISC:
            strcpy(msg, "Hewlett-Packard PA-RISC (EM_PARISC)");
            ret_val = EM_PARISC;
            break;
        case EM_VPP500:
            strcpy(msg, "Fujitsu VPP500 (EM_VPP500)");
            ret_val = EM_VPP500;
            break;
        case EM_SPARC32PLUS:
            strcpy(msg, "Enhanced instruction set SPARC (EM_SPARC32PLUS)");
            ret_val = EM_SPARC32PLUS;
            break;
        case EM_960:
            strcpy(msg, "Intel 80960 (EM_960)");
            ret_val = EM_960;
            break;
        case EM_PPC:
            strcpy(msg, "PowerPC (EM_PPC)");
            ret_val = EM_PPC;
            break;
        case EM_PPC64:
            strcpy(msg, "64-bit PowerPC (EM_PPC64)");
            ret_val = EM_PPC64;
            break;
        case EM_S390:
            strcpy(msg, "IBM System/390 Processor (EM_S390)");
            ret_val = EM_S390;
            break;
        case EM_SPU:
            strcpy(msg, "IBM SPU/SPC (EM_SPU)");
            ret_val = EM_SPU;
            break;
        case EM_V800:
            strcpy(msg, "NEC V800 (EM_V800)");
            ret_val = EM_V800;
            break;
        case EM_FR20:
            strcpy(msg, "Fujitsu FR20 (EM_FR20)");
            ret_val = EM_FR20;
            break;
        case EM_RH32:
            strcpy(msg, "TRW RH-32 (EM_RH32)");
            ret_val = EM_RH32;
            break;
        case EM_RCE:
            strcpy(msg, "Motorola RCE (EM_RCE)");
            ret_val = EM_RCE;
            break;
        case EM_ARM:
            strcpy(msg, "ARM 32-bit architecture (AARCH32) (EM_ARM)");
            ret_val = EM_ARM;
            break;
        case EM_ALPHA:
            strcpy(msg, "Digital Alpha (EM_ALPHA)");
            ret_val = EM_ALPHA;
            break;
        case EM_SH:
            strcpy(msg, "Hitachi SH (EM_SH)");
            ret_val = EM_SH;
            break;
        case EM_SPARCV9:
            strcpy(msg, "SPARC Version 9 (EM_SPARCV9)");
            ret_val = EM_SPARCV9;
            break;
        case EM_TRICORE:
            strcpy(msg, "Siemens TriCore embedded processor (EM_TRICORE)");
            ret_val = EM_TRICORE;
            break;
        case EM_ARC:
            strcpy(msg, "Argonaut RISC Core, Argonaut Technologies Inc. (EM_ARC)");
            ret_val = EM_ARC;
            break;
        case EM_H8_300:
            strcpy(msg, "Hitachi H8/300 (EM_H8_300)");
            ret_val = EM_H8_300;
            break;
        case EM_H8_300H:
            strcpy(msg, "Hitachi H8/300H (EM_H8_300H)");
            ret_val = EM_H8_300H;
            break;
        case EM_H8S:
            strcpy(msg, "Hitachi H8S (EM_H8S)");
            ret_val = EM_H8S;
            break;
        case EM_H8_500:
            strcpy(msg, "Hitachi H8/500 (EM_H8_500)");
            ret_val = EM_H8_500;
            break;
        case EM_IA_64:
            strcpy(msg, "Intel IA-64 processor architecture (EM_IA_64)");
            ret_val = EM_IA_64;
            break;
        case EM_MIPS_X:
            strcpy(msg, "Stanford MIPS-X (EM_MIPS_X)");
            ret_val = EM_MIPS_X;
            break;
        case EM_COLDFIRE:
            strcpy(msg, "Motorola ColdFire (EM_COLDFIRE)");
            ret_val = EM_COLDFIRE;
            break;
        case EM_68HC12:
            strcpy(msg, "Motorola M68HC12 (EM_68HC12)");
            ret_val = EM_68HC12;
            break;
        case EM_MMA:
            strcpy(msg, "Fujitsu MMA Multimedia Accelerator (EM_MMA)");
            ret_val = EM_MMA;
            break;
        case EM_PCP:
            strcpy(msg, "Siemens PCP (EM_PCP)");
            ret_val = EM_PCP;
            break;
        case EM_NCPU:
            strcpy(msg, "Sony nCPU embedded RISC processor (EM_NCPU)");
            ret_val = EM_NCPU;
            break;
        case EM_NDR1:
            strcpy(msg, "Denso NDR1 microprocessor (EM_NDR1)");
            ret_val = EM_NDR1;
            break;
        case EM_STARCORE:
            strcpy(msg, "Motorola Star*Core processor (EM_STARCORE)");
            ret_val = EM_STARCORE;
            break;
        case EM_ME16:
            strcpy(msg, "Toyota ME16 processor (EM_ME16)");
            ret_val = EM_ME16;
            break;
        case EM_ST100:
            strcpy(msg, "STMicroelectronics ST100 processor (EM_ST100)");
            ret_val = EM_ST100;
            break;
        case EM_TINYJ:
            strcpy(msg, "Advanced Logic Corp. TinyJ embedded processor family (EM_TINYJ)");
            ret_val = EM_TINYJ;
            break;
        case EM_X86_64:
            strcpy(msg, "AMD x86-64 architecture (EM_X86_64)");
            ret_val = EM_X86_64;
            break;
        case EM_PDSP:
            strcpy(msg, "Sony DSP Processor (EM_PDSP)");
            ret_val = EM_PDSP;
            break;
        case EM_PDP10:
            strcpy(msg, "Digital Equipment Corp. PDP-10 (EM_PDP10)");
            ret_val = EM_PDP10;
            break;
        case EM_PDP11:
            strcpy(msg, "Digital Equipment Corp. PDP-11 (EM_PDP11)");
            ret_val = EM_PDP11;
            break;
        case EM_FX66:
            strcpy(msg, "Siemens FX66 microcontroller (EM_FX66)");
            ret_val = EM_FX66;
            break;
        case EM_ST9PLUS:
            strcpy(msg, "STMicroelectronics ST9+ 8/16 bit microcontroller (EM_ST9PLUS)");
            ret_val = EM_ST9PLUS;
            break;
        case EM_ST7:
            strcpy(msg, "STMicroelectronics ST7 8-bit microcontroller (EM_ST7)");
            ret_val = EM_ST7;
            break;
        case EM_68HC16:
            strcpy(msg, "Motorola MC68HC16 Microcontroller (EM_68HC16)");
            ret_val = EM_68HC16;
            break;
        case EM_68HC11:
            strcpy(msg, "Motorola MC68HC11 Microcontroller (EM_68HC11)");
            ret_val = EM_68HC11;
            break;
        case EM_68HC08:
            strcpy(msg, "Motorola MC68HC08 Microcontroller (EM_68HC08)");
            ret_val = EM_68HC08;
            break;
        case EM_68HC05:
            strcpy(msg, "Motorola MC68HC05 Microcontroller (EM_68HC05)");
            ret_val = EM_68HC05;
            break;
        case EM_SVX:
            strcpy(msg, "Silicon Graphics SVx (EM_SVX)");
            ret_val = EM_SVX;
            break;
        case EM_ST19:
            strcpy(msg, "STMicroelectronics ST19 8-bit microcontroller (EM_ST19)");
            ret_val = EM_ST19;
            break;
        case EM_VAX:
            strcpy(msg, "Digital VAX (EM_VAX)");
            ret_val = EM_VAX;
            break;
        case EM_CRIS:
            strcpy(msg, "Axis Communications 32-bit embedded processor (EM_CRIS)");
            ret_val = EM_CRIS;
            break;
        case EM_JAVELIN:
            strcpy(msg, "Infineon Technologies 32-bit embedded processor (EM_JAVELIN)");
            ret_val = EM_JAVELIN;
            break;
        case EM_FIREPATH:
            strcpy(msg, "Element 14 64-bit DSP Processor (EM_FIREPATH)");
            ret_val = EM_FIREPATH;
            break;
        case EM_ZSP:
            strcpy(msg, "LSI Logic 16-bit DSP Processor (EM_ZSP)");
            ret_val = EM_ZSP;
            break;
        case EM_MMIX:
            strcpy(msg, "Donald Knuth’s educational 64-bit processor (EM_MMIX)");
            ret_val = EM_MMIX;
            break;
        case EM_HUANY:
            strcpy(msg, "Harvard University machine-independent object files (EM_HUANY)");
            ret_val = EM_HUANY;
            break;
        case EM_PRISM:
            strcpy(msg, "SiTera Prism (EM_PRISM)");
            ret_val = EM_PRISM;
            break;
        case EM_AVR:
            strcpy(msg, "Atmel AVR 8-bit microcontroller (EM_AVR)");
            ret_val = EM_AVR;
            break;
        case EM_FR30:
            strcpy(msg, "Fujitsu FR30 (EM_FR30)");
            ret_val = EM_FR30;
            break;
        case EM_D10V:
            strcpy(msg, "Mitsubishi D10V (EM_D10V)");
            ret_val = EM_D10V;
            break;
        case EM_D30V:
            strcpy(msg, "Mitsubishi D30V (EM_D30V)");
            ret_val = EM_D30V;
            break;
        case EM_V850:
            strcpy(msg, "NEC v850 (EM_V850)");
            ret_val = EM_V850;
            break;
        case EM_M32R:
            strcpy(msg, "Mitsubishi M32R (EM_M32R)");
            ret_val = EM_M32R;
            break;
        case EM_MN10300:
            strcpy(msg, "Matsushita MN10300 (EM_MN10300)");
            ret_val = EM_MN10300;
            break;
        case EM_MN10200:
            strcpy(msg, "Matsushita MN10200 (EM_MN10200)");
            ret_val = EM_MN10200;
            break;
        case EM_PJ:
            strcpy(msg, "picoJava (EM_PJ)");
            ret_val = EM_PJ;
            break;
        case EM_OPENRISC:
            strcpy(msg, "OpenRISC 32-bit embedded processor (EM_OPENRISC)");
            ret_val = EM_OPENRISC;
            break;
        case EM_ARC_COMPACT:
            strcpy(msg, "ARC International ARCompact processor (old spelling/synonym: EM_ARC_A5) (EM_ARC_COMPACT)");
            ret_val = EM_ARC_COMPACT;
            break;
        case EM_XTENSA:
            strcpy(msg, "Tensilica Xtensa Architecture (EM_XTENSA)");
            ret_val = EM_XTENSA;
            break;
        case EM_VIDEOCORE:
            strcpy(msg, "Alphamosaic VideoCore processor (EM_VIDEOCORE)");
            ret_val = EM_VIDEOCORE;
            break;
        case EM_TMM_GPP:
            strcpy(msg, "Thompson Multimedia General Purpose Processor (EM_TMM_GPP)");
            ret_val = EM_TMM_GPP;
            break;
        case EM_NS32K:
            strcpy(msg, "National Semiconductor 32000 series (EM_NS32K)");
            ret_val = EM_NS32K;
            break;
        case EM_TPC:
            strcpy(msg, "Tenor Network TPC processor (EM_TPC)");
            ret_val = EM_TPC;
            break;
        case EM_SNP1K:
            strcpy(msg, "Trebia SNP 1000 processor (EM_SNP1K)");
            ret_val = EM_SNP1K;
            break;
        case EM_ST200:
            strcpy(msg, "STMicroelectronics (www.st.com) ST200 microcontroller (EM_ST200)");
            ret_val = EM_ST200;
            break;
        case EM_IP2K:
            strcpy(msg, "Ubicom IP2xxx microcontroller family (EM_IP2K)");
            ret_val = EM_IP2K;
            break;
        case EM_MAX:
            strcpy(msg, "MAX Processor (EM_MAX)");
            ret_val = EM_MAX;
            break;
        case EM_CR:
            strcpy(msg, "National Semiconductor CompactRISC microprocessor (EM_CR)");
            ret_val = EM_CR;
            break;
        case EM_F2MC16:
            strcpy(msg, "Fujitsu F2MC16 (EM_F2MC16)");
            ret_val = EM_F2MC16;
            break;
        case EM_MSP430:
            strcpy(msg, "Texas Instruments embedded microcontroller msp430 (EM_MSP430)");
            ret_val = EM_MSP430;
            break;
        case EM_BLACKFIN:
            strcpy(msg, "Analog Devices Blackfin (DSP) processor (EM_BLACKFIN)");
            ret_val = EM_BLACKFIN;
            break;
        case EM_SE_C33:
            strcpy(msg, "S1C33 Family of Seiko Epson processors (EM_SE_C33)");
            ret_val = EM_SE_C33;
            break;
        case EM_SEP:
            strcpy(msg, "Sharp embedded microprocessor (EM_SEP)");
            ret_val = EM_SEP;
            break;
        case EM_ARCA:
            strcpy(msg, "Arca RISC Microprocessor (EM_ARCA)");
            ret_val = EM_ARCA;
            break;
        case EM_UNICORE:
            strcpy(msg, "Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University (EM_UNICORE)");
            ret_val = EM_UNICORE;
            break;
        case EM_EXCESS:
            strcpy(msg, "eXcess: 16/32/64-bit configurable embedded CPU (EM_EXCESS)");
            ret_val = EM_EXCESS;
            break;
        case EM_DXP:
            strcpy(msg, "Icera Semiconductor Inc. Deep Execution Processor (EM_DXP)");
            ret_val = EM_DXP;
            break;
        case EM_ALTERA_NIOS2:
            strcpy(msg, "Altera Nios II soft-core processor (EM_ALTERA_NIOS2)");
            ret_val = EM_ALTERA_NIOS2;
            break;
        case EM_CRX:
            strcpy(msg, "National Semiconductor CompactRISC CRX microprocessor (EM_CRX)");
            ret_val = EM_CRX;
            break;
        case EM_XGATE:
            strcpy(msg, "Motorola XGATE embedded processor (EM_XGATE)");
            ret_val = EM_XGATE;
            break;
        case EM_C166:
            strcpy(msg, "Infineon C16x/XC16x processor (EM_C166)");
            ret_val = EM_C166;
            break;
        case EM_M16C:
            strcpy(msg, "Renesas M16C series microprocessors (EM_M16C)");
            ret_val = EM_M16C;
            break;
        case EM_DSPIC30F:
            strcpy(msg, "Microchip Technology dsPIC30F Digital Signal Controller (EM_DSPIC30F)");
            ret_val = EM_DSPIC30F;
            break;
        case EM_CE:
            strcpy(msg, "Freescale Communication Engine RISC core (EM_CE)");
            ret_val = EM_CE;
            break;
        case EM_M32C:
            strcpy(msg, "Renesas M32C series microprocessors (EM_M32C)");
            ret_val = EM_M32C;
            break;
        case EM_TSK3000:
            strcpy(msg, "Altium TSK3000 core (EM_TSK3000)");
            ret_val = EM_TSK3000;
            break;
        case EM_RS08:
            strcpy(msg, "Freescale RS08 embedded processor (EM_RS08)");
            ret_val = EM_RS08;
            break;
        case EM_SHARC:
            strcpy(msg, "Analog Devices SHARC family of 32-bit DSP processors (EM_SHARC)");
            ret_val = EM_SHARC;
            break;
        case EM_ECOG2:
            strcpy(msg, "Cyan Technology eCOG2 microprocessor (EM_ECOG2)");
            ret_val = EM_ECOG2;
            break;
        case EM_SCORE7:
            strcpy(msg, "Sunplus S+core7 RISC processor (EM_SCORE7)");
            ret_val = EM_SCORE7;
            break;
        case EM_DSP24:
            strcpy(msg, "New Japan Radio (NJR) 24-bit DSP Processor (EM_DSP24)");
            ret_val = EM_DSP24;
            break;
        case EM_VIDEOCORE3:
            strcpy(msg, "Broadcom VideoCore III processor (EM_VIDEOCORE3)");
            ret_val = EM_VIDEOCORE3;
            break;
        case EM_LATTICEMICO32:
            strcpy(msg, "RISC processor for Lattice FPGA architecture (EM_LATTICEMICO32)");
            ret_val = EM_LATTICEMICO32;
            break;
        case EM_SE_C17:
            strcpy(msg, "Seiko Epson C17 family (EM_SE_C17)");
            ret_val = EM_SE_C17;
            break;
        case EM_TI_C6000:
            strcpy(msg, "The Texas Instruments TMS320C6000 DSP family (EM_TI_C6000)");
            ret_val = EM_TI_C6000;
            break;
        case EM_TI_C2000:
            strcpy(msg, "The Texas Instruments TMS320C2000 DSP family (EM_TI_C2000)");
            ret_val = EM_TI_C2000;
            break;
        case EM_TI_C5500:
            strcpy(msg, "The Texas Instruments TMS320C55x DSP family (EM_TI_C5500)");
            ret_val = EM_TI_C5500;
            break;
        case EM_TI_ARP32:
            strcpy(msg, "Texas Instruments Application Specific RISC Processor, 32bit fetch (EM_TI_ARP32)");
            ret_val = EM_TI_ARP32;
            break;
        case EM_TI_PRU:
            strcpy(msg, "Texas Instruments Programmable Realtime Unit (EM_TI_PRU)");
            ret_val = EM_TI_PRU;
            break;
        case EM_MMDSP_PLUS:
            strcpy(msg, "STMicroelectronics 64bit VLIW Data Signal Processor (EM_MMDSP_PLUS)");
            ret_val = EM_MMDSP_PLUS;
            break;
        case EM_CYPRESS_M8C:
            strcpy(msg, "Cypress M8C microprocessor (EM_CYPRESS_M8C)");
            ret_val = EM_CYPRESS_M8C;
            break;
        case EM_R32C:
            strcpy(msg, "Renesas R32C series microprocessors (EM_R32C)");
            ret_val = EM_R32C;
            break;
        case EM_TRIMEDIA:
            strcpy(msg, "NXP Semiconductors TriMedia architecture family (EM_TRIMEDIA)");
            ret_val = EM_TRIMEDIA;
            break;
        case EM_QDSP6:
            strcpy(msg, "QUALCOMM DSP6 Processor (EM_QDSP6)");
            ret_val = EM_QDSP6;
            break;
        case EM_8051:
            strcpy(msg, "Intel 8051 and variants (EM_8051)");
            ret_val = EM_8051;
            break;
        case EM_STXP7X:
            strcpy(msg, "STMicroelectronics STxP7x family of configurable and extensible RISC processors (EM_STXP7X)");
            ret_val = EM_STXP7X;
            break;
        case EM_NDS32:
            strcpy(msg, "Andes Technology compact code size embedded RISC processor family (EM_NDS32)");
            ret_val = EM_NDS32;
            break;
        case EM_ECOG1X:
            strcpy(msg, "Cyan Technology eCOG1X family (EM_ECOG1X)");
            ret_val = EM_ECOG1X;
            break;
        case EM_MAXQ30:
            strcpy(msg, "Dallas Semiconductor MAXQ30 Core Micro-controllers (EM_MAXQ30)");
            ret_val = EM_MAXQ30;
            break;
        case EM_XIMO16:
            strcpy(msg, "New Japan Radio (NJR) 16-bit DSP Processor (EM_XIMO16)");
            ret_val = EM_XIMO16;
            break;
        case EM_MANIK:
            strcpy(msg, "M2000 Reconfigurable RISC Microprocessor (EM_MANIK)");
            ret_val = EM_MANIK;
            break;
        case EM_CRAYNV2:
            strcpy(msg, "Cray Inc. NV2 vector architecture (EM_CRAYNV2)");
            ret_val = EM_CRAYNV2;
            break;
        case EM_RX:
            strcpy(msg, "Renesas RX family (EM_RX)");
            ret_val = EM_RX;
            break;
        case EM_METAG:
            strcpy(msg, "Imagination Technologies META processor architecture (EM_METAG)");
            ret_val = EM_METAG;
            break;
        case EM_MCST_ELBRUS:
            strcpy(msg, "MCST Elbrus general purpose hardware architecture (EM_MCST_ELBRUS)");
            ret_val = EM_MCST_ELBRUS;
            break;
        case EM_ECOG16:
            strcpy(msg, "Cyan Technology eCOG16 family (EM_ECOG16)");
            ret_val = EM_ECOG16;
            break;
        case EM_CR16:
            strcpy(msg, "National Semiconductor CompactRISC CR16 16-bit microprocessor (EM_CR16)");
            ret_val = EM_CR16;
            break;
        case EM_ETPU:
            strcpy(msg, "Freescale Extended Time Processing Unit (EM_ETPU)");
            ret_val = EM_ETPU;
            break;
        case EM_SLE9X:
            strcpy(msg, "Infineon Technologies SLE9X core (EM_SLE9X)");
            ret_val = EM_SLE9X;
            break;
        case EM_L10M:
            strcpy(msg, "Intel L10M (EM_L10M)");
            ret_val = EM_L10M;
            break;
        case EM_K10M:
            strcpy(msg, "Intel K10M (EM_K10M)");
            ret_val = EM_K10M;
            break;
        case EM_AARCH64:
            strcpy(msg, "ARM 64-bit architecture (AARCH64) (EM_AARCH64)");
            ret_val = EM_AARCH64;
            break;
        case EM_AVR32:
            strcpy(msg, "Atmel Corporation 32-bit microprocessor family (EM_AVR32)");
            ret_val = EM_AVR32;
            break;
        case EM_STM8:
            strcpy(msg, "STMicroeletronics STM8 8-bit microcontroller (EM_STM8)");
            ret_val = EM_STM8;
            break;
        case EM_TILE64:
            strcpy(msg, "Tilera TILE64 multicore architecture family (EM_TILE64)");
            ret_val = EM_TILE64;
            break;
        case EM_TILEPRO:
            strcpy(msg, "Tilera TILEPro multicore architecture family (EM_TILEPRO)");
            ret_val = EM_TILEPRO;
            break;
        case EM_MICROBLAZE:
            strcpy(msg, "Xilinx MicroBlaze 32-bit RISC soft processor core (EM_MICROBLAZE)");
            ret_val = EM_MICROBLAZE;
            break;
        case EM_CUDA:
            strcpy(msg, "NVIDIA CUDA architecture (EM_CUDA)");
            ret_val = EM_CUDA;
            break;
        case EM_TILEGX:
            strcpy(msg, "Tilera TILE-Gx multicore architecture family (EM_TILEGX)");
            ret_val = EM_TILEGX;
            break;
        case EM_CLOUDSHIELD:
            strcpy(msg, "CloudShield architecture family (EM_CLOUDSHIELD)");
            ret_val = EM_CLOUDSHIELD;
            break;
        case EM_COREA_1ST:
            strcpy(msg, "KIPO-KAIST Core-A 1st generation processor family (EM_COREA_1ST)");
            ret_val = EM_COREA_1ST;
            break;
        case EM_COREA_2ND:
            strcpy(msg, "KIPO-KAIST Core-A 2nd generation processor family (EM_COREA_2ND)");
            ret_val = EM_COREA_2ND;
            break;
        case EM_ARC_COMPACT2:
            strcpy(msg, "Synopsys ARCompact V2 (EM_ARC_COMPACT2)");
            ret_val = EM_ARC_COMPACT2;
            break;
        case EM_OPEN8:
            strcpy(msg, "Open8 8-bit RISC soft processor core (EM_OPEN8)");
            ret_val = EM_OPEN8;
            break;
        case EM_RL78:
            strcpy(msg, "Renesas RL78 family (EM_RL78)");
            ret_val = EM_RL78;
            break;
        case EM_VIDEOCORE5:
            strcpy(msg, "Broadcom VideoCore V processor (EM_VIDEOCORE5)");
            ret_val = EM_VIDEOCORE5;
            break;
        case EM_78KOR:
            strcpy(msg, "Renesas 78KOR family (EM_78KOR)");
            ret_val = EM_78KOR;
            break;
        case EM_56800EX:
            strcpy(msg, "Freescale 56800EX Digital Signal Controller (DSC) (EM_56800EX)");
            ret_val = EM_56800EX;
            break;
        case EM_BA1:
            strcpy(msg, "Beyond BA1 CPU architecture (EM_BA1)");
            ret_val = EM_BA1;
            break;
        case EM_BA2:
            strcpy(msg, "Beyond BA2 CPU architecture (EM_BA2)");
            ret_val = EM_BA2;
            break;
        case EM_XCORE:
            strcpy(msg, "XMOS xCORE processor family (EM_XCORE)");
            ret_val = EM_XCORE;
            break;
        case EM_MCHP_PIC:
            strcpy(msg, "Microchip 8-bit PIC(r) family (EM_MCHP_PIC)");
            ret_val = EM_MCHP_PIC;
            break;
        case EM_INTEL205:
            strcpy(msg, "Reserved by Intel (EM_INTEL205)");
            ret_val = EM_INTEL205;
            break;
        case EM_INTEL206:
            strcpy(msg, "Reserved by Intel (EM_INTEL206)");
            ret_val = EM_INTEL206;
            break;
        case EM_INTEL207:
            strcpy(msg, "Reserved by Intel (EM_INTEL207)");
            ret_val = EM_INTEL207;
            break;
        case EM_INTEL208:
            strcpy(msg, "Reserved by Intel (EM_INTEL208)");
            ret_val = EM_INTEL208;
            break;
        case EM_INTEL209:
            strcpy(msg, "Reserved by Intel (EM_INTEL209)");
            ret_val = EM_INTEL209;
            break;
        case EM_KM32:
            strcpy(msg, "KM211 KM32 32-bit processor (EM_KM32)");
            ret_val = EM_KM32;
            break;
        case EM_KMX32:
            strcpy(msg, "KM211 KMX32 32-bit processor (EM_KMX32)");
            ret_val = EM_KMX32;
            break;
        case EM_KMX16:
            strcpy(msg, "KM211 KMX16 16-bit processor (EM_KMX16)");
            ret_val = EM_KMX16;
            break;
        case EM_KMX8:
            strcpy(msg, "KM211 KMX8 8-bit processor (EM_KMX8)");
            ret_val = EM_KMX8;
            break;
        case EM_KVARC:
            strcpy(msg, "KM211 KVARC processor (EM_KVARC)");
            ret_val = EM_KVARC;
            break;
        case EM_CDP:
            strcpy(msg, "Paneve CDP architecture family (EM_CDP)");
            ret_val = EM_CDP;
            break;
        case EM_COGE:
            strcpy(msg, "Cognitive Smart Memory Processor (EM_COGE)");
            ret_val = EM_COGE;
            break;
        case EM_COOL:
            strcpy(msg, "Bluechip Systems CoolEngine (EM_COOL)");
            ret_val = EM_COOL;
            break;
        case EM_NORC:
            strcpy(msg, "Nanoradio Optimized RISC (EM_NORC)");
            ret_val = EM_NORC;
            break;
        case EM_CSR_KALIMBA:
            strcpy(msg, "CSR Kalimba architecture family (EM_CSR_KALIMBA)");
            ret_val = EM_CSR_KALIMBA;
            break;
        case EM_Z80:
            strcpy(msg, "Zilog Z80 (EM_Z80)");
            ret_val = EM_Z80;
            break;
        case EM_VISIUM:
            strcpy(msg, "Controls and Data Services VISIUMcore processor (EM_VISIUM)");
            ret_val = EM_VISIUM;
            break;
        case EM_FT32:
            strcpy(msg, "FTDI Chip FT32 high performance 32-bit RISC architecture (EM_FT32)");
            ret_val = EM_FT32;
            break;
        case EM_MOXIE:
            strcpy(msg, "Moxie processor family (EM_MOXIE)");
            ret_val = EM_MOXIE;
            break;
        case EM_AMDGPU:
            strcpy(msg, "AMD GPU architecture (EM_AMDGPU)");
            ret_val = EM_AMDGPU;
            break;
        case EM_RISCV:
            strcpy(msg, "RISC-V (EM_RISCV)");
            ret_val = EM_RISCV;
            break;
        case EM_LANAI:
            strcpy(msg, "Lanai processor (EM_LANAI)");
            ret_val = EM_LANAI;
            break;
        case EM_CEVA:
            strcpy(msg, "CEVA Processor Architecture Family (EM_CEVA)");
            ret_val = EM_CEVA;
            break;
        case EM_CEVA_X2:
            strcpy(msg, "CEVA X2 Processor Family (EM_CEVA_X2)");
            ret_val = EM_CEVA_X2;
            break;
        case EM_BPF:
            strcpy(msg, "Linux BPF – in-kernel virtual machine (EM_BPF)");
            ret_val = EM_BPF;
            break;
        case EM_GRAPHCORE_IPU:
            strcpy(msg, "Graphcore Intelligent Processing Unit (EM_GRAPHCORE_IPU)");
            ret_val = EM_GRAPHCORE_IPU;
            break;
        case EM_IMG1:
            strcpy(msg, "Imagination Technologies (EM_IMG1)");
            ret_val = EM_IMG1;
            break;
        case EM_NFP:
            strcpy(msg, "Netronome Flow Processor (NFP) (EM_NFP)");
            ret_val = EM_NFP;
            break;
        case EM_VE:
            strcpy(msg, "NEC Vector Engine (EM_VE)");
            ret_val = EM_VE;
            break;
        case EM_CSKY:
            strcpy(msg, "C-SKY processor family (EM_CSKY)");
            ret_val = EM_CSKY;
            break;
        case EM_ARC_COMPACT3_64:
            strcpy(msg, "Synopsys ARCv2.3 64-bit (EM_ARC_COMPACT3_64)");
            ret_val = EM_ARC_COMPACT3_64;
            break;
        case EM_MCS6502:
            strcpy(msg, "MOS Technology MCS 6502 processor (EM_MCS6502)");
            ret_val = EM_MCS6502;
            break;
        case EM_ARC_COMPACT3:
            strcpy(msg, "Synopsys ARCv2.3 32-bit (EM_ARC_COMPACT3)");
            ret_val = EM_ARC_COMPACT3;
            break;
        case EM_KVX:
            strcpy(msg, "Kalray VLIW core of the MPPA processor family (EM_KVX)");
            ret_val = EM_KVX;
            break;
        case EM_65816:
            strcpy(msg, "WDC 65816/65C816 (EM_65816)");
            ret_val = EM_65816;
            break;
        case EM_LOONGARCH:
            strcpy(msg, "Loongson Loongarch (EM_LOONGARCH)");
            ret_val = EM_LOONGARCH;
            break;
        case EM_KF32:
            strcpy(msg, "ChipON KungFu32 (EM_KF32)");
            ret_val = EM_KF32;
            break;
        case EM_U16_U8CORE:
            strcpy(msg, "LAPIS nX-U16/U8 (EM_U16_U8CORE)");
            ret_val = EM_U16_U8CORE;
            break;
        case EM_TACHYUM:
            strcpy(msg, "Reserved for Tachyum processor (EM_TACHYUM)");
            ret_val = EM_TACHYUM;
            break;
        case EM_56800EF:
            strcpy(msg, "NXP 56800EF Digital Signal Controller (DSC) (EM_56800EF)");
            ret_val = EM_56800EF;
            break;
        case EM_SBF:
            strcpy(msg, "Solana Bytecode Format (EM_SBF)");
            ret_val = EM_SBF;
            break;
        case EM_AIENGINE:
            strcpy(msg, "AMD/Xilinx AIEngine architecture (EM_AIENGINE)");
            ret_val = EM_AIENGINE;
            break;
        case EM_SIMA_MLA:
            strcpy(msg, "SiMa MLA (EM_SIMA_MLA)");
            ret_val = EM_SIMA_MLA;
            break;
        case EM_BANG:
            strcpy(msg, "Cambricon BANG (EM_BANG)");
            ret_val = EM_BANG;
            break;
        case EM_LOONGGPU:
            strcpy(msg, "Loongson LoongGPU (EM_LOONGGPU)");
            ret_val = EM_LOONGGPU;
            break;
        case EM_SW64:
            strcpy(msg, "Wuxi Institute of Advanced Technology SW64 (EM_SW64)");
            ret_val = EM_SW64;
            break;
        case EM_AIECTRLCODE:
            strcpy(msg, "AMD/Xilinx AIEngine ctrlcode (EM_AIECTRLCODE)");
            ret_val = EM_AIECTRLCODE;
            break;
        default:
            strcpy(msg, "Invalid machine architecture value");
            ret_val = -1;
            break;
    }

    if(buf != NULL)
    {
        strcpy(buf, msg);
    }
    return ret_val;
}
