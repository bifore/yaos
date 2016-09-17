#include <stdint.h>

#include "gdt.h"

struct gdt_entry gdt[NB_ENTRY_GDT];
struct gdt_ptr gp;

/* Properly reload the new segment registers */
extern void gdt_flush();

void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
{
   gdt[num].base_low = base & 0xFFFF;
   gdt[num].base_middle = (base >> 16) & 0xFF;
   gdt[num].base_high = (base >> 24) & 0xFF;

   gdt[num].limit_low = limit & 0xFFFF;
   gdt[num].limit_high = (limit >> 16) & 0xF;

   gdt[num].access = access;
   gdt[num].flags = flags;
}

void gdt_install(void)
{
   /* NULL descriptor */
   gdt_set_entry(0, 0, 0, 0, 0);
   /* Code segment descriptor */
   gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xC);
   /* Data segment descriptor */
   gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xC);

   /* Setup our GDT pointer */
   gp.limit = (sizeof(struct gdt_entry) * NB_ENTRY_GDT) - 1;
   gp.base = &gdt;

   /* Flush out the old GDT and install the new one */
   gdt_flush();
}
