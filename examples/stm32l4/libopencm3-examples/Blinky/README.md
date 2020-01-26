This code does not use the ESOS32 system in any regard. Additionally, the makefile generates ASM files with .S extensions.

**Steps to running blinky_blink**
------------------------------------
    -Within the Makefile, change directories to match the locations to which your directories are
    -Open a cmd prompt within the folder with this makefile
    -Run mingw32-make to generate hex file
    -Use STMCubeProgrammer to upload hex file to board

**Note: this example can serve as a foundation for labs. The only modifications that need to be changed are as follows**
    -Change the target to match that of your file with your main loop
    -In C-source, add all files that are found in you ./src/ folder 
    -When ESOS is added in, add the ESOS files to the C-source section of the makefile in the format:
     (ESOS_DIR)/src/blahblahblah.c which needs to be added for each of the esos src files.
    -You will need to add a ESOS_DIR that points to the esos directory to the top of the makefile
     /**You will need to add the include and lib as well, in the C-include section. Follow the CM3 setup, or ESOSCM3 makefiles in armdev**/
     
**These makefiles will not compile in msys. Msys is necessary to compile anything with libopencm. Use mingw32-make for all other cases that do not involve a libopencm3-made makefile**


     