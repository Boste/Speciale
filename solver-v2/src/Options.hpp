#ifndef OPTIONS_HPP
#define	OPTIONS_HPP

class Options {
public:
    Options();
    Options(const Options& orig);
    virtual ~Options();
private:
    ínt timelimit = 0;

};

#endif	/* OPTIONS_HPP */

