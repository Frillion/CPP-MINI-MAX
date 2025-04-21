#include "./search.cpp"
#include "python3.13/Python.h"
#include <memory>
#include <python3.13/moduleobject.h>
#include <python3.13/object.h>
#include <python3.13/pytypedefs.h>

struct Agent{
    PyObject_HEAD
    Player role;
    double play_clock;
    std::unique_ptr<State> root_state;
    std::unique_ptr<move> last_legal_move;
};

static PyObject* Agent_start(Agent* self, PyObject* args){}

static PyMethodDef Agent_methods[] = {

};

static PyTypeObject AgentType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL,0)
    .tp_name = "AI.Agent",
    .tp_basicsize = sizeof(Agent),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_new = PyType_GenericNew,
};

static PyModuleDef aimodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "AI",
    .m_doc = "A Module Containing an AI Agent That Uses Negamax Search",
    .m_size = -1
};
