#ifndef _ASM_DMA_MAPPING_H
#define _ASM_DMA_MAPPING_H

#include <linux/scatterlist.h>
#include <asm/dma-coherence.h>
#include <asm/cache.h>

#ifndef CONFIG_SGI_IP27 /* Kludge to fix 2.6.39 build for IP27 */
#include <dma-coherence.h>
#endif

extern struct dma_map_ops *mips_dma_map_ops;

static inline struct dma_map_ops *get_dma_ops(struct device *dev)
{
	if (dev && dev->archdata.dma_ops)
		return dev->archdata.dma_ops;
	else
		return mips_dma_map_ops;
}

static inline bool dma_capable(struct device *dev, dma_addr_t addr, size_t size)
{
	if (!dev->dma_mask)
		return false;

#if defined(CONFIG_CPU_LOONGSON3)
	/*
	 * Memory of 3A2000 four way board's 3rd node can not be
	 * used by DMA due to hardware failer. DMA start address
	 * of node3 is 0x0000_0060_0000_0000, so we workaround this
	 * by using swiotlb when dma address >= 0x60_0000_0000.
	 */
	if ((read_c0_prid() & PRID_REV_MASK) == PRID_REV_LOONGSON3A_R2) {
		if ((addr + size) >= 0x0000006000000000UL)
			return false;
	}
#endif

	return addr + size <= *dev->dma_mask;
}

static inline void dma_mark_clean(void *addr, size_t size) {}

extern void dma_cache_sync(struct device *dev, void *vaddr, size_t size,
	       enum dma_data_direction direction);

#endif /* _ASM_DMA_MAPPING_H */
