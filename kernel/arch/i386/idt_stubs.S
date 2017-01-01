.section .text
.extern isr_handler
.extern irq_handler
.extern trap_handler
.globl isr0
.globl isr1
.globl isr2
.globl isr3
.globl isr4
.globl isr5
.globl isr6
.globl isr7
.globl isr8
.globl isr9
.globl isr10
.globl isr11
.globl isr12
.globl isr13
.globl isr14
.globl isr15
.globl isr16
.globl isr17
.globl isr18
.globl isr19
.globl isr20
.globl isr21
.globl isr22
.globl isr23
.globl isr24
.globl isr25
.globl isr26
.globl isr27
.globl isr28
.globl isr29
.globl isr30
.globl isr31

.globl irq0
.globl irq1
.globl irq2
.globl irq3
.globl irq4
.globl irq5
.globl irq6
.globl irq7
.globl irq8
.globl irq9
.globl irq10
.globl irq11
.globl irq12
.globl irq13
.globl irq14
.globl irq15


isr0:
    push $0
    push $0
    jmp common_isr
isr1:
    push $0
    push $1
    jmp common_isr
isr2:
    push $0
    push $2
    jmp common_isr
isr3:
    push $0
    push $3
    jmp common_isr
isr4:
    push $0
    push $4
    jmp common_isr
isr5:
    push $0
    push $5
    jmp common_isr
isr6:
    push $0
    push $6
    jmp common_isr
isr7:
    push $0
    push $7
    jmp common_isr
isr8:
    push $0
    push $8
    jmp common_isr
isr9:
    push $0
    push $9
    jmp common_isr
isr10:
    push $0
    push $10
    jmp common_isr
isr11:
    push $0
    push $11
    jmp common_isr
isr12:
    push $0
    push $12
    jmp common_isr
isr13:
    push $0
    push $13
    jmp common_isr
isr14:
    push $0
    push $14
    jmp common_isr
isr15:
    push $0
    push $15
    jmp common_isr
isr16:
    push $0
    push $16
    jmp common_isr
isr17:
    push $0
    push $17
    jmp common_isr
isr18:
    push $0
    push $18
    jmp common_isr
isr19:
    push $0
    push $19
    jmp common_isr
isr20:
    push $0
    push $20
    jmp common_isr
isr21:
    push $0
    push $21
    jmp common_isr
isr22:
    push $0
    push $22
    jmp common_isr
isr23:
    push $0
    push $23
    jmp common_isr
isr24:
    push $0
    push $24
    jmp common_isr
isr25:
    push $0
    push $25
    jmp common_isr
isr26:
    push $0
    push $26
    jmp common_isr
isr27:
    push $0
    push $27
    jmp common_isr
isr28:
    push $0
    push $28
    jmp common_isr
isr29:
    push $0
    push $29
    jmp common_isr
isr30:
    push $0
    push $30
    jmp common_isr
isr31:
    push $0
    push $31
    jmp common_isr
irq0:
    push $0
    push $32
    jmp common_irq
irq1:
    push $0
    push $33
    jmp common_irq
irq2:
    push $0
    push $34
    jmp common_irq
irq3:
    push $0
    push $35
    jmp common_irq
irq4:
    push $0
    push $36
    jmp common_irq
irq5:
    push $0
    push $37
    jmp common_irq
irq6:
    push $0
    push $38
    jmp common_irq
irq7:
    push $0
    push $39
    jmp common_irq
irq8:
    push $0
    push $40
    jmp common_irq
irq9:
    push $0
    push $41
    jmp common_irq
irq10:
    push $0
    push $42
    jmp common_irq
irq11:
    push $0
    push $43
    jmp common_irq
irq12:
    push $0
    push $44
    jmp common_irq
irq13:
    push $0
    push $45
    jmp common_irq
irq14:
    push $0
    push $46
    jmp common_irq
irq15:
    push $0
    push $47
    jmp common_irq

common_isr:
    pusha
    call isr_handler
    popa
    addl $8, %esp
    iret
common_irq:
    pusha
    call irq_handler
    popa
    addl $8, %esp
    iret
common_trap:
    pusha
    call trap_handler
    popa
    addl $8, %esp
    iret