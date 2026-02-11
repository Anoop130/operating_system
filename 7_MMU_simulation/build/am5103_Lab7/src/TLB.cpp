#include <algorithm>
#include <iostream>
#include <optional>

#include <cse4733/TLB.hpp>

void cse4733::TLB::addEntry(unsigned int virtual_page_number,
                            unsigned int physical_frame_number) {
  // 1. If the virtual page number (VPN) is already present in the TLB:
  if (entries.find(virtual_page_number) != entries.end()) {
    // a. Replace the existing mapping for that VPN with the given physical frame number.
    entries[virtual_page_number] = physical_frame_number;
    
    // b. Remove that VPN from the eviction-order list wherever it appears.
    auto it = std::find(order.begin(), order.end(), virtual_page_number);
    if (it != order.end()) {
      order.erase(it);
    }
    
    // c. Append that VPN to the end of the eviction-order list to mark it as most-recently used.
    order.push_back(virtual_page_number);
    
    // d. Log or print that the TLB entry was updated.
    std::cout << "TLB entry updated for VPN: 0x" << std::hex << virtual_page_number << std::endl;
    
    // e. Return from the function.
    return;
  }
  
  // 2. If the VPN is not present in the TLB:
  // a. If the number of entries in the TLB is greater than or equal to MAX_ENTRIES:
  if (entries.size() >= MAX_ENTRIES) {
    // i. Let a variable named "oldest" represent the VPN at the front of the eviction-order list.
    unsigned int oldest = order.front();
    
    // ii. Remove the front element from the eviction-order list.
    order.pop_front();
    
    // iii. Remove the mapping for the VPN stored in "oldest" from the TLB entries.
    entries.erase(oldest);
    
    // iv. Log or print which VPN was evicted.
    std::cout << "TLB is full. Evicting VPN 0x" << std::hex << oldest << std::endl;
  }
  
  // b. Create a new mapping from the VPN to the provided physical frame number in the TLB entries.
  entries[virtual_page_number] = physical_frame_number;
  
  // c. Append the VPN to the end of the eviction-order list to mark it as most-recently used.
  order.push_back(virtual_page_number);
}

auto cse4733::TLB::searchTLB(unsigned int virtualAddress)
    -> std::optional<unsigned int> {
  // 1. Initialize an empty result (no value).
  std::optional<unsigned int> result = std::nullopt;
  
  // 2. If the virtual address exists as a key in the TLB entries:
  auto it = entries.find(virtualAddress);
  if (it != entries.end()) {
    // set the result to the corresponding physical frame number.
    result = it->second;
  }
  
  // 3. Return the result (either contains a physical frame number or remains empty).
  return result;
}
