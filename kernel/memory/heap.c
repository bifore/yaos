#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/memory.h>
#include <kernel/paging.h>

uint32_t *kernel_heap_top;

void heap_init(void)
{
   kernel_heap_top = (uint32_t *) KERNEL_HEAP_BASE_ADDR;
}

void *heap_expand(ptrdiff_t increment)
{
   assert(increment % FRAME_SIZE == 0);

   /* Expand the heap */
   if(increment >= 0) {
      assert((uint32_t) kernel_heap_top + increment < KERNEL_HEAP_TOP_ADDR);

      uint32_t nb_pages = increment / FRAME_SIZE;   
      void *address = kernel_heap_top;

      for(size_t i = 0; i < nb_pages; ++i) {
         void *frame = phys_mem_alloc_frame();
         assert(frame != NULL);

         virt_mem_map_page(frame, kernel_heap_top, TABLE_PRESENT_BIT | TABLE_WRITABLE_BIT);

         memset(kernel_heap_top, 0, FRAME_SIZE);
         kernel_heap_top = (uint32_t *) ((uint32_t) kernel_heap_top + FRAME_SIZE);
      }

      return address;
   }
   /* Contract the heap */
   else {
      assert((uint32_t) kernel_heap_top - increment >= KERNEL_HEAP_BASE_ADDR);

      uint32_t nb_pages = (-increment) / FRAME_SIZE;

      for(size_t i = 0; i < nb_pages; ++i) {
         kernel_heap_top = (uint32_t *) ((uint32_t) kernel_heap_top - FRAME_SIZE);

         void *frame = virt_mem_get_phys_addr(kernel_heap_top);

         phys_mem_free_frame(frame);
         virt_mem_unmap_page(kernel_heap_top);
      }

      return kernel_heap_top;
   }
}
