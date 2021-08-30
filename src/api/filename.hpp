#ifndef GRAPHWALKER_FILENAMES_DEF
#define GRAPHWALKER_FILENAMES_DEF

#include <fstream>
#include <fcntl.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include "api/datatype.hpp"
#include "util/logger.hpp"

// static std::string fidname( std::string basefilename, bid_t p ){
//     std::stringstream ss;
//     ss << basefilename;
//     ss << "_GraphWalker/graphinfo/file";
//     ss << "_" << p;
//     return ss.str();
// }

// static std::string walksname( std::string basefilename, bid_t p ){
//     std::stringstream ss;
//     ss << basefilename;
//     ss << "_GraphWalker/walks/pool";
//     ss << "_" << p << ".walks";
//     return ss.str();
// }

// static std::string filerangename(std::string basefilename, uint16_t filesize_GB){
//     std::stringstream ss;
//     ss << basefilename;
//     ss << "_GraphWalker/filesize_" << filesize_GB << "GB.filerange";
//     return ss.str();
// }

// static std::string blockrangename(std::string basefilename, unsigned long long blocksize_KB){
//     std::stringstream ss;
//     ss << basefilename;
//     ss << "_GraphWalker/blocksize_" << blocksize_KB << "KB.blockrange";
//     return ss.str();
// }

// static std::string nverticesname(std::string basefilename) {
//     std::stringstream ss;
//     ss << basefilename;
//     ss << "_GraphWalker/N.nvertices"; 
//     return ss.str();
// }

/**
 * Configuration file name
 */
static std::string configname() {
    char * chi_root = getenv("NVMGRAPH_ROOT");
    if (chi_root != NULL) {
        return std::string(chi_root) + "src/api/nvmgraph.cnf";
    } else {
        return "src/api/nvmgraph.cnf";
    }
}

/**
 * Configuration file name - local version which can
 * override the version in the version control.
 */
static std::string configlocalname() {
    char * chi_root = getenv("NVMGRAPH_ROOT");
    if (chi_root != NULL) {
        return std::string(chi_root) + "src/api/nvmgraph.local.cnf";
    } else {
        return "src/api/nvmgraph.local.cnf";
    }
}

#endif