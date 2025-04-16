#define PY_SSIZE_T_CLEAN
#include <iostream>
#include <Python.h>

static PyObject* test_helloword(PyObject* self){
    std::cout << "Hello, python extensions!!!";
    return Py_None;
}

static PyMethodDef test_methods[] = {
    {"helloworld", (PyCFunction)test_helloword, METH_NOARGS, NULL},
    {NULL,}
};

static struct PyModuleDef test_module = {
    PyModuleDef_HEAD_INIT,
    "test",
    NULL,
    -1,
    test_methods
};

PyMODINIT_FUNC
PyInit_test(void){
    return PyModule_Create(&test_module);
}


