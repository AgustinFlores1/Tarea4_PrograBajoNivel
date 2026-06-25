%module gravitacion
%{
#include"variable2d.h"
#include"cuerpo.h"
#include"sim.h"
%}
%include "variable2d.h"
%include "cuerpo.h"
%include "std_vector.i"

namespace std {
    %template(VectorCuerpos) vector<cuerpo>;
}

%include "sim.h"