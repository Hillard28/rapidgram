#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Sep 19 15:29:02 2022

@author: rgilland
"""

import numpy as np
import pandas as pd
from rapidgram import gram

# =============================================================================
# Matching function
# =============================================================================
# Matching function
def match(
    left_df,
    right_df,
    left_id,
    right_id,
    left_strict,
    right_strict,
    left_fuzzy=None,
    right_fuzzy=None,
    threshold=None,
    mtype="full",
    ladd=None,
    radd=None,
    split=False,
    split_frames=25
):
    if split == False:
        if type(left_id) != str or type(right_id) != str:
            raise ValueError("Please enter a single column name for each dataframe ID")
        if (
            type(left_strict) == list
            and type(right_strict) == list
            and len(left_strict) != len(right_strict)
        ) or (type(left_strict) != type(right_strict)):
            raise ValueError(
                "Please enter the same number of columns for each strict match"
            )
        if left_fuzzy and right_fuzzy:
            if (
                type(left_fuzzy) == list
                and type(right_fuzzy) == list
                and len(left_fuzzy) != len(right_fuzzy)
            ) or (type(left_fuzzy) != type(right_fuzzy)):
                raise ValueError(
                    "Please enter the same number of columns for each fuzzy match"
                )
            if not threshold:
                raise ValueError(
                    "Please enter a corresponding threshold for fuzzy matching"
                )
        lcont = left_strict[:]
        rcont = right_strict[:]
        if type(left_strict) == list and type(right_strict) == list:
            lcont.insert(0, left_id)
            rcont.insert(0, right_id)
        elif type(left_strict) == str and type(right_strict) == str:
            lcont = [left_id, left_strict]
            rcont = [right_id, right_strict]
        if left_fuzzy and right_fuzzy:
            if type(left_fuzzy) == list and type(right_fuzzy) == list:
                lcont = lcont + left_fuzzy
                rcont = rcont + right_fuzzy
            elif type(left_fuzzy) == str and type(right_fuzzy) == str:
                lcont.append(left_fuzzy)
                rcont.append(right_fuzzy)
        left_df.dropna(subset=lcont, inplace=True)
        right_df.dropna(subset=rcont, inplace=True)
        if ladd and radd:
            if type(ladd) == list and type(radd) == list:
                lcont = lcont + ladd
                rcont = rcont + radd
            elif type(ladd) == str and type(radd) == str:
                lcont.append(ladd)
                rcont.append(radd)
        potential = left_df[lcont].merge(
            right_df[rcont], left_on=left_strict, right_on=right_strict, how="inner"
        )
        if left_fuzzy and right_fuzzy and threshold:
            if (
                type(left_fuzzy) == list
                and type(right_fuzzy) == list
                and type(threshold) == list
            ):
                if type(mtype) == list:
                    m = 0
                    for lvar, rvar, thres, mt in zip(
                        left_fuzzy, right_fuzzy, threshold, mtype
                    ):
                        if mt == "full":
                            potential["fuzz_" + str(m)] = [gram.ratio(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                        elif mt == "partial":
                            potential["fuzz_" + str(m)] = [gram.partial_ratio(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                        elif mt == "token":
                            potential["fuzz_" + str(m)] = [gram.token(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                        potential = potential.loc[potential["fuzz_" + str(m)] >= thres]
                        if m == 0:
                            potential["total_score"] = potential["fuzz_" + str(m)]
                        else:
                            potential["total_score"] = (
                                potential["total_score"] + potential["fuzz_" + str(m)]
                            )
                        m += 1
                    potential["total_score"] = potential["total_score"] / m
                elif type(mtype) == str:
                    m = 0
                    for lvar, rvar, thres in zip(left_fuzzy, right_fuzzy, threshold):
                        if mtype == "full":
                            potential["fuzz_" + str(m)] = [gram.ratio(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                        elif mtype == "partial":
                            potential["fuzz_" + str(m)] = [gram.partial_ratio(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                        elif mtype == "token":
                            potential["fuzz_" + str(m)] = [gram.token(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                        potential = potential.loc[potential["fuzz_" + str(m)] >= thres]
                        if m == 0:
                            potential["total_score"] = potential["fuzz_" + str(m)]
                        else:
                            potential["total_score"] = (
                                potential["total_score"] + potential["fuzz_" + str(m)]
                            )
                        m += 1
                    potential["total_score"] = potential["total_score"] / m
            elif (
                type(left_fuzzy) == str
                and type(right_fuzzy) == str
                and type(threshold) == float
            ):
                if mtype == "full":
                    potential["total_score"] = [gram.ratio(left, right) for left, right in zip(potential[left_fuzzy], potential[right_fuzzy])]
                elif mtype == "partial":
                    potential["total_score"] = [gram.partial_ratio(left, right) for left, right in zip(potential[left_fuzzy], potential[right_fuzzy])]
                elif mtype == "token":
                    potential["total_score"] = [gram.token(left, right) for left, right in zip(potential[left_fuzzy], potential[right_fuzzy])]
                potential = potential.loc[potential["total_score"] >= threshold]
        return potential
    else:
        potentials = pd.DataFrame()
        left_dfs = np.array_split(left_df, split_frames)
        for left_df in left_dfs:
            if type(left_id) != str or type(right_id) != str:
                raise ValueError("Please enter a single column name for each dataframe ID")
            if (
                type(left_strict) == list
                and type(right_strict) == list
                and len(left_strict) != len(right_strict)
            ) or (type(left_strict) != type(right_strict)):
                raise ValueError(
                    "Please enter the same number of columns for each strict match"
                )
            if left_fuzzy and right_fuzzy:
                if (
                    type(left_fuzzy) == list
                    and type(right_fuzzy) == list
                    and len(left_fuzzy) != len(right_fuzzy)
                ) or (type(left_fuzzy) != type(right_fuzzy)):
                    raise ValueError(
                        "Please enter the same number of columns for each fuzzy match"
                    )
                if not threshold:
                    raise ValueError(
                        "Please enter a corresponding threshold for fuzzy matching"
                    )
            lcont = left_strict[:]
            rcont = right_strict[:]
            if type(left_strict) == list and type(right_strict) == list:
                lcont.insert(0, left_id)
                rcont.insert(0, right_id)
            elif type(left_strict) == str and type(right_strict) == str:
                lcont = [left_id, left_strict]
                rcont = [right_id, right_strict]
            if left_fuzzy and right_fuzzy:
                if type(left_fuzzy) == list and type(right_fuzzy) == list:
                    lcont = lcont + left_fuzzy
                    rcont = rcont + right_fuzzy
                elif type(left_fuzzy) == str and type(right_fuzzy) == str:
                    lcont.append(left_fuzzy)
                    rcont.append(right_fuzzy)
            left_df.dropna(subset=lcont, inplace=True)
            right_df.dropna(subset=rcont, inplace=True)
            if ladd and radd:
                if type(ladd) == list and type(radd) == list:
                    lcont = lcont + ladd
                    rcont = rcont + radd
                elif type(ladd) == str and type(radd) == str:
                    lcont.append(ladd)
                    rcont.append(radd)
            potential = left_df[lcont].merge(
                right_df[rcont], left_on=left_strict, right_on=right_strict, how="inner"
            )
            if left_fuzzy and right_fuzzy and threshold:
                if (
                    type(left_fuzzy) == list
                    and type(right_fuzzy) == list
                    and type(threshold) == list
                ):
                    if type(mtype) == list:
                        m = 0
                        for lvar, rvar, thres, mt in zip(
                            left_fuzzy, right_fuzzy, threshold, mtype
                        ):
                            if mt == "full":
                                potential["fuzz_" + str(m)] = [gram.ratio(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                            elif mt == "partial":
                                potential["fuzz_" + str(m)] = [gram.partial_ratio(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                            elif mt == "token":
                                potential["fuzz_" + str(m)] = [gram.token(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                            potential = potential.loc[potential["fuzz_" + str(m)] >= thres]
                            if m == 0:
                                potential["total_score"] = potential["fuzz_" + str(m)]
                            else:
                                potential["total_score"] = (
                                    potential["total_score"] + potential["fuzz_" + str(m)]
                                )
                            m += 1
                        potential["total_score"] = potential["total_score"] / m
                    elif type(mtype) == str:
                        m = 0
                        for lvar, rvar, thres in zip(left_fuzzy, right_fuzzy, threshold):
                            if mtype == "full":
                                potential["fuzz_" + str(m)] = [gram.ratio(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                            elif mtype == "partial":
                                potential["fuzz_" + str(m)] = [gram.partial_ratio(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                            elif mtype == "token":
                                potential["fuzz_" + str(m)] = [gram.token(left, right) for left, right in zip(potential[lvar], potential[rvar])]
                            potential = potential.loc[potential["fuzz_" + str(m)] >= thres]
                            if m == 0:
                                potential["total_score"] = potential["fuzz_" + str(m)]
                            else:
                                potential["total_score"] = (
                                    potential["total_score"] + potential["fuzz_" + str(m)]
                                )
                            m += 1
                        potential["total_score"] = potential["total_score"] / m
                elif (
                    type(left_fuzzy) == str
                    and type(right_fuzzy) == str
                    and type(threshold) == float
                ):
                    if mtype == "full":
                        potential["total_score"] = [gram.ratio(left, right) for left, right in zip(potential[left_fuzzy], potential[right_fuzzy])]
                    elif mtype == "partial":
                        potential["total_score"] = [gram.partial_ratio(left, right) for left, right in zip(potential[left_fuzzy], potential[right_fuzzy])]
                    elif mtype == "token":
                        potential["total_score"] = [gram.token(left, right) for left, right in zip(potential[left_fuzzy], potential[right_fuzzy])]
                    potential = potential.loc[potential["total_score"] >= threshold]
            potentials = pd.concat([potentials, potential])
        return potentials