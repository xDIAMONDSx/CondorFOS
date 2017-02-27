/*
 * Copyright (C) 2017 DropDemBits <r3usrlnd@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * condor.c: Implementation of condor.h
 */
#include <condor.h>
#include <serial.h>
#include <io.h>
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/klogger.h>

extern udword_t stack_bottom;
extern udword_t stack_top;

extern udword_t readCR0();
extern udword_t readCR2();
extern udword_t readCR3();
extern udword_t readCR4();

static udword_t version[] = {0, 1, 1, KERNEL_TYPE_ALPHA};

static char* getTable(int table)
{
    if(table & 4) return "LDT";
    else return "GDT";
}

udword_t* getKernelVersion()
{
    return version;
}

void itoa(qword_t number, char* str, int radix)
{
    int begin = 0;
    if(!number)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    if(number < 0)
    {
        str[0] = '-';
        begin++;
        number = ~(number)+1;
    }
    
    char nums[256] = {};
    
    int index = -1;

    while(number)
    {
        index++;
        nums[index] = "0123456789ABCDEF"[number % radix];
        number /= radix;
    }

    for(; index >= 0; index--) {
        str[begin++] = nums[index];
    }
    str[begin] = '\0';    
}

void kexit(int status)
{
    if(!status)
        kpanic("Placeholder");
    else
        kpanic("Abnormal exit");
}

void kpanic(const char* message)
{
    udword_t rsp = 0;
    asm("movl %%esp, %0" : "=m"(rsp) :: "memory");
    kdumpStack((uqword_t*)rsp, (udword_t)&stack_bottom);
    
    logFErr(message);
    
    asm("cli");
    for(;;) asm("pause");
}

void kputchar(const char c)
{
    terminal_putchar(c);
}

void kdump_useStack(uqword_t* rsp)
{
    udword_t* esp = (udword_t*)rsp;
    
    //Registers
    uint16_t cs =     *(esp+15) & 0xFFFF;
    uint16_t ds =     *(esp+ 3) & 0xFFFF;
    uint16_t es =     *(esp+ 2) & 0xFFFF;
    uint16_t fs =     *(esp+ 1) & 0xFFFF;
    uint16_t gs =     *(esp+ 0) & 0xFFFF;
    uint32_t eflags = *(esp+16) & 0xFFFF;
    uint32_t eip =    *(esp+14);
    uint32_t eax =    *(esp+11);
    uint32_t ebx =    *(esp+8);
    uint32_t ecx =    *(esp+10);
    uint32_t edx =    *(esp+9);
    uint32_t esp_now = 0;
    uint32_t ebp =    *(esp+6);
    uint32_t esi =    *(esp+5);
    uint32_t edi =    *(esp+4);
    
    asm("movl %%esp, %0" : "=m"(esp_now) :: "memory");
    
    //Dump Registers:
    printf("BEGIN DUMP:\n");
    printf("REGS: EAX: %#lx, EBX: %#lx, ECX: %#lx, EDX: %#lx\n", eax, ebx, ecx, edx);
    printf("ESP: %#lx, EBP: %#lx, ESI: %#lx, EDI: %#lx\n", esp_now, ebp, esi, edi);
    printf("SEGMENT REGS: VALUE (INDEX|TABLE|RPL)\n");
    printf("CS: %x (%d|%s|%d)\n", cs, cs >> 4, getTable(cs), cs & 0x2);
    printf("DS: %x (%d|%s|%d)\n", ds, ds >> 4, getTable(ds), ds & 0x2);
    printf("ES: %x (%d|%s|%d)\n", es, es >> 4, getTable(es), es & 0x2);
    printf("FS: %x (%d|%s|%d)\n", fs, fs >> 4, getTable(fs), fs & 0x2);
    printf("GS: %x (%d|%s|%d)\n", gs, gs >> 4, getTable(gs), gs & 0x2);
    printf("EFLAGS: %#lx\n", eflags);
    printf("EIP: %#lx\n", eip);
    printf("CR0: %lx, CR2: %lx, CR3: %lx, CR4: %lx\n", readCR0(), readCR2(), readCR3(), readCR4());
    //Dump Registers to serial
    char buffer[65];
    //GPRs
    serial_writes(COM1, "\nBEGIN DUMP\nREGS: EAX: 0x");
    itoa(eax, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " EBX: 0x");
    itoa(ebx, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " ECX: 0x");
    itoa(ecx, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " EDX: 0x");
    itoa(edx, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, "\nESP: 0x");
    itoa(esp_now, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " EBP: 0x");
    itoa(ebp, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " ESI: 0x");
    itoa(esi, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " EDI: 0x");
    itoa(edi, buffer, 16);
    serial_writes(COM1, buffer);
    
    //Segment registers
    serial_writes(COM1, "SREGS:\nCS: ");
    itoa(cs, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, "\nDS: ");
    itoa(ds, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, "\nES: ");
    itoa(es, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, "\nFS: ");
    itoa(fs, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, "\nGS: ");
    itoa(gs, buffer, 16);
    serial_writes(COM1, buffer);
    
    serial_writes(COM1, "\nEFLAGS: 0x");
    itoa(eflags, buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, "\nEIP: 0x");
    itoa(eip, buffer, 16);
    serial_writes(COM1, buffer);
    
    //Control Registers
    serial_writes(COM1, "\nCR0: ");
    itoa(readCR0(), buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " CR2: ");
    itoa(readCR0(), buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " CR3: ");
    itoa(readCR0(), buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, " CR4: ");
    itoa(readCR0(), buffer, 16);
    serial_writes(COM1, buffer);
    serial_writes(COM1, "\r\n");
}

void kdump_useRegs(uqword_t rip)
{
    //Registers
    uint16_t cs =     0;//__readReg(6);
    uint16_t ds =     0;//__readReg(7);
    uint16_t es =     0;//__readReg(8);
    uint16_t fs =     0;//__readReg(9);
    uint16_t gs =     0;//__readReg(10);
    uint32_t eflags = 0;//__readReg(11);
    
    uint32_t eax = 0;//__readReg(12);
    uint32_t ebx = 0;//__readReg(13);
    uint32_t ecx = 0;//__readReg(14);
    uint32_t edx = 0;//__readReg(15);
    uint32_t esp = 0;//__readReg(16);
    uint32_t ebp = 0;//__readReg(17);
    uint32_t esi = 0;//__readReg(18);
    uint32_t edi = 0;//__readReg(19);
    
    //Dump Registers:
    printf("BEGIN DUMP:\n");
    printf("REGS: EAX: %#lx, EBX: %#lx, ECX: %#lx, EDX: %#lx\n", eax, ebx, ecx, edx);
    printf("ESP: %#lx, EBP: %#lx, ESI: %#lx, EDI: %#lx\n", esp, ebp, esi, edi);
    printf("SEGMENT REGS: VALUE (INDEX|TABLE|RPL)\n");
    printf("CS: %x (%d|%s|%d)\n", cs, cs >> 4, getTable(cs), cs & 0x2);
    printf("DS: %x (%d|%s|%d)\n", ds, ds >> 4, getTable(ds), ds & 0x2);
    printf("ES: %x (%d|%s|%d)\n", es, es >> 4, getTable(es), es & 0x2);
    printf("FS: %x (%d|%s|%d)\n", fs, fs >> 4, getTable(fs), fs & 0x2);
    printf("GS: %x (%d|%s|%d)\n", gs, gs >> 4, getTable(gs), gs & 0x2);
    printf("EFLAGS: %#lx\n", eflags);
    printf("EIP: %#lx\n", (udword_t)rip);
    printf("CR0: %lx, CR2: %lx, CR3: %lx, CR4: %lx\n", readCR0(), readCR2(), readCR3(), readCR4());
    kdumpStack((uqword_t*)esp, (udword_t)&stack_bottom);
}

void kdumpStack(uqword_t* rsp, udword_t ebp)
{
    char buffer[512];
    udword_t* esp = (udword_t*) rsp;
    for(udword_t* ind = esp; ((udword_t)ind) > ebp; ind -= 4)
    {
        printf("[STACK|%#lx] [%#lx]\n", ind, *ind);
        serial_writes(COM1, "[STACK|");
        itoa((udword_t)ind, buffer, 16);
        serial_writes(COM1, buffer);
        itoa(*ind, buffer, 16);
        serial_writes(COM1, "] [");
        serial_writes(COM1, buffer);
        serial_writes(COM1, "]\n");
    }
}
