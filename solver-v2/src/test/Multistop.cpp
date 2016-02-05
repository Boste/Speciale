#include "Multistop.hpp"


/// Creates a Stop object with three stop criteria node, fail, and time. 
/// Giving 0 as an argument will ignorer that criteria. Time is in ms. 
Multistop::Multistop(unsigned node, unsigned fail, unsigned time) :
ns((node > 0) ? new Gecode::Search::NodeStop(node) : NULL),
fs((fail > 0) ? new Gecode::Search::FailStop(fail) : NULL),
ts((time > 0) ? new Gecode::Search::TimeStop(time) : NULL) {
}

/// Return true if node, time or fail limit is exceeded

bool Multistop::stop(const Gecode::Search::Statistics& s, const Gecode::Search::Options& o) {
    if ((ns != NULL) && ns->stop(s, o)) {
        std::cout <<  "Stopped because of Node limit" << std::endl;
    } else 
    if ((fs != NULL) && fs->stop(s, o)) {
        std::cout <<  "Stopped because of Fail limit" << std::endl;
    } else
    if ((ts != NULL) && ts->stop(s, o)) {
        std::cout <<  "Stopped because of time limit" << std::endl;
    }
    
    called++;
    
    
    return
    ((ns != NULL) && ns->stop(s, o)) ||
            ((fs != NULL) && fs->stop(s, o)) ||
            ((ts != NULL) && ts->stop(s, o));
}

Multistop::Multistop(const Multistop& orig) {
    this->ns = orig.ns;
    this->fs = orig.fs;
    this->ts = orig.ts;

}

Multistop::~Multistop() {
    delete ns;
    delete fs;
    delete ts;
}

