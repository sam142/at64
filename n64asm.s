#include <avr/io.h>

; Call-used registers (r18-r27, r30-r31):
; May be allocated by gcc for local data. You may use them freely in assembler subroutines. Calling C subroutines can clobber any of them - the caller is responsible for saving and restoring.

; Call-saved registers (r2-r17, r28-r29):
; May be allocated by gcc for local data. Calling C subroutines leaves them unchanged. Assembler subroutines are responsible for saving and restoring these registers, if changed. r29:r28 (Y pointer) is used as a frame pointer (points to local data on stack) if necessary.
; The requirement for the callee to save/preserve the contents of these registers even applies in situations where the compiler assigns them for argument passing.


ZERO = 1 ;register r1 contains always 0

.macro NOPS X
  .rept \X
    nop
  .endr
.endm

.section .text


; r20: set if receive completed (timed out)
.global TIMER0_OVF_vect
TIMER0_OVF_vect:

	;data receive timed out
	ldi r20, 1
	reti


; uint8_t<r24> ntd_request(uint8_t *request<r25:r24>, uint8_t request_length<r22>, uint8_t response[128]<r21:r20>, uint8_t portc_mask<r18>);
; register usage:
; r25
; r24
; r22
; r21
; r20
; r18

.global ntd_request
ntd_request:

	;disable interrupts
	cli

	;r16: portc_mask
	push r16
	mov r16, r18

	;portc = 0
	out _SFR_IO_ADDR(PORTC), ZERO

	push r20
	push r21

	;r20: portc_mask
	mov r20, r16
	call ntd_send

	;enable interrupts
	sei

	pop r25
	pop r24

	;r20: portc_mask
	mov r22, r16
	call ntd_receive

	pop r16
	ret


; uint8_t<r24> ntd_receive(uint8_t response[128]<r25:r24>, uint8_t portc_mask<r22>);
; register usage:
; r25
; r24
; X
ntd_receive:

	;X = response
	mov XL, r24
	mov XH, r25

	com r22

	;use portc as an input
	in r18, _SFR_IO_ADDR(DDRC)
	and r18, r22
	out _SFR_IO_ADDR(DDRC), r18

	com r22

	;clear timer0
	out _SFR_IO_ADDR(TCNT0), ZERO
	clr r20

	;enable timer0 overflow interrupt
	ldi r18, (1<<TOIE0)
	sts _SFR_MEM_ADDR(TIMSK0), r18

	;start timer0 (no prescaling)
	ldi r18, (1<<CS00)
	out _SFR_IO_ADDR(TCCR0B), r18

  ;wait for falling edge
  wait_ntd_low:	
	in r18, _SFR_IO_ADDR(PINC)
	and r18, r22
	breq ntd_low
	cpi r20, 1
	brne wait_ntd_low

  ;timeout: receive completed
  timeout:
	;stop timer, disable interrupt
	out _SFR_IO_ADDR(TCCR0B), ZERO
	sts _SFR_MEM_ADDR(TIMSK0), ZERO
	
	;calculate length
    ;<XH, XL> = <XH, XL> - <r25, r24>
    sub XL, r24
    sbc XH, r25

	;<r25, r24> = <XH, XL>
	mov r24, XL
	mov r25, XH

	ret


  ;falling edge occurred
  ntd_low:
    ;reset timer
	out _SFR_IO_ADDR(TCNT0), ZERO

  ;wait for rising edge
  wait_ntd_high:
	cpi r20, 1
	breq timeout
	in r18, _SFR_IO_ADDR(PINC)
	and r18, r22
	breq wait_ntd_high

	;rising edge occurred
	;store timer
	in r18, _SFR_IO_ADDR(TCNT0)
	st X+, r18

	rjmp wait_ntd_low

	


; void ntd_send(uint8_t *request<r25:r24>, uint8_t request_length<r22>, uint8_t portc_mask<r20>);
; send buffer at r25:r24 with length r22 to controller
; register usage:
; r25
; r24
; r22
; Z

ntd_send:

	mov ZL, r24
	mov ZH, r25
	mov r23, r20

  send_byte_loop:

	;r20; portc_mask
	mov r20, r23
	;r24: byte
	ld r24, Z+
	call ntd_send_byte

	dec r22

	brne send_byte_loop

	;send stop bit
	;low
	NOPS 7

	out _SFR_IO_ADDR(DDRC), r21

	;low for 1us
	NOPS 16

	;high
	out _SFR_IO_ADDR(DDRC), r20

	ret

; void ntd_send_byte(uint8_t byte<r24>, uint8_t portc_mask<r20>);
; send r24 to controller
; register usage:
; r24
; r20
; r19
; r18

ntd_send_byte:

	;r21 DDRC high -> ntd low
	in r21, _SFR_IO_ADDR(DDRC)
	or r21, r20

	;r20 DDRC low -> ntd high
	com r20
	in r18, _SFR_IO_ADDR(DDRC)
	and r20, r18

	ldi r18, (1<<7)

  send_loop:

	;low
	out _SFR_IO_ADDR(DDRC), r21

	mov r19, r24
  	and r19, r18

	breq send_zero

	  send_one:

	  	;low for 1us
		NOPS 13

		;high
		out _SFR_IO_ADDR(DDRC), r20

		;high for 3us
		NOPS 30

		rjmp send_continue_loop

	  send_zero:

	    ;low for 3us
		NOPS 44

		;high
		out _SFR_IO_ADDR(DDRC), r20
	
	  send_continue_loop:
	lsr r18
	brcs send_done

	;high for 1us
	NOPS 11

	jmp send_loop

  send_done:

	ret
