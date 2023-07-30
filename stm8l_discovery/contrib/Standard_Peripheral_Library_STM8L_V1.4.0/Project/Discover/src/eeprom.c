/*
 * Copyright 2010 IAR Systems AB.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "stm8l15x_flash.h"

/*
 * The user must implement the three extern-declared functions below
 * in order for the compiler to be able to automatically write to the
 * EEPROM memory when __eeprom variables are assigned to.
 */


/*
 * Wait for the last data EEPROM operation to finish.  Return 0 if the
 * operation failed, otherwise non-zero.  You may want to handle
 * errors here, since the utility functions below simply ignore
 * errors, aborting multi-write operations early.
 */
int __eeprom_wait_for_last_operation(void)
{
  FLASH_Status_TypeDef status = FLASH_WaitForLastOperation(FLASH_MemType_Data);
  return !!(status & (  FLASH_Status_Successful_Operation));
}

/*
 * Write one byte to the data EEPROM memory.
 */
void __eeprom_program_byte(unsigned char __near * dst, unsigned char v)
{
  FLASH_ProgramByte((u32)dst, (u8)v);
}

/*
 * Write one 4-byte long word to the data EEPROM memory.  The address
 * must be 4-byte aligned.
 */
void __eeprom_program_long(unsigned char __near * dst, unsigned long v)
{
  FLASH_ProgramWord((u32)dst, (u32)v);
}
