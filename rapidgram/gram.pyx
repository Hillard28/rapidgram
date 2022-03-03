# distutils: language = c++

from libcpp.string cimport string

cdef extern from "gram.hpp":
    string bigram(string) nogil
    string vsunion(string, string) nogil
    string ufreq(string, string) nogil
    float jaccard(int, int) nogil
    float similarity(string, string) nogil

def gram(s1, s2):
    return similarity(s1.encode(), s2.encode())
