#include <iomanip>
#include <iostream>
#include <vector>
#include "cse4733/MMU.hpp"

int main() {
    // Define a base address for physical memory.
    const unsigned int physicalBase = 0x20000000;
    
    // Create an MMU with the given physical base address.
    cse4733::MMU mmu(physicalBase);

    // List of full virtual addresses (32-bit) to be translated.
    std::vector<unsigned int> virtualAddresses = {
        // Preload: 10 distinct pages (all will be misses when first loaded)
        0x30000000, // Page 0
        0x30001000, // Page 1
        0x30002000, // Page 2
        0x30003000, // Page 3
        0x30004000, // Page 4
        0x30005000, // Page 5
        0x30006000, // Page 6
        0x30007000, // Page 7
        0x30008000, // Page 8
        0x30009000, // Page 9
    
        // 30 repeated accesses to the preloaded pages (these should be TLB hits)
        0x30000000, 0x30001000, 0x30002000, 0x30003000, 0x30004000,
        0x30005000, 0x30006000, 0x30007000, 0x30008000, 0x30009000,
        0x30000000, 0x30001000, 0x30002000, 0x30003000, 0x30004000,
        0x30005000, 0x30006000, 0x30007000, 0x30008000, 0x30009000,
        0x30000000, 0x30001000, 0x30002000, 0x30003000, 0x30004000,
        0x30005000, 0x30006000, 0x30007000, 0x30008000, 0x30009000,
    
        // Introduce 3 new pages to force evictions (these will be misses)
        0x3000A000, // Page 10
        0x3000B000, // Page 11
        0x3000C000, // Page 12
    
        // Finally, repeat these new pages a few times so that they result in hits
        0x3000A000, 0x3000B000, 0x3000C000, 0x3000A000, 0x3000B000, 0x3000C000, 0x3000A000
    };
    
    

    // Translate each virtual address.
    for (unsigned int va : virtualAddresses) {
        try {
            unsigned int pa = mmu.translateAddress(va);
            std::cout << "Virtual address 0x" << std::hex << va
                      << " translates to physical address 0x" << pa << std::endl;
        } catch (const std::exception &e) {
            std::cout << "Error translating virtual address 0x" << std::hex << va
                      << ": " << e.what() << std::endl;
        }
    }

    // Print out the TLB hit and miss ratios.
    std::cout << "TLB Hit Ratio: " << mmu.getHitRatio() << std::endl;
    std::cout << "TLB Miss Ratio: " << mmu.getMissRatio() << std::endl;

    return 0;
}
