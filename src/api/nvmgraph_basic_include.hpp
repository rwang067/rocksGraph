#ifndef NVMGRAPH_DEF_ALLBASIC_INCLUDES
#define NVMGRAPH_DEF_ALLBASIC_INCLUDES

#include <iostream>
#include <sstream>

#include <cassert>
#include "rocksdb/db.h"

#include "util/stringop.hpp"
#include "util/logger.hpp"
#include "util/metrics/metrics.hpp"
#include "util/metrics/reps/basic_reporter.hpp"
#include "util/metrics/reps/file_reporter.hpp"
#include "util/metrics/reps/html_reporter.hpp"
#include "util/metrics/reps/null_reporter.hpp"

/**
 * Helper for metrics.
 */
static VARIABLE_IS_NOT_USED void metrics_report(metrics &m);
static VARIABLE_IS_NOT_USED void metrics_report(metrics &m) {
// std::string reporters = get_option_string("metrics.reporter", "console, file, html");
std::string reporters = "console,file";
char * creps = (char*)reporters.c_str();
const char * delims = ",";
char * t = strtok(creps, delims);

while(t != NULL) {            
    std::string repname(t);
    if (repname == "basic" || repname == "console") {
        basic_reporter rep;
        m.report(rep);
    } else if (repname == "file") {
        file_reporter rep(get_option_string("metrics.reporter.filename", "rocksgraph_metrics.txt"));
        m.report(rep);
    } else if (repname == "html") {
        html_reporter rep(get_option_string("metrics.reporter.htmlfile", "rocksgraph_metrics.html"));
        m.report(rep);
    } else {
        logstream(LOG_WARNING) << "Could not find metrics reporter with name [" << repname << "], ignoring." << std::endl;
    }
    t = strtok(NULL, delims);
}


}

#endif
