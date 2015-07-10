/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2001
 *
 *  Last modified:
 *     $Date: 2013-07-08 14:22:40 +0200 (Mon, 08 Jul 2013) $ by $Author: schulte $
 *     $Revision: 13820 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */



#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include "BP_Data.hpp"

#ifndef GECODELINEAR_HPP
#define	GECODELINEAR_HPP

using namespace Gecode;

/**
 * \brief %Example: Magic squares
 *
 * Compute magic squares of arbitrary size
 *
 * See problem 19 at http://www.csplib.org/.
 *
 * \ingroup Example
 *
 */
class MagicSquare : public Script {
private:
    /// Size of magic square

    /// Fields of square
    IntVarArray varInt;

public:
    /// Post constraints

    MagicSquare(Gecode::InstanceOptions opt) {
        BP_Input* p = new BP_Input(opt.instance());

        Gecode::IntVarArray varInt(*this, p->getNvars(), 0, 1);

        for (unsigned i = 0; i < p->getNcons(); i++) {
            const vector<elem> leftside = p->getMatcoeff(i);
            bounds b = p->getBterms(i);
            Gecode::IntArgs c(leftside.size());
            Gecode::IntVarArgs x(leftside.size());
            for (unsigned j = 0; j < leftside.size(); j++) {
                elem e = leftside[j];

                c[j] = static_cast<int> (e.coeff);
                x[j] = varInt[e.index];
            }




            //        std::cout << "før gecode" << std::endl;

            int ub = static_cast<int> (b.ub);
            if (b.type == 5) {
                Gecode::linear(*this, c, x, Gecode::IRT_EQ, ub, Gecode::ICL_DOM);
            } else {
                Gecode::linear(*this, c, x, Gecode::IRT_LQ, ub, Gecode::ICL_DOM);
            }

        }
        delete p;

        // Branch
        Gecode::branch(*this, varInt, Gecode::INT_VAR_ACTIVITY_MAX(), Gecode::INT_VAL_MIN());
    }

    /// Constructor for cloning \a s

    MagicSquare(bool share, MagicSquare& s) : Script(share, s) {
        varInt.update(*this, share, s.varInt);
    }

    /// Copy during cloning

    virtual Space*
    copy(bool share) {
        return new MagicSquare(share, *this);
    }
    /// Print solution

    virtual void
    print(std::ostream& os) const {
        // Matrix-wrapper for the square
        for (int i = 0; i < varInt.size(); i++) {
            os << varInt[i];
        }
        os << std::endl;

    }

};

/** \brief Main-function
 *  \relates MagicSquare
 */
//int
//main(int argc, char* argv[]) {
//  SizeOptions opt("MagicSquare");
//  opt.iterations(1);
//  opt.size(7);
//  opt.parse(argc,argv);
//  Script::run<MagicSquare,DFS,SizeOptions>(opt);
//  return 0;
//}

// STATISTICS: example-any
#endif