#!/usr/bin/env python3
# Copyright 2024 UT-Battelle, LLC, and otrher QIR-EE developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
"""
Generate interface and binding code for QIR interfaces.
"""
from collections import namedtuple
import re

def _make_type_maps():
    cpptypes = {}
    ctypes = {}
    tm_in = {}
    tm_out = {}
    for (lltype, ct, cppt) in [
            ("i2", "pauli_type", "Pauli"),
            ("qstate", "bool", "QState"),
            ("i64", "std::int64_t", "size_type"),
        ]:
        cpptypes[lltype] = cppt
        ctypes[lltype] = ct
        tm_in[lltype] = "static_cast<" + cppt + ">({})"
        tm_out[lltype] = "static_cast<" + ct + ">({})"

    for t in "Array Result String Tuple Qubit".split():
        lltype = "%" + t + "*"
        ctypes[lltype] = "std::uintptr_t"
        cpptypes[lltype] = t
        tm_in[lltype] = t + "{{{}}}" # Tuple{arg}
        tm_out[lltype] = "{}.value"

    return (cpptypes, ctypes, tm_in, tm_out)


(CPPTYPES, CTYPES, TMAP_IN, TMAP_OUT) = _make_type_maps()

def get_cpptype(t):
    return CPPTYPES.get(t, t)

def get_ctype(t):
    return CTYPES.get(t, t)

def apply_tmin(t, arg):
    try:
        tm = TMAP_IN[t]
    except KeyError:
        return arg
    return tm.format(arg)

def apply_tmout(t, arg):
    try:
        tm = TMAP_OUT[t]
    except KeyError:
        return arg
    return tm.format(arg)


RE_FUNCTION = re.compile(r"^\s*declare\s+([^@]+)\s+@__quantum__([^(]+)\(([^)]+)\)$")
class Signature(namedtuple('Signature',
                           ['ret', 'ns', 'name', 'suffix', 'args'])):
    __slots__ = ()
    @classmethod
    def from_string(cls, s):
        match = RE_FUNCTION.match(s)
        if not match:
            raise ValueError(f"failed to match string '{s}'")
        (ret, name, args) = match.groups()
        args = tuple(s.strip() for s in args.split(','))
        (ns, name, *suffix) = name.split('__')
        suffix = suffix[0] if suffix else None
        return cls(ret, ns, name, suffix, args)

SEPARATOR = "//" + "-"*75 + "//"

class Generator:
    def __init__(self):
        self.interface = []
        self.bindings = []
        self.apply_bind = []
        self.cc_code = []

    def __call__(self, line):
        line = line.rstrip()
        if not line:
            return

        if line.startswith(";"):
            return self.comment(line)

        # Assume signature
        sig = Signature.from_string(line)
        return self.signature(sig)

    def comment(self, line):
        # Comment block
        self.interface.append("")
        section = line[1:].strip()
        self.interface.extend(["//@}", "//@{", "//! \\name " + section, ""])
        self.bindings.extend([SEPARATOR, "// " + section.upper(), SEPARATOR])
        self.apply_bind.append("// " + section)


class QisGenerator(Generator):
    def signature(self, sig):
        qis_function = f"QIS_FUNCTION({sig.name}, {sig.suffix})"
        binding_decl = []

        cppname = sig.name
        if "adj" in sig.suffix:
            cppname = cppname + "_adj"

        cargs = []
        cppargs = []
        binding_args = []
        for (i, a) in enumerate(sig.args, start=1):
            aname = f"arg{i}"
            cargs.append(" ".join([get_ctype(a), aname]))
            binding_args.append(apply_tmin(a, aname))
            cppargs.append(get_cpptype(a))

        binding_decl.append(")")
        binding_call = "".join(
            ["q_interface_->", cppname, "("] +
            [", ".join(binding_args)] +
            [")"]
        )

        cpp_decl = " ".join([
            cppname, "(",
            ", ".join(cppargs),
            ")"
        ])
        self.interface.append(" ".join([
            "virtual",
            get_cpptype(sig.ret),
            cpp_decl,
            "= 0;",
            "//!<", sig.suffix
        ]))
        self.bindings.extend([
            " ".join([
                get_ctype(sig.ret),
                "QIREE_" + qis_function,
                "(", ", ".join(cargs), ")"
            ]),
            "{",
            "  return " + apply_tmout(sig.ret, binding_call) + ";",
            "}"
        ])
        self.apply_bind.append("QIREE_BIND_" + qis_function + ";")
        self.cc_code.append(" ".join([
            get_cpptype(sig.ret), "QuantumNotImpl::", cpp_decl
        ]))
        self.cc_code.extend([
            "{",
            f"QIREE_NOT_IMPLEMENTED(\"quantum instruction '{sig.name}.{sig.suffix}'\");",
            "}"
        ])




def write_lines(f, lines):
    for line in lines:
        print(line, file=f)

def generate_qis():
    process_line = QisGenerator()
    with open("qis.ll") as f:
        for line in f:
            process_line(line)

    with open("interface.hh", "w") as f:
        write_lines(f, process_line.interface)
    with open("bindings.cc", "w") as f:
        write_lines(f, process_line.interface)
        f.write("\n\n/** APPLY BIND **/\n\n")
        write_lines(f, process_line.apply_bind)
    with open("concrete.cc", "w") as f:
        write_lines(f, process_line.cc_code)

if __name__ == "__main__":
    generate_qis()
