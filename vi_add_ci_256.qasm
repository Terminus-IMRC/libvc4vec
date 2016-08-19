; nloops
alu.or, r0, uniform_read, #0
; first VPM read addr
alu.or, r1, uniform_read, #0
; first VPM write addr
alu.or, r2, uniform_read, #0
; the constant to add
alu.or, rb0, uniform_read, #0

; VPM addr step (256 * (32 / 8) = 1024)
li32, r3, #1024

li32, vpmvcd_rd_setup, #0b 1 000 0011 0000 0000 0001 0 0000000 0000

;                             |the number of vectors to write
;                             |       |width of each vector (do not change!)
li32, vpmvcd_wr_setup, #0b 10 0010000 0010000 0 1 00000000000 000

:loop
	alu.or, vpm_ld_addr, r1, #0
	li32, vpmvcd_wr_setup, #0b 00 000000000000 000001 1 0 10 00000000
	alu.or, nop, vpm_ld_wait, nop
	li32, vpmvcd_rd_setup, #0b 00 000000 0000 00 000001 1 0 10 00000000

	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0
	alu.add, vpm_write, vpm_read, rb0

	alu.or, vpm_st_addr, r2, #0

	alu.add.sf, r0, r0, #-1
	bra.allzc, , nop, nop, :loop
	alu.add, r1, r1, r3
	alu.add, r2, r2, r3
	alu.or, nop, nop, vpm_st_wait

alu.program_end.nop.never, nop, nop, nop
alu.nop.never, nop, nop, nop
alu.nop.never, nop, nop, nop
