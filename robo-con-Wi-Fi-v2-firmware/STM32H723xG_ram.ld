/*
******************************************************************************
**

**  File        : LinkerScript.ld
**
**  Author		: maidui3
**
**  Abstract    : Linker script for STM32H723VGTx series
**                1024Kbytes FLASH and 560Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** <h2><center>&copy; COPYRIGHT(c) 2025 STMicroelectronics</center></h2>
**
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
**   1. Redistributions of source code must retain the above copyright notice,
**      this list of conditions and the following disclaimer.
**   2. Redistributions in binary form must reproduce the above copyright notice,
**      this list of conditions and the following disclaimer in the documentation
**      and/or other materials provided with the distribution.
**   3. Neither the name of STMicroelectronics nor the names of its contributors
**      may be used to endorse or promote products derived from this software
**      without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Specify the memory areas */
MEMORY
{
DTCMRAM (xrw)     : ORIGIN = 0x20000000, LENGTH = 128K
RAM_D1 (xrw)      : ORIGIN = 0x24000000, LENGTH = 320K
RAM_D2 (xrw)      : ORIGIN = 0x30000000, LENGTH = 32K
RAM_D3 (xrw)      : ORIGIN = 0x38000000, LENGTH = 16K
ITCMRAM (xrw)     : ORIGIN = 0x00000000, LENGTH = 64K
FLASH (rx)        : ORIGIN = 0x08000000, LENGTH = 1024K
RAM_BackUp(xrw)   : ORIGIN = 0x38800000, LENGTH = 4K
}

/* Highest address of the user mode stack */
_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);    /* end of RAM */
_sstack = _estack - _Min_Stack_Size;
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x8000;      /* required amount of heap  */
_Min_Stack_Size = 0x2000; /* required amount of stack */

/* Define output sections */
SECTIONS
{
/*************************************************** CODE BEGIN ***************************************************/
/*---------------------------------------- itcm BEGIN ----------------------------------------*/
  _siitcm = LOADADDR(.itcm);
  .itcm :
  {
    . = ALIGN(4);
    _sitcm = .;
    *(itcm)
    *(itcm*)
    . = ALIGN(4);
    _eitcm = .;
  } >ITCMRAM AT> FLASH
/*---------------------------------------- itcm END ----------------------------------------*/

/*---------------------------------------- text BEGIN ----------------------------------------*/
  _sitext = LOADADDR(.text);
  .text :
  {
    . = ALIGN(4);
    _stext = .;

    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >RAM_D1 AT> FLASH
/*---------------------------------------- text END ----------------------------------------*/

/*---------------------------------------- text_flash BEGIN ----------------------------------------*/
  .text_flash :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    
    *(text_flash)
    *(text_flash*)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext_flash = .;
  } >FLASH
/*---------------------------------------- text_flash END ----------------------------------------*/

/*---------------------------------------- rodata BEGIN ----------------------------------------*/
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH
/*---------------------------------------- rodata END ----------------------------------------*/

/*---------------------------------------- XXXX BEGIN ----------------------------------------*/
  .ARM.extab (READONLY) : /* The "READONLY" keyword is only supported in GCC11 and later, remove it if using GCC10 or earlier. */
  {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >FLASH

  .ARM (READONLY) : /* The "READONLY" keyword is only supported in GCC11 and later, remove it if using GCC10 or earlier. */
  {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >FLASH

  .preinit_array (READONLY) : /* The "READONLY" keyword is only supported in GCC11 and later, remove it if using GCC10 or earlier. */
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >FLASH

  .init_array (READONLY) : /* The "READONLY" keyword is only supported in GCC11 and later, remove it if using GCC10 or earlier. */
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >FLASH

  .fini_array (READONLY) : /* The "READONLY" keyword is only supported in GCC11 and later, remove it if using GCC10 or earlier. */
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >FLASH
/*---------------------------------------- XXXX END ----------------------------------------*/
/*************************************************** CODE END ***************************************************/

/*************************************************** DATA BEGIN ***************************************************/
/*---------------------------------------- DTCM BEGIN ----------------------------------------*/
  _sidata = LOADADDR(.data);
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    . = ALIGN(4);
  } >DTCMRAM AT> FLASH

  .tdata : ALIGN(4)
  {
    *(.tdata .tdata.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >DTCMRAM AT> FLASH

  .tbss (NOLOAD) : ALIGN(4)
  {
    _sbss = .;
    *(.tbss .tbss.*)
    . = ALIGN(4);
  } >DTCMRAM

  .bss (NOLOAD) : ALIGN(4)
  {
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } >DTCMRAM
/*---------------------------------------- DTCM END ----------------------------------------*/

/*---------------------------------------- RAM1 BEGIN ----------------------------------------*/
  _sidata_ram1 = LOADADDR(.data_ram1);
  .data_ram1 :
  {
    . = ALIGN(4);
    _sdata_ram1 = .;
    *(.data_ram1)
    *(.data_ram1*)
    . = ALIGN(4);
  } >RAM_D1 AT> FLASH

  .tdata_ram1 : ALIGN(4)
  {
    *(.tdata_ram1 .tdata_ram1.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _edata_ram1 = .;
  } >RAM_D1 AT> FLASH

  .tbss_ram1 (NOLOAD) : ALIGN(4)
  {
    _sbss_ram1 = .;
    *(.tbss_ram1 .tbss_ram1.*)
    . = ALIGN(4);
  } >RAM_D1

  .bss_ram1 (NOLOAD) : ALIGN(4)
  {
    *(.bss_ram1)
    *(.bss_ram1*)
    . = ALIGN(4);
    _ebss_ram1 = .;
  } >RAM_D1
/*---------------------------------------- RAM1 END ----------------------------------------*/

/*---------------------------------------- RAM2 BEGIN ----------------------------------------*/
  _sidata_ram2 = LOADADDR(.data_ram2);
  .data_ram2 :
  {
    . = ALIGN(4);
    _sdata_ram2 = .;
    *(.data_ram2)
    *(.data_ram2*)
    . = ALIGN(4);
  } >RAM_D2 AT> FLASH

  .tdata_ram2 : ALIGN(4)
  {
    *(.tdata_ram2 .tdata_ram2.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _edata_ram2 = .;
  } >RAM_D2 AT> FLASH

  .tbss_ram2 (NOLOAD) : ALIGN(4)
  {
    _sbss_ram2 = .;
    *(.tbss_ram2 .tbss_ram2.*)
    . = ALIGN(4);
  } >RAM_D2

  .bss_ram2 (NOLOAD) : ALIGN(4)
  {
    *(.bss_ram2)
    *(.bss_ram2*)
    . = ALIGN(4);
    _ebss_ram2 = .;
  } >RAM_D2
/*---------------------------------------- RAM2 END ----------------------------------------*/

/*---------------------------------------- RAM3 BEGIN ----------------------------------------*/
  _sidata_ram3 = LOADADDR(.data_ram3);
  .data_ram3 :
  {
    . = ALIGN(4);
    _sdata_ram3 = .;
    *(.data_ram3)
    *(.data_ram3*)
    . = ALIGN(4);
  } >RAM_D3 AT> FLASH

  .tdata_ram3 : ALIGN(4)
  {
    *(.tdata_ram3 .tdata_ram3.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _edata_ram3 = .;
  } >RAM_D3 AT> FLASH

  .tbss_ram3 (NOLOAD) : ALIGN(4)
  {
    _sbss_ram3 = .;
    *(.tbss_ram3 .tbss_ram3.*)
    . = ALIGN(4);
  } >RAM_D3

  .bss_ram3 (NOLOAD) : ALIGN(4)
  {
    *(.bss_ram3)
    *(.bss_ram3*)
    . = ALIGN(4);
    _ebss_ram3 = .;
  } >RAM_D3
/*---------------------------------------- RAM3 END ----------------------------------------*/

/*---------------------------------------- backup BEGIN ----------------------------------------*/
  .data_backup :
  {
    . = ALIGN(4);
    *(.data_backup)
    *(.data_backup*)
    . = ALIGN(4);
  } >RAM_BackUp
/*---------------------------------------- backup END ----------------------------------------*/

/*---------------------------------------- heap BEGIN ----------------------------------------*/
  ._user_heap_stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >DTCMRAM
/*---------------------------------------- heap END ----------------------------------------*/
/*************************************************** DATA END ***************************************************/

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a:* ( * )
    libm.a:* ( * )
    libgcc.a:* ( * )
  }

}