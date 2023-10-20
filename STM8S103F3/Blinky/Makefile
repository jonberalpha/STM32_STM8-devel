all:
	sdcc *.c -lstm8 -mstm8 --out-fmt-ihx -I. -I../stm8s_header_sdcc -I../stm8s_header_sdcc/inc -oBLINK.ihx

flash:
	stm8flash -c stlinkv2 -p stm8s103f3 -w BLINK.ihx

clean:
	rm -rf *.asm *.cdb *.ihx *.lk *.lst *.map *.rel *.rst *.sym
