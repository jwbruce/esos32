/*
 * "Copyright (c) 2019 J. W. Bruce ("AUTHOR(S)")"
 * All rights reserved.
 * (J. W. Bruce, jwbruce_AT_tntech.edu, Tennessee Tech University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */


/**
 * \addtogroup ESOS_UART_Service
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/**
* /** \file
* \brief PC (linux) code to emulating the ESOS32 communications service.
* Input comes from keyboard, and output is directed to the "terminal".
* It would not be *too hard* to modify to actually use a serial port.
* 
* \note The windows version has not been tested in many years.
*/

/*** I N C L U D E S ****
*********************************************/
#include "esos_pc_stdio.h"
#ifdef USE_NCURSES
#include <ncurses.h>
#else
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#endif



//#define USE_NCURSES

/*** G L O B A L S *************************************************/
struct termios stored_settings;

/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/
void    __esos_hw_signal_start_tx(void) {
  while (__st_TxBuffer.u16_Head != __st_TxBuffer.u16_Tail) {
    __st_TxBuffer.u16_Tail++;      //increment tail pointer
    if (__st_TxBuffer.u16_Tail == ESOS_SERIAL_IN_EP_SIZE)
      __st_TxBuffer.u16_Tail = 0;  //wrap if needed
    //transfer character from software buffer to transmit buffer
#ifdef USE_NCURSES
    waddch( __st_TxBuffer.pau8_Data[__st_TxBuffer.u16_Tail] );
#else
    printf("%c", __st_TxBuffer.pau8_Data[__st_TxBuffer.u16_Tail]);
    // make the stdout do its thing right away
    fflush(stdout);
#endif
  }
}

void    __esos_hw_signal_stop_tx(void) {

}

ESOS_USER_TASK( __Linux_check_keyboard ) {
  static uint8_t      u8_c;

  ESOS_TASK_BEGIN();
  set_keypress();
  while (TRUE) {
    ESOS_TASK_WAIT_UNTIL(  kbhit() );
    u8_c = getchar();             //read character
    __st_RxBuffer.u16_Head++;     //increment head pointer
    if (__st_RxBuffer.u16_Head == ESOS_SERIAL_OUT_EP_SIZE)
      __st_RxBuffer.u16_Head = 0; //wrap if needed

    __st_RxBuffer.pau8_Data[__st_RxBuffer.u16_Head] = u8_c;   //place in buffer
  } // endof while(TRUE)
  ESOS_TASK_END();
} // endof TASK


/*
** POSIX systems do NOT have a kbhit() function in their stdio
** libraries.  So we will write our own here.  This function
** returns TRUE when there is some keypress, else FALSE.  So,
** we can poll or wait on kbhit().  When it is true, we can
** call the normal getch()
*/
uint8_t kbhit(void) {
  struct timeval tv;
  fd_set read_fd;

  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&read_fd);
  FD_SET(0,&read_fd);

  if (select(1, &read_fd, NULL, NULL, &tv) == -1)
    return FALSE;

  if (FD_ISSET(0,&read_fd))
    return TRUE;

  return FALSE;
} //end kbhit())


void set_keypress(void) {
  struct termios new_settings;

  tcgetattr(0,&stored_settings);
  new_settings = stored_settings;

  /* Disable canonical mode, set buffer size to 1 byte, turn OFF echo */
  new_settings.c_lflag &= (~ICANON);
  new_settings.c_cc[VTIME] = 0;
  new_settings.c_cc[VMIN] = 1;
  new_settings.c_lflag &= (~ECHO);
  tcsetattr(0,TCSANOW,&new_settings);
  return;
}

void reset_keypress(void) {
  tcsetattr(0,TCSANOW,&stored_settings);
  return;
}



/* ########################################################################### */


/******************************************************************************
 * Function:        void _esos_hw_InitSerialUart( void )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          ptr to ESOS_COMM_BUFF_DSC structure with initialized ptrs
 *
 * Side Effects:    Turns on USART hardware
 *
 *****************************************************************************/
void __esos_hw_InitCommSystem(void) {
  // Init the NCURSES system instead of the serial port....
  uint8_t       u8_char;

#if 0
  initscr();                      /* Start curses mode            */
  raw();                          /* Line buffering disabled      */
  keypad(stdscr, TRUE);           /* We get F1, F2 etc..          */
  noecho();                       /* Don't echo() while we do getch */

  printw("Hello from ESOS -- the PC/Linux version w/ NCURSES\n");
  refresh();                      /* Print it on to the real screen */
  getch();                        /* Wait for user input */
#else
  printf ("Hello from ESOS -- the PC/Linux version\n");
#endif

  // Since the PC does not support background interrupts (easily),
  // we will just mimic that by creating an ESOS task to read the
  // from the keyboard.  The transmits will be done with printf
  // and occur immediately.
  //
  // NOTE: When this task runs, the KDE debugger (kdbg) does not
  //     run very well.  When stepping, try to structure the application
  //     to run without input from stdin and COMMENT OUT THE FOLLOWING LINE!
  esos_RegisterTask( __Linux_check_keyboard );

}  // end __esos_hw_InitCommSystem()


/******************************************************************************
 * Function:        uint8_t esos_GetCommSystemMaxInDataLen(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          the maximum number of uint8_ts that the comm system will
 *                  receive in a single buffer transfer from the host -- OR --
 *          in the case of single uint8_t xfers (like RS232), the maximum
 *          number of uint8_ts that can be RX-ed before the buffers
 *          overflow
 *
 * Side Effects:    None
 *
 * Overview:        A way for a run-time determination of the maximum buffer
 *                  size that the user can can expect.  This number is
 *                  actually hard-coded in the USB CDC header file, but this
 *                  method will allow the user code to be more generic, if
 *                  it chooses to be.
 *
 *****************************************************************************/
uint8_t esos_GetCommSystemMaxInDataLen(void) {
  return ESOS_SERIAL_OUT_EP_SIZE;
} //end esos_GetCommSystemMaxInDataLen()

/******************************************************************************
 * Function:        uint8_t esos_GetCommSystemMaxOutDataLen(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          the maximum number of uint8_ts that the comm system will
 *                  transfer back to the host in a single buffer  -- OR --
 *          in the case of singe uint8_t xfers (like RS232), the maximum
 *          number of uint8_ts in the output buffer before overflow
 *
 * Side Effects:    None
 *
 * Overview:        A way for a run-time determination of the maximum buffer
 *                  size that the user can can send efficiently.  The USB system
 *                  will send a bigger buffer than getUSBCdcTxMax() size, but
 *                  will do so in several smaller getUSBCdcTxMax()-sized chunks.
 *
 *                  This number is actually hard-coded in the USB CDC header file,
 *                  but this method will allow the user code to be more generic,
 *                  if it chooses to be.
 *
 *****************************************************************************/
uint8_t esos_GetCommSystemMaxOutDataLen(void) {
  return  ESOS_SERIAL_IN_EP_SIZE;
} //end esos_GetCommSystemMaxOutDataLen()

/******************************************************************************
 * Function:        uint8_t _esos_hw_GetUartVersion(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          Return the version number of the MSU Bulk CDC driver firmware
 *                  currently running.
 *                  The most-significant bit denotes we're running USB
 *                  The most-significant nibble is the major revision number
 *                  The least-significant nibble is the minor revision number
 *
 * Side Effects:    None
 *
 *****************************************************************************/
uint8_t _esos_hw_GetSerialUartVersion(void) {
  return  ESOS_COMM_SYS_SERIAL_REV;
} //end _esos_hw_GetUartVersion()
