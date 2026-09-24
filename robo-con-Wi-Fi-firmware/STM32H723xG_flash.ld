/*
******************************************************************************
**

**  File        : LinkerScript.ld
**
**  Author		: STM32CubeMX
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

/*RAMd1_Heap_Size = 0x8000;  /**/
/*RAMd1_Stack_Size = 0x4000; /**/
RAMd1_Heap_Size = 0x0000;/**/
RAMd1_Stack_Size = 0x000;/**/

/*RAMd2_Heap_Size = 0x400; /**/
/*RAMd2_Stack_Size = 0x400;/**/
RAMd2_Heap_Size = 0x0000;  /**/
RAMd2_Stack_Size = 0x000;  /**/

/*RAMd3_Heap_Size = 0x400; /**/
/*RAMd3_Stack_Size = 0x400;/**/
RAMd3_Heap_Size = 0x0000;  /**/
RAMd3_Stack_Size = 0x000;  /**/

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH

  _siitcm = LOADADDR(.itcm_text);

  /* ITCM */
  .itcm_text : 
  {
    . = ALIGN(4);
    _sitcm = .;
    *(.itcm_text)
    *(.itcm_text*)
    
    . = ALIGN(4);
  } > ITCMRAM AT>FLASH

  .titcm : ALIGN(4)
  {
    *(titcm .titcm.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _eitcm = .;
    PROVIDE(__itcm_end = .);
    PROVIDE(__titcm_end = .);
  } > ITCMRAM AT>FLASH

  PROVIDE( __titcm_start = ADDR(.titcm) );
  PROVIDE( __titcm_size = __titcm_end - __titcm_start );

  PROVIDE( __itcm_start = ADDR(.itcm_text) );
  PROVIDE( __itcm_size = __itcm_end - __itcm_start );

  PROVIDE( __titcm_source = LOADADDR(.titcm) );
  PROVIDE( __titcm_source_end = LOADADDR(.titcm) + SIZEOF(.titcm) );
  PROVIDE( __titcm_source_size = __titcm_source_end - __titcm_source );

  PROVIDE( __itcm_source = LOADADDR(.itcm_text) );
  PROVIDE( __itcm_source_end = __tdata_RAM1_source_end );
  PROVIDE( __itcm_source_size = __itcm_source_end - __itcm_source );

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH

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

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);
  _sidata_RAM1 = LOADADDR(.ram_d1_data);
  _sidata_RAM2 = LOADADDR(.ram_d2_data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    . = ALIGN(4);
  } >DTCMRAM AT> FLASH

 /* Initialized TLS data section */
  .tdata : ALIGN(4)
  {
    *(.tdata .tdata.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
    PROVIDE(__data_end = .);
    PROVIDE(__tdata_end = .);
  } >DTCMRAM AT> FLASH

  PROVIDE( __tdata_start = ADDR(.tdata) );
  PROVIDE( __tdata_size = __tdata_end - __tdata_start );

  PROVIDE( __data_start = ADDR(.data) );
  PROVIDE( __data_size = __data_end - __data_start );

  PROVIDE( __tdata_source = LOADADDR(.tdata) );
  PROVIDE( __tdata_source_end = LOADADDR(.tdata) + SIZEOF(.tdata) );
  PROVIDE( __tdata_source_size = __tdata_source_end - __tdata_source );

  PROVIDE( __data_source = LOADADDR(.data) );
  PROVIDE( __data_source_end = __tdata_source_end );
  PROVIDE( __data_source_size = __data_source_end - __data_source );
  /* Uninitialized data section */
  .tbss (NOLOAD) : ALIGN(4)
  {
     /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.tbss .tbss.*)
    . = ALIGN(4);
    PROVIDE( __tbss_end = . );
  } >DTCMRAM

  PROVIDE( __tbss_start = ADDR(.tbss) );
  PROVIDE( __tbss_size = __tbss_end - __tbss_start );
  PROVIDE( __tbss_offset = ADDR(.tbss) - ADDR(.tdata) );

  PROVIDE( __tls_base = __tdata_start );
  PROVIDE( __tls_end = __tbss_end );
  PROVIDE( __tls_size = __tls_end - __tls_base );
  PROVIDE( __tls_align = MAX(ALIGNOF(.tdata), ALIGNOF(.tbss)) );
  PROVIDE( __tls_size_align = (__tls_size + __tls_align - 1) & ~(__tls_align - 1) );
  PROVIDE( __arm32_tls_tcb_offset = MAX(8, __tls_align) );
  PROVIDE( __arm64_tls_tcb_offset = MAX(16, __tls_align) );

  .bss (NOLOAD) : ALIGN(4)
  {
    *(.bss)
    *(.bss*)
    *(COMMON)

      . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
      PROVIDE( __bss_end = .);
  } >DTCMRAM
  PROVIDE( __non_tls_bss_start = ADDR(.bss) );

  PROVIDE( __bss_start = __tbss_start );
  PROVIDE( __bss_size = __bss_end - __bss_start );


  /* RAM_D1_data */
  .ram_d1_data :
  {
    . = ALIGN(4);
    _sdata_RAM1 = .;        /* create a global symbol at data start */
    *(.ram_d1_data)
    *(.ram_d1_data*)
    
    . = ALIGN(4);
  } > RAM_D1 AT>FLASH
  

/* Initialized TLS data section */
  .tdata_RAM1 : ALIGN(4)
  {
    *(.tdata_RAM1 .tdata_RAM1.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _edata_RAM1 = .;        /* define a global symbol at data end */
    PROVIDE(__data_RAM1_end = .);
    PROVIDE(__tdata_RAM1_end = .);
  } >RAM_D1 AT> FLASH

  PROVIDE( __tdata_RAM1_start = ADDR(.tdata_RAM1) );
  PROVIDE( __tdata_RAM1_size = __tdata_RAM1_end - __tdata_RAM1_start );

  PROVIDE( __data_RAM1_start = ADDR(.ram_d1_data) );
  PROVIDE( __data_RAM1_size = __data_RAM1_end - __data_RAM1_start );

  PROVIDE( __tdata_RAM1_source = LOADADDR(.tdata_RAM1) );
  PROVIDE( __tdata_RAM1_source_end = LOADADDR(.tdata_RAM1) + SIZEOF(.tdata_RAM1) );
  PROVIDE( __tdata_RAM1_source_size = __tdata_RAM1_source_end - __tdata_RAM1_source );

  PROVIDE( __data_RAM1_source = LOADADDR(.ram_d1_data) );
  PROVIDE( __data_RAM1_source_end = __tdata_RAM1_source_end );
  PROVIDE( __data_RAM1_source_size = __data_RAM1_source_end - __data_RAM1_source );
  /* Uninitialized data section */
  .tbss_RAM1 (NOLOAD) : ALIGN(4)
  {
     /* This is used by the startup in order to initialize the .bss secion */
    _sbss_RAM1 = .;         /* define a global symbol at bss start */
    __bss_RAM1_start__ = _sbss_RAM1;
    *(.tbss_RAM1 .tbss_RAM1.*)
    . = ALIGN(4);
    PROVIDE( __tbss_RAM1_end = . );
  } >RAM_D1

  PROVIDE( __tbss_RAM1_start = ADDR(.tbss_RAM1) );
  PROVIDE( __tbss_RAM1_size = __tbss_RAM1_end - __tbss_RAM1_start );
  PROVIDE( __tbss_RAM1_offset = ADDR(.tbss_RAM1) - ADDR(.tdata_RAM1) );

  PROVIDE( __tls_RAM1_base = __tdata_RAM1_start );
  PROVIDE( __tls_RAM1_end = __tbss_RAM1_end );
  PROVIDE( __tls_RAM1_size = __tls_RAM1_end - __tls_RAM1_base );
  PROVIDE( __tls_RAM1_align = MAX(ALIGNOF(.tdata_RAM1), ALIGNOF(.tbss_RAM1)) );
  PROVIDE( __tls_RAM1_size_align = (__tls_RAM1_size + __tls_RAM1_align - 1) & ~(__tls_RAM1_align - 1) );
  PROVIDE( __arm32_tls_RAM1_tcb_offset = MAX(8, __tls_RAM1_align) );
  PROVIDE( __arm64_tls_RAM1_tcb_offset = MAX(16, __tls_RAM1_align) );

  .bss_RAM1 (NOLOAD) : ALIGN(4)
  {
    *(.bss_RAM1)
    *(.bss_RAM1*)
    *(COMMON)

      . = ALIGN(4);
    _ebss_RAM1 = .;         /* define a global symbol at bss end */
    __bss_RAM1_end__ = _ebss_RAM1;
      PROVIDE( __bss_RAM1_end = .);
  } >RAM_D1
  PROVIDE( __non_tls_RAM1_bss_start = ADDR(.bss_RAM1) );

  PROVIDE( __bss_RAM1_start = __tbss_RAM1_start );
  PROVIDE( __bss_RAM1_size = __bss_RAM1_end - __bss_RAM1_start );

  /* RAM_D2_data */
  .ram_d2_data :
  {
    . = ALIGN(4);
    _sdata_RAM2 = .;        /* create a global symbol at data start */
    *(.ram_d2_data)
    *(.ram_d2_data*)

    . = ALIGN(4);
  } > RAM_D2 AT>FLASH
  
  
/* Initialized TLS data section */
  .tdata_RAM2 : ALIGN(4)
  {
    *(.tdata_RAM2 .tdata_RAM2.* .gnu.linkonce.td.*)
    . = ALIGN(4);
    _edata_RAM2 = .;        /* define a global symbol at data end */
    PROVIDE(__data_RAM2_end = .);
    PROVIDE(__tdata_RAM2_end = .);
  } >RAM_D2 AT> FLASH

  PROVIDE( __tdata_RAM2_start = ADDR(.tdata_RAM2) );
  PROVIDE( __tdata_RAM2_size = __tdata_RAM2_end - __tdata_RAM2_start );

  PROVIDE( __data_RAM2_start = ADDR(.ram_d2_data) );
  PROVIDE( __data_RAM2_size = __tdata_RAM2_end - __data_RAM2_start );

  PROVIDE( __tdata_RAM2_source = LOADADDR(.tdata_RAM2) );
  PROVIDE( __tdata_RAM2_source_end = LOADADDR(.tdata_RAM2) + SIZEOF(.tdata_RAM2) );
  PROVIDE( __tdata_RAM2_source_size = __tdata_RAM2_source_end - __tdata_RAM2_source );

  PROVIDE( __data_RAM2_source = LOADADDR(.ram_d2_data) );
  PROVIDE( __data_RAM2_source_end = __tdata_RAM2_source_end );
  PROVIDE( __data_RAM2_source_size = __data_RAM2_source_end - __data_RAM2_source );
  /* Uninitialized data section */
  .tbss_RAM2 (NOLOAD) : ALIGN(4)
  {
     /* This is used by the startup in order to initialize the .bss secion */
    _sbss_RAM2 = .;         /* define a global symbol at bss start */
    __bss_RAM2_start__ = _sbss_RAM2;
    *(.tbss_RAM2 .tbss_RAM2.*)
    . = ALIGN(4);
    PROVIDE( __tbss_RAM2_end = . );
  } >RAM_D2

  PROVIDE( __tbss_RAM2_start = ADDR(.tbss_RAM2) );
  PROVIDE( __tbss_RAM2_size = __tbss_RAM2_end - __tbss_RAM2_start );
  PROVIDE( __tbss_RAM2_offset = ADDR(.tbss_RAM2) - ADDR(.tdata_RAM2) );

  PROVIDE( __tls_RAM2_base = __tdata_RAM2_start );
  PROVIDE( __tls_RAM2_end = __tbss_RAM2_end );
  PROVIDE( __tls_RAM2_size = __tls_RAM2_end - __tls_RAM2_base );
  PROVIDE( __tls_RAM2_align = MAX(ALIGNOF(.tdata_RAM2), ALIGNOF(.tbss_RAM2)) );
  PROVIDE( __tls_RAM2_size_align = (__tls_RAM2_size + __tls_RAM2_align - 1) & ~(__tls_RAM2_align - 1) );
  PROVIDE( __arm32_tls_RAM2_tcb_offset = MAX(8, __tls_RAM2_align) );
  PROVIDE( __arm64_tls_RAM2_tcb_offset = MAX(16, __tls_RAM2_align) );

  .bss_RAM2 (NOLOAD) : ALIGN(4)
  {
    *(.bss_RAM2)
    *(.bss_RAM2*)
    *(COMMON)

      . = ALIGN(4);
    _ebss_RAM2 = .;         /* define a global symbol at bss end */
    __bss_RAM2_end__ = _ebss_RAM2;
      PROVIDE( __bss_RAM2_end = .);
  } >RAM_D2
  PROVIDE( __non_tls_RAM2_bss_start = ADDR(.bss_RAM2) );

  PROVIDE( __bss_RAM2_start = __tbss_RAM2_start );
  PROVIDE( __bss_RAM2_size = __bss_RAM2_end - __bss_RAM2_start );


  /* RAM_D3_data */
  .ram_d3_data :
  {
    . = ALIGN(4);
    *(.ram_d3_data)
    *(.ram_d3_data*)
    . = ALIGN(4);
  } > RAM_D3 AT>FLASH
  
  /* RAM_D3_data */
  .ram_BackUp_data :
  {
    . = ALIGN(4);
    *(.ram_BackUp_data)
    *(.ram_BackUp_data*)
    . = ALIGN(4);
  } > RAM_BackUp AT>FLASH

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >DTCMRAM

  /* Heap */
  ._RAM_D1_heap_stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE ( RAMd1_end = .);
    PROVIDE ( RAMd1__end = .);
    . = . + RAMd1_Heap_Size;
    . = . + RAMd1_Stack_Size;
    . = ALIGN(8);
  } >RAM_D1
  
  ._RAM_D2_heap_stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE ( RAMd2_end = .);
    PROVIDE ( RAMd2__end = .);
    . = . + RAMd2_Heap_Size;
    . = . + RAMd2_Stack_Size;
    . = ALIGN(8);
  } >RAM_D2
  
  ._RAM_D3_heap_stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE ( RAMd3_end = .);
    PROVIDE ( RAMd3__end = .);
    . = . + RAMd3_Heap_Size;
    . = . + RAMd3_Stack_Size;
    . = ALIGN(8);
  } >RAM_D3



  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a:* ( * )
    libm.a:* ( * )
    libgcc.a:* ( * )
  }

}
