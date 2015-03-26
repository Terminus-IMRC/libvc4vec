; nloops
alu.or, r0, uniform_read, #0
; first VPM read addr
alu.or, r1, uniform_read, #0
; first VPM write addr
alu.or, r2, uniform_read, #0

; VPM addr step (256 * (32 / 8) = 1024)
li32, r3, #1024

li32, vpmvcd_rd_setup, #0b 1 000 0011 0000 0000 0001 0 0000000 0000

:loop
	alu.or, vpm_ld_addr, r1, #0
	alu.or, nop, vpm_ld_wait, nop
	li32, vpmvcd_rd_setup, #0b 00 000000 0000 00 000001 1 0 10 00000000

	alu.or, ra0, vpm_read, #0
	alu.or, ra1, vpm_read, #0
	alu.or, ra2, vpm_read, #0
	alu.or, ra3, vpm_read, #0
	alu.or, ra4, vpm_read, #0
	alu.or, ra5, vpm_read, #0
	alu.or, ra6, vpm_read, #0
	alu.or, ra7, vpm_read, #0
	alu.or, ra8, vpm_read, #0
	alu.or, ra9, vpm_read, #0
	alu.or, ra10, vpm_read, #0
	alu.or, ra11, vpm_read, #0
	alu.or, ra12, vpm_read, #0
	alu.or, ra13, vpm_read, #0
	alu.or, ra14, vpm_read, #0
	alu.or, ra15, vpm_read, #0

	li32, vpmvcd_wr_setup, #0b 00 000000000000 000001 1 0 10 00000000
	alu.add, vpm_write, ra0, #1
	alu.add, vpm_write, ra1, #1
	alu.add, vpm_write, ra2, #1
	alu.add, vpm_write, ra3, #1
	alu.add, vpm_write, ra4, #1
	alu.add, vpm_write, ra5, #1
	alu.add, vpm_write, ra6, #1
	alu.add, vpm_write, ra7, #1
	alu.add, vpm_write, ra8, #1
	alu.add, vpm_write, ra9, #1
	alu.add, vpm_write, ra10, #1
	alu.add, vpm_write, ra11, #1
	alu.add, vpm_write, ra12, #1
	alu.add, vpm_write, ra13, #1
	alu.add, vpm_write, ra14, #1
	alu.add, vpm_write, ra15, #1

	;                             |the number of vectors to write
	;                             |       |width of each vector (do not change!)
	li32, vpmvcd_wr_setup, #0b 10 0010000 0010000 0 1 00000000000 000
	alu.or, vpm_st_addr, r2, #0

	alu.add.sf, nop, -, #1
	alu.add.sf, r0, r0, #-1
	bra.allzc, , nop, nop, :loop
	alu.or, nop, nop, vpm_st_wait
	alu.add, r1, r1, r3
	alu.add, r2, r2, r3

alu.program_end.nop.never, nop, nop, nop
alu.nop.never, nop, nop, nop
alu.nop.never, nop, nop, nop
alu.nop.never, nop, nop, nop
