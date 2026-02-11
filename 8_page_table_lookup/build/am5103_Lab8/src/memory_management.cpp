#include "memory_management.hpp"
#include "invalid_page_table_entry_exception.hpp"
#include "invalid_virtual_address_exception.hpp"
#include "page_table_entry_not_found_exception.hpp"

#include <iostream>

namespace cse4733 {
// Add a constant for the physical memory base address
// This makes addresses look more realistic (e.g., starting at 0x80000000)
constexpr unsigned int PHYSICAL_MEMORY_BASE_ADDRESS = 0x81726354;

memory_management::memory_management() {
  /** TODO: Implement the constructor for the memory_management class
   * 1. Create an empty page table as a map with pairs
   *    of unsigned integers as keys and page_table_entry
   *    as values.
   * 2. Loop over the range from 0 to PAGE_DIRECTORY_SIZE */
  for (unsigned int i = 0; i < PAGE_DIRECTORY_SIZE; i++) {
    /** a. Nested loop over the range from 0 to PAGE_TABLE_SIZE */
    for (unsigned int j = 0; j < PAGE_TABLE_SIZE; j++) {
      /** 1) Create a pair of unsigned integers (i, j)
       *    as the key.
       */
      std::pair<unsigned int, unsigned int> key(i, j);
      /** 2) Assign a new page_table_entry to the page table
       *    at the key.
       * 3) Set the valid bit of the page_table_entry to true
       *    and assign the page frame number as j.
       */
      m_pageTable[key].valid = true;
      m_pageTable[key].page_frame_number = j;
    }
  }
}

auto memory_management::pageTableLookup(unsigned int virtualAddress)
    -> unsigned int {
  // TODO: Implement the pageTableLookup method
  /**
   * 1. Function `pageTableLookup` takes an unsigned integer
   *    `virtualAddress` as a parameter
   * 2. Check if the `virtualAddress` is valid
   *    1. If the `virtualAddress` is greater than `MAX_VIRTUAL_ADDRESS`
   *       or less than 0, throw `invalid_virtual_address_exception`
   */
  if (virtualAddress > MAX_VIRTUAL_ADDRESS) {
    throw invalid_virtual_address_exception();
  }
  /** 3. Extract the `pageTableIndex`, `pageDirectoryIndex`, and
   *    `pageOffset` from the `virtualAddress`
   */
  unsigned int pageDirectoryIndex = (virtualAddress & PAGE_DIRECTORY_MASK) >> PAGE_DIRECTORY_BIT_SHIFT;
  unsigned int pageTableIndex = (virtualAddress & PAGE_TABLE_MASK) >> PAGE_TABLE_BIT_SHIFT;
  unsigned int pageOffset = virtualAddress & PAGE_OFFSET_MASK;
  /** 4. Create a pair of unsigned integers (`pageDirectoryIndex`,
   * `pageTableIndex`) as the key
   */
  std::pair<unsigned int, unsigned int> key(pageDirectoryIndex, pageTableIndex);
  /** 5. Find the page table entry using the key
   *    a. If the entry does not exist in the page table, throw
   *       `page_table_entry_not_found_exception`
   */
  auto it = m_pageTable.find(key);
  if (it == m_pageTable.end()) {
    throw page_table_entry_not_found_exception();
  }
  /** 6. Access the page table entry */
  page_table_entry entry = it->second;
  /** 7. Check if the page table entry is valid
   *    a. If the page table entry is not valid,
   *       throw `invalid_page_table_entry_exception`
   */
  if (!entry.valid) {
    throw invalid_page_table_entry_exception();
  }
  /** 8. Calculate the physical address using the memory base address, page frame
   * number from the page table entry and the `pageOffset`
   */  
  unsigned int physicalAddress = (entry.page_frame_number * PAGE_SIZE) + pageOffset + PHYSICAL_MEMORY_BASE_ADDRESS;
  /** 9. Print the virtual address, page directory index, page table index,
   *    and page offset
   */
  std::cout << "Virtual Address: 0x" << std::hex << virtualAddress << "(" << std::dec << virtualAddress << ")" << std::endl;
  std::cout << "  Page Directory Index:.. 0x" << std::hex << pageDirectoryIndex << " (" << std::dec << pageDirectoryIndex << ")" << std::endl;
  std::cout << "  Page Table Index:...... 0x" << std::hex << pageTableIndex << " (" << std::dec << pageTableIndex << ")" << std::endl;
  std::cout << "  Page offset:........... 0x" << std::hex << pageOffset << " (" << std::dec << pageOffset << ")" << std::endl;
  /** 10. Print the physical address */
  std::cout << "Physical Address: 0x" << std::hex << physicalAddress << "(" << std::dec << physicalAddress << ")" << std::endl << std::endl;
  /** 11. Return the physical address */
  return physicalAddress;
}

} // namespace cse4733