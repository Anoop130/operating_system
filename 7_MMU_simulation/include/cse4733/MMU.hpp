#ifndef CSE4733_MMU_HPP
#define CSE4733_MMU_HPP

#include <vector>

#include "TLB.hpp"
#include "address_components.hpp"

namespace cse4733 {

class MMU {
private:
  /**
   * @brief The size of virtual memory pages.
   */
  static constexpr unsigned int PAGE_SIZE = 4096;

  /**
   * @brief Number of bits used for the offset within a page.
   */
  static constexpr unsigned int OFFSET_BITS = 12;

  /**
   * @brief Mask to extract the offset from a virtual address.
   */
  static constexpr unsigned int OFFSET_MASK = 0xfff;

  /**
   *  A translation-lookaside buffer (TLB) is part of the chip’s
   *  memory-management unit (MMU), and is simply a hardware cache
   *  of popular virtual-to-physical address translations; thus,
   *  a better name would be an address-translation cache.
   */
  TLB tlb;

  /**
   * The page table contains all physical addresses.
   */
  std::vector<unsigned int> pageTable;

  /**
   * Number of times the physical address was found in the TLB.
   */
  int tlbHits;

  /**
   * Number of times the physical address was NOT found in the TLB.
   */
  int tlbMisses;

  /**
   * @brief m_base_address Beginning address of the physical memory.
   */
  const unsigned int m_base_address;

  /**
   * @brief Helper function to extract VPN and offset from a full virtual
   * address.
   * @param virtual_address The full virtual address.
   * @return The VPN and offset.
   */
  [[nodiscard]] static auto
  extract_address_components(unsigned int virtual_address)
      -> address_components;

public:
  /**
   * @brief MMU The memory-management unit (MMU) constructor.
   * @param base_address Beginning address of the physical memory.
   */
  MMU(unsigned int base_address);

  /**
   * @brief translateAddress The MMU searches the TLB for the physical address
   * for the given virtual address.
   * @param virtualAddress Virtual memory address of the memory location.
   * @return The physical memory address of the memory location.
   */
  auto translateAddress(unsigned int virtualAddress) -> unsigned int;

  /**
   * @brief addEntry Create a memory location within the MMU.
   * @param virtualAddress Virtual memory address of the memory location.
   * @return The physical memory address of the memory location.
   */
  auto addEntry(unsigned int virtualAddress) -> unsigned int;

  /**
   * @brief getHitRatio Retrieve the TLB hit ratio.
   * @return Fractional number representing the TLB hit ratio.
   */
  auto getHitRatio() const -> double;

  /**
   * @brief getMissRatio Retrieve the TLB miss ratio.
   * @return Fractional number representing the TLB miss ratio.
   */
  auto getMissRatio() const -> double;
};

} // namespace cse4733

#endif // CSE4733_MMU_HPP