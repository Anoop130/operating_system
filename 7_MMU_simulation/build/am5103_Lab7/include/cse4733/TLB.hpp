#ifndef LAB7_TLB_HPP
#define LAB7_TLB_HPP

#include <deque>
#include <map>
#include <optional>
#include <utility>

namespace cse4733 {

class TLB {
private:

    /**
     * @brief Maximum number of entries in the TLB.
     */
    static constexpr int MAX_ENTRIES = 10;

    /**
     * @brief entries Map of the virtual page number to page frame number loaded in memory.
     * key: = virtual page number for the memory location.
     * value: = physical frame number for the memory location.
     */
    std::map<unsigned int, unsigned int> entries;

    /**
     * @brief Maintain FIFO order for eviction.
     * The front of the deque is the oldest entry.
     * The back of the deque is the newest entry.
     * The deque is used to evict the oldest entry when the TLB is full.
     */
    std::deque<unsigned int> order;

public:

    /**
     * @brief addEntry Create an entry within the TLB.
     * @param virtual_page_number Virtual page number of the memory location.
     * @param physical_frame_number The physical page number of the memory location.
     */
    void addEntry(unsigned int virtual_page_number, unsigned int physical_frame_number);

    /**
     * @brief searchTLB Find the physical address for the virtual memory address.
     * @param virtualAddress Virtual memory address of the memory location.
     * @return Null optional if there is no entry for the virtual address. Otherwise return
     * the physical address for the virtual memory address.
     */
    auto searchTLB(unsigned int virtualAddress) -> std::optional<unsigned int>;
};

}  // namespace cse4733

#endif // LAB7_TLB_HPP