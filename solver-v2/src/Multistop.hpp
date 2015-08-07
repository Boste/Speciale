#ifndef MULTISTOP_HPP
#define	MULTISTOP_HPP
#include <gecode/search.hh>


class Multistop : public Gecode::Search::Stop {
private:
    Gecode::Search::NodeStop* ns; ///< Used node stop object
    Gecode::Search::FailStop* fs; ///< Used fail stop object
    Gecode::Search::TimeStop* ts; ///< Used time stop object
public:
    Multistop(unsigned node, unsigned fail, unsigned time);
    virtual bool stop(const Gecode::Search::Statistics& s, const Gecode::Search::Options& o);
    Multistop(const Multistop& orig);
    ~Multistop();

};

#endif	/* MULTISTOP_HPP */

