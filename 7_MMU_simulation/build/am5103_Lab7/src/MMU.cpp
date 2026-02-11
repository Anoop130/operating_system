#include <cse4733/MMU.hpp>

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace cse4733 {

MMU::MMU(unsigned int base_address)
    : tlbHits{0}, tlbMisses{0}, m_base_address(base_address) {}

auto MMU::extract_address_components(unsigned int virtual_address)
    -> address_components {
  // Extract VPN and offset from the virtual address.
  address_components components{0, 0};
  components.vpn =
      virtual_address >> MMU::OFFSET_BITS; // Remove the lower 12 bits.
  components.offset =
      virtual_address & MMU::OFFSET_MASK; // Lower 12 bits represent the offset.
  return components;
}

auto MMU::addEntry(unsigned int virtualAddress) -> unsigned int {
  // 1. Extract the virtual page number (VPN) and the page offset from the
  //    provided virtual address.
  address_components components = extract_address_components(virtualAddress);
  unsigned int vpn = components.vpn;
  unsigned int offset = components.offset;
  
  // 2. Compute the physical frame base by adding the MMU's base physical
  //    address to the VPN multiplied by the page size.
  unsigned int physical_frame_base = m_base_address + (vpn * PAGE_SIZE);
  
  // 3. Ensure the page table can store an entry for this VPN; if it cannot,
  //    grow the page table to accommodate the VPN.
  if (vpn >= pageTable.size()) {
    pageTable.resize(vpn + 1);
  }
  
  // 4. Record the mapping from VPN to the computed physical frame base in
  //    the page table.
  pageTable[vpn] = physical_frame_base;
  
  // 5. Update the TLB by adding the mapping for the VPN to the physical
  //    frame base so future translations can be served from the TLB.
  tlb.addEntry(vpn, physical_frame_base);
  
  // 6. Return the final physical address formed by adding the page offset
  //    to the physical frame base.
  return physical_frame_base + offset;
}

auto MMU::translateAddress(unsigned int virtualAddress) -> unsigned int {
  // 1. Extract the virtual page number (VPN) and the page offset from the
  //    given virtual address.
  address_components components = extract_address_components(virtualAddress);
  unsigned int vpn = components.vpn;
  unsigned int offset = components.offset;
  
  // 2. Query the TLB for a mapping for this VPN.
  std::optional<unsigned int> tlb_result = tlb.searchTLB(vpn);
  
  // - If the TLB contains a mapping (a TLB hit):
  if (tlb_result.has_value()) {
    // * Increment the TLB hit counter.
    tlbHits++;
    
    // * Form the physical address by combining the physical frame base
    //   from the TLB with the page offset, and return it.
    return tlb_result.value() + offset;
  }
  
  // - If the TLB does not contain a mapping (a TLB miss):
  // * Increment the TLB miss counter.
  tlbMisses++;
  
  // * Create the mapping by invoking addEntry (which updates the
  //   page table and installs the entry into the TLB).
  // * Return the physical address produced by the newly created mapping.
  return addEntry(virtualAddress);
}

auto MMU::getHitRatio() const -> double {
  // 1. If the total number of TLB accesses (hits + misses) is zero, return
  //    0.0 to avoid division by zero.
  int total_accesses = tlbHits + tlbMisses;
  if (total_accesses == 0) {
    return 0.0;
  }
  
  // 2. Otherwise, return the number of TLB hits divided by the total
  //    number of TLB accesses (hits plus misses).
  return static_cast<double>(tlbHits) / static_cast<double>(total_accesses);
}

auto MMU::getMissRatio() const -> double {
  // 1. If the total number of TLB accesses (hits + misses) is zero, return
  //    0.0 to avoid division by zero.
  int total_accesses = tlbHits + tlbMisses;
  if (total_accesses == 0) {
    return 0.0;
  }
  
  // 2. Otherwise, return the number of TLB misses divided by the total
  //    number of TLB accesses (hits plus misses).
  return static_cast<double>(tlbMisses) / static_cast<double>(total_accesses);
}

} // namespace cse4733
