#include "cse4733/Inode.hpp"

namespace cse4733 {

Inode::Inode()
    : isAllocated(false), fileSize(0), creationTime(0), modificationTime(0) {
  // Reserve space for the maximum number of direct blocks to avoid
  // reallocations
  directBlocks.reserve(MAX_DIRECT_BLOCKS);
}

void Inode::allocate(size_t size, const std::vector<int> &blocks) {
  isAllocated = true;
  fileSize = size;
  creationTime = std::time(nullptr);
  modificationTime = creationTime;
  directBlocks = blocks;
  // TODO: Allocate the inode with the given size and blocks
  // 1. Mark the inode as allocated
  // 2. Set the file size
  // 3. Set the creation time to the current time
  // 4. Set the modification time to the creation time initially
  // 5. Assign the provided blocks to the inode
}

void Inode::deallocate() {
  isAllocated = false;
  fileSize = 0;
  creationTime = 0;
  modificationTime = 0;
  directBlocks.clear();
  // TODO: Deallocate the inode
  // 1. Mark the inode as not allocated
  // 2. Reset the file size
  // 3. Reset the creation time
  // 4. Reset the modification time
  // 5. Clear all block indices
}

} // namespace cse4733
