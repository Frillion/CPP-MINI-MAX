#include "search.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include "python3.13/Python.h"
#include <python3.13/exports.h>
#include <python3.13/methodobject.h>
#include <python3.13/modsupport.h>
#include <python3.13/moduleobject.h>
#include <python3.13/object.h>
#include <python3.13/pyerrors.h>
#include <python3.13/pytypedefs.h>

struct Agent{
    PyObject_HEAD
    Player role;
    double play_clock;
    std::unique_ptr<State> root_state;
    bool my_turn;
};

static PyObject* Agent_start(Agent* self, PyObject* args){
    const char* role_cstr;
    int width;
    int height;
    double play_clock;

    if(!PyArg_ParseTuple(args, "siid", &role_cstr, &width, &height, &play_clock))
        return NULL;


    if(strcmp(role_cstr, "white") == 0){
        self->role = WHITE;
        self->my_turn = false;
    }
    else{
        self->role = BLACK;
        self->my_turn = true;
    }
    
    if(!self->root_state){
        self->root_state = std::make_unique<State>(width, height);
    }
    else{
        self->root_state.reset(new State(width, height));
    }

    self->play_clock = play_clock;

    Py_RETURN_NONE;
}

static PyObject* Agent_next_action(Agent* self, PyObject* args){
    PyObject* last_move = nullptr;
    if(!PyArg_ParseTuple(args, "|O", &last_move))
        return NULL;

    int x_from, y_from, x_to, y_to;

    if(last_move != Py_None){
        if(!PyArg_ParseTuple(last_move, "iiii", &x_from, &y_from, &x_to, &y_to))
            return NULL;
    }
    else{
        last_move = nullptr;
    }

    self->my_turn = (!self->my_turn);
    if(!self->my_turn)
        return Py_BuildValue("s","NOOP");

    if(last_move){
        std::unique_ptr<move> mv = std::make_unique<move>(x_from, y_from, x_to, y_to);
        self->root_state = self->root_state->apply_move(*mv);
    }
    else{
        std::cout << "First Move!!" << std::endl;
    }

    std::vector<move> legal_moves = self->root_state->get_legal_moves();
    if(legal_moves.empty()){
        return Py_BuildValue("s","NOOP");
    }

    Player agent_col = self->role;
    std::unique_ptr<move> best_move = get_best_move_timed(*self->root_state, 7, self->play_clock, self->role);

    if(best_move){
        self->root_state = self->root_state->apply_move(*best_move);
        std::string move_msg;

        // Build the string message that will be sent for the move
        move_msg.append("(move ");
        move_msg.append(std::to_string(best_move->from.x));
        move_msg.append(" ");
        move_msg.append(std::to_string(best_move->from.y));
        move_msg.append(" ");
        move_msg.append(std::to_string(best_move->to.x));
        move_msg.append(" ");
        move_msg.append(std::to_string(best_move->to.y));
        move_msg.append(")");

        return Py_BuildValue("s",move_msg.c_str());
    }
    else{
        return Py_BuildValue("s","NOOP");
    }
}

static PyObject* Agent_cleanup(Agent* self, PyObject* args){
    self->root_state = nullptr;
    self->my_turn = self->role != WHITE;
    Py_RETURN_NONE;
}

static int Agent_init(Agent* self, PyObject* args, PyObject* kwds){
    static char* kwlist[] = {"role","play_clock", NULL};
    const char* role = nullptr;
    double play_clock = 0.0;

    if(!PyArg_ParseTupleAndKeywords(args, kwds, "|sd", kwlist, &role, &play_clock))
        return -1;

    if(role){
        self->role = strcmp(role, "white") == 0 ? WHITE : BLACK;
    }
    if(play_clock != 0.0){
        self->play_clock = play_clock;
    }

    self->root_state = nullptr;

    return 0;
}

static void Agent_dealloc(Agent* self){
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static PyMethodDef Agent_methods[] = {
    {"start", (PyCFunction)Agent_start, METH_VARARGS, "Initailizes the Agent."},
    {"next_action", (PyCFunction)Agent_next_action, METH_VARARGS, "Grabs the next move the agent should play"},
    {"cleanup", (PyCFunction)Agent_cleanup, METH_VARARGS, "Grabs the next move the agent should play"},
    {NULL}
};

static PyTypeObject AgentType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL,0)
    .tp_name = "AI.Agent",
    .tp_basicsize = sizeof(Agent),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor)Agent_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_methods = Agent_methods,
    .tp_init = (initproc)Agent_init,
    .tp_new = PyType_GenericNew,
};

static PyModuleDef aimodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "AI",
    .m_doc = "A Module Containing an AI Agent That Uses Negamax Search",
    .m_size = -1
};

PyMODINIT_FUNC
PyInit_AI(void){
    PyObject* mod;
    if(PyType_Ready(&AgentType) < 0)
        return NULL;

    mod = PyModule_Create(&aimodule);
    if(mod == NULL)
        return NULL;

    if(PyModule_AddObjectRef(mod, "Agent",(PyObject*) &AgentType) < 0){
        Py_DECREF(mod);
        return NULL;
    }

    return mod;
}
