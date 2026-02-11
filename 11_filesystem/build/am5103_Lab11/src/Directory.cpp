#include "cse4733/Directory.hpp"
#include "cse4733/FileAlreadyExistsException.hpp"
#include "cse4733/FileMissingException.hpp"

namespace cse4733 {

Directory::Directory() {
  // Initializes an empty directory.
}

void Directory::addFile(const std::string &filename, int inodeIndex) {
  if (fileTable.find(filename) != fileTable.end()) {
    throw FileAlreadyExistsException(filename);
  }
  fileTable[filename] = inodeIndex;
  // TODO: Implement the addFile method
  // 1. Check if the file already exists in the directory
  //    a. If it does, throw a FileAlreadyExistsException
  // 2. Add the file to the directory with the associated inode index
}

void Directory::removeFile(const std::string &filename) {
  if (fileTable.find(filename) == fileTable.end()) {
    throw FileMissingException(filename);
  }
  fileTable.erase(filename);
  // TODO: Implement the removeFile method
  // Function: removeFile
  //
  // Input: filename (string) - The name of the file to remove
  //
  // 1. Check if the file exists in the directory
  //    a. If it does not, throw a FileMissingException
  // 2. Attempt to erase the file from the directory
}

unsigned int Directory::getInodeIndex(const std::string &filename) const {
  if (fileTable.find(filename) == fileTable.end()) {
    throw FileMissingException(filename);
  }
  return fileTable.at(filename);
  // TODO: Check if the file exists using the fileExists method
  // 1. If the file exists, find the file in the fileTable using the provided
  // filename
  //    a. If the file is found, return the associated inode index
  // 2. If the file does not exist, throw a FileMissingException with the
  // provided filename
}

std::vector<std::string> Directory::listFiles() const {
  std::vector<std::string> filenames;
  for (const auto &entry : fileTable) {
    filenames.push_back(entry.first);
  }
  return filenames;
  // TODO: List all files in the directory
  // 1. Create a vector to store the filenames
  // 2. Iterate over each entry in the fileTable
  //    a. Add the filename to the vector
  // 3. Return the vector of filenames
}

bool Directory::fileExists(const std::string &filename) const {
  if (fileTable.find(filename) != fileTable.end()) {
    return true;
  }
  return false;
  // TODO: Check if the file exists in the directory
  // 1. Use the find method of the fileTable to check for the filename
  // 2. Return true if the file is found, otherwise return false
}

} // namespace cse4733
