#include <algorithm>
#include <iostream> // For error messages (optional)

#include "cse4733/FileSystem.hpp"
#include "cse4733/FileAlreadyExistsException.hpp"
#include "cse4733/FileMissingException.hpp"
#include "cse4733/NoAvailableInodeException.hpp"
#include "cse4733/UnformattedFilesystemException.hpp"
#include "cse4733/NoFreeBlockAvailableException.hpp"

namespace cse4733
{

    FileSystem::FileSystem(size_t diskSize, size_t blockSize)
        : diskSize(diskSize), blockSize(blockSize),
          blockManager(diskSize / blockSize), inodeTable(diskSize / (blockSize * 10))
    {
        // Initialize the filesystem with a root directory and empty inode table.
    }

    FileSystem::~FileSystem()
    {
        // Destructor - no special cleanup needed for this in-memory filesystem
    }

    bool FileSystem::createFile(const std::string &filename)
    {
        if (!isFormatted) {
            throw UnformattedFilesystemException();
        }
        try {
            int inodeIndex = allocateInode();
            rootDirectory.addFile(filename, inodeIndex);
            return true;
        } catch (const NoAvailableInodeException &e) {
            return false;
        }
        // TODO:
        // 1. Check if the filesystem has been formatted
        //    a. If not, throw an UnformattedFilesystemException
        // 2. Try: Attempt to allocate a new inode
        //    a. If successful, add the file to the root directory, return true
        // 3. Any exceptions thrown should be caught and handled and return false
    }

    bool FileSystem::deleteFile(const std::string &filename)
    {
        if (!isFormatted) {
            throw UnformattedFilesystemException();
        }
        try {
            int inodeIndex = findInode(filename);
            Inode &inode = inodeTable[inodeIndex];
            for (int blockIndex : inode.directBlocks) {
                blockManager.freeBlock(blockIndex);
            }
            releaseInode(inodeIndex);
            rootDirectory.removeFile(filename);
            return true;
        } catch (const FileMissingException &e) {
            return false;
        }
        // TODO:
        // 1. Check if the filesystem has been formatted
        //    a. If not, throw an UnformattedFilesystemException
        // 2. Try: Find the inode index for the file
        //    a. Get the inode for the file
        //    b. Free the blocks associated with the file
        //    c. Release the inode
        //    d. Remove the file from the root directory
        //    e. Return true
        // 3. Catch: FileMissingException and return false
    }

    bool FileSystem::writeFile(const std::string &filename, const std::string &data)
    {
        if (!isFormatted) {
            throw UnformattedFilesystemException();
        }
        int inodeIndex = findInode(filename);
        Inode &inode = inodeTable[inodeIndex];
        for (int blockIndex : inode.directBlocks) {
            blockManager.freeBlock(blockIndex);
        }
        std::vector<int> blockIndexes = writeDataToBlocks(data);
        inode.allocate(data.size(), blockIndexes);
        return true;
        // TODO:
        // 1. Check if the filesystem has been formatted
        //    a. If not, throw an UnformattedFilesystemException
        // 2. Find the inode index for the file
        // 3. Get the inode for the file
        // 4. Free the blocks associated with the file
        // 5. Write the data to blocks
        // 6. Update inode with new block information
        // 7. Return true
    }

    std::string FileSystem::readFile(const std::string &filename)
    {
        if (!isFormatted) {
            throw UnformattedFilesystemException();
        }
        try {
            int inodeIndex = findInode(filename);
            Inode &inode = inodeTable[inodeIndex];
            std::string data = readDataFromBlocks(inode.directBlocks);
            return data;
        } catch (const FileMissingException &e) {
            return "";
        }
        // TODO:
        // 1. Check if the filesystem has been formatted
        //    a. If not, throw an UnformattedFilesystemException
        // 2. Try: Find the inode index for the file
        //    a. Get the inode for the file
        //    b. Read the data from the blocks
        //    c. Return the data
        // 3. Catch: FileMissingException and return empty string
    }

    std::vector<std::string> FileSystem::listFiles()
    {
        if (!isFormatted)
        {
            throw UnformattedFilesystemException(); // Return empty if the filesystem has not been formatted
        }

        return rootDirectory.listFiles();
    }

    bool FileSystem::format()
    {
        if (isFormatted) {
            return false;
        }
        inodeTable = std::vector<Inode>(diskSize / (blockSize * 10));
        rootDirectory = Directory();
        blockManager = BlockManager(diskSize / blockSize);
        isFormatted = true;
        return true;
        // TODO:
        // 1. Clear the inode table
        // 2. Reset the root directory
        // 3. Reset the block manager
        // 4. Set the filesystem as formatted
        // 5. Return true
    }

    int FileSystem::findInode(const std::string &filename)
    {
        if (!isFormatted)
        {
            throw UnformattedFilesystemException(); // Filesystem has not been formatted
        }
        return rootDirectory.getInodeIndex(filename);
    }

    unsigned int FileSystem::allocateInode()
    {
        if (!isFormatted) {
            throw UnformattedFilesystemException();
        }
        for (size_t i = 0; i < inodeTable.size(); i++) {
            if (!inodeTable[i].isAllocated) {
                inodeTable[i].allocate(0, std::vector<int>());
                return i;
            }
        }
        throw NoAvailableInodeException();
        // TODO:
        // 1. Check if the filesystem has been formatted
        //    a. If not, throw an UnformattedFilesystemException
        // 2. Find the first available inode
        // 3. If found, allocate the inode and return the index
        // 4. If not found, throw a NoAvailableInodeException
    }

    void FileSystem::releaseInode(int inodeIndex)
    {
        if (inodeIndex >= 0 && static_cast<size_t>(inodeIndex) < inodeTable.size())
        {
            inodeTable[inodeIndex].deallocate();
        }
    }

    std::vector<int> FileSystem::writeDataToBlocks(const std::string &data)
    {
        if (!isFormatted) {
            throw UnformattedFilesystemException();
        }
        std::vector<int> blockIndexes;
        size_t numBlocks = (data.size() + blockSize - 1) / blockSize;
        for (size_t i = 0; i < numBlocks; i++) {
            try {
                int blockIndex = blockManager.allocateBlock();
                blockIndexes.push_back(blockIndex);
                size_t offset = i * blockSize;
                size_t length = std::min(blockSize, data.size() - offset);
                std::string blockData = data.substr(offset, length);
                blockManager.writeBlock(blockIndex, blockData);
            } catch (const NoFreeBlockAvailableException &e) {
                for (int blockIndex : blockIndexes) {
                    blockManager.freeBlock(blockIndex);
                }
                return std::vector<int>();
            }
        }
        return blockIndexes;
        //TODO:
        // 1. Create vector to store block indexes
        // 2. Calculate the number of blocks needed
        // 3. For each block needed
        //    a. Allocate a block
        //    b. If allocation fails, free already allocated blocks and return empty vector
        //    c. Calculate the offset and length of the data to write
        //    d. Write the data to the block
        // 4. Return the vector of block indexes
    }

    std::string FileSystem::readDataFromBlocks(const std::vector<int> &blockIndexes)
    {
        if (!isFormatted) {
            throw UnformattedFilesystemException();
        }
        std::string data;
        for (int blockIndex : blockIndexes) {
            data += blockManager.readBlock(blockIndex);
        }
        return data;
        // TODO:
        // 1. Create a string to store the data
        // 2. Get the block size
        // 3. For each block index
        //    a. Read the block data and append to the string
        // 4. Return the data
    }

    size_t FileSystem::getFreeBlockCount() const
    {
        return blockManager.getFreeBlockCount(); // Retrieve the count of free blocks from BlockManager
    }

    size_t FileSystem::getTotalBlockCount() const
    {
        return blockManager.getTotalBlocks(); // Retrieve the total count of blocks from BlockManager
    }

} // namespace cse4733
