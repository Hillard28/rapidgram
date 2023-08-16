# distutils: language = c++

from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "cgram.cpp":
    float cratio(string, string, bool) nogil
    float cpartial_ratio(string, string, bool) nogil
    float ctoken(string, string) nogil
    float cpartial_token(string, string) nogil

def ratio(s1, s2, strict=False):
    return cratio(s1.encode("UTF-8"), s2.encode("UTF-8"), strict)

def partial_ratio(s1, s2, strict=False):
    return cpartial_ratio(s1.encode("UTF-8"), s2.encode("UTF-8"), strict)

def token(s1, s2):
    return ctoken(s1.encode("UTF-8"), s2.encode("UTF-8"))

def partial_token(s1, s2):
    return cpartial_token(s1.encode("UTF-8"), s2.encode("UTF-8"))