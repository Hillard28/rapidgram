# distutils: language = c++

from libcpp.string cimport string

cdef extern from "gram.hpp":
    float cratio(string, string) nogil
    float cpartial_ratio(string, string) nogil

def ratio(s1, s2):
    if len(s1) <= 1 or len(s2) <= 1:
        if s1 == s2 and len(s1) != 0:
            return 1.0
        else:
            return 0.0
    else:
        return cratio(s1.encode("UTF-8"), s2.encode("UTF-8"))

def partial_ratio(s1, s2):
    if len(s1) <= 1 or len(s2) <= 1:
        if s1 == s2 and len(s1) != 0:
            return 1.0
        else:
            return 0.0
    elif len(s1) == len(s2):
        return cratio(s1.encode("UTF-8"), s2.encode("UTF-8"))
    else:
        return cpartial_ratio(s1.encode("UTF-8"), s2.encode("UTF-8"))