# distutils: language = c++

from libcpp.string cimport string

cdef extern from "gram.hpp":
    float similarity(string, string) nogil

def gram(s1, s2):
    if len(s1) <= 1 or len(s2) <= 1:
        if s1 == s2 and len(s1) != 0:
            return 1.0
        else:
            return 0.0
    else:
        return similarity(s1.encode("UTF-8"), s2.encode("UTF-8"))