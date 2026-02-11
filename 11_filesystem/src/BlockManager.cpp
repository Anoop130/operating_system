#include "cse4733/BlockManager.hpp"
#include "cse4733/InvalidBlockIndexException.hpp"
#include "cse4733/NoFreeBlockAvailableException.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>

namespace cse4733 {

BlockManager::BlockManager(size_t totalBlocks)
    : blockSize(64), blocks(totalBlocks), freeBlocks(totalBlocks, true) {}

unsigned int BlockManager::allocateBlock() {

  auto it = std::find_if(freeBlocks.begin(), freeBlocks.end(), [](bool free) { return free; });
  if (it == freeBlocks.end()) {
    throw NoFreeBlockAvailableException();
  }
  *it = false;
  return it - freeBlocks.begin();
  // TODO: Implement the allocateBlock method
  // 1. Use std::find_if to find the first free block
  // 2. Check if a free block was found
  //    a. Mark the block as allocated
  //    b. Return the index of the allocated block
  // 3. No free block available, throw NoFreeBlockAvailableException
}

void BlockManager::freeBlock(unsigned int blockIndex) {
  if (blockIndex >= blocks.size()) {
    throw InvalidBlockIndexException(blockIndex);
  }
  freeBlocks[blockIndex] = true;
  // TODO: Implement the freeBlock method
  // 1. Check if the block index is within bounds
  //   a. Mark the block as free
  // 2. If the block index is out of bounds, throw InvalidBlockIndexException
}

std::vector<int> BlockManager::allocateBlocks(size_t numBlocks) {
  std::vector<int> allocatedBlocks;
  for (size_t i = 0; i < numBlocks; i++) {
    try {
      allocatedBlocks.push_back(allocateBlock());
    } catch (const NoFreeBlockAvailableException &e) {
      for (int block : allocatedBlocks) {
        freeBlock(block);
      }
      throw e;
    }
  }
  return allocatedBlocks;

  // TODO: Implement the allocateBlocks method
  // 1. Create vector to store the indices of allocated blocks
  // 2. Try to allocate the requested number of blocks
  //    a. Allocate a block and save the index
  //    b. Add the index of the allocated block to the vector
  // 3. If a NoFreeBlockAvailableException is thrown while allocating blocks
  //    a. Try to free the already allocated blocks
  //    b. If an InvalidBlockIndexException is thrown while freeing blocks
  //      1) Log the error or handle it as needed
  //      2) Rethrow the exception
  //    c. Rethrow the NoFreeBlockAvailableException
  // 4. Return the vector of allocated block indices
}

void BlockManager::writeBlock(unsigned int blockIndex, const std::string &data) {
  if (blockIndex >= blocks.size()) {
    throw InvalidBlockIndexException(blockIndex);
  }
  blocks[blockIndex] = data;
                                
  // TODO: Implement the writeBlock method
  // 1. Check if the block index is within bounds
  //    a. Write the data to the block
  // 2. If the block index is out of bounds, throw InvalidBlockIndexException
}

std::string BlockManager::readBlock(unsigned int blockIndex) const {
  if (blockIndex >= blocks.size()) {
    throw InvalidBlockIndexException(blockIndex);
  }
  return blocks[blockIndex];
  // TODO: Implement the readBlock method
  // 1. Check if the block index is within bounds
  //    a. Read the data from the block
  //    b. Return the data
  // 2. If the block index is out of bounds, throw InvalidBlockIndexException
}

size_t BlockManager::getBlockSize() const {
  // Return the size of each block
  return blockSize;
}

size_t BlockManager::getTotalBlocks() const {
  // Return the total number of blocks
  return blocks.size();
}

size_t BlockManager::getFreeBlockCount() const {
  // Count the number of free blocks where freeBlocks[i] is true
  size_t count = std::count_if(freeBlocks.begin(), freeBlocks.end(),
                               [](bool free) { return free; });
  return count;
}

} // namespace cse4733
