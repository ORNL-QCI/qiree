#  A VERY BASIC CODE THAT TAKES A BELL QIR FILE AND OUTPUTS CORRESPONDING XACC IR REPRESENTATION

# for better visualiztion of the output
def print_bar():
    print("-------------------------------------------------------------------")

def main():
    # read qir file in .ll format or maybe in other format
    f = open('bell.ll', 'r')
    qir = f.read()    # read qir
    print_bar()
    print("Input QIR is:")
    print_bar()
    print(qir)
    print_bar()

    # Extract only the part inside "define" function
    define_index = qir.find('define')   # find where the define keyword located
    qir = qir[define_index:]  # ignore what is before "define" keyword
    bracket_start_index = qir.find('{')
    bracket_end_index = qir.find('}')

    qir = qir[bracket_start_index+1:bracket_end_index].strip()
    #print(qir)



    # quantum functions
    qir_func_dic = {'@__quantum__qis__h__body':'H',
                    '@__quantum__qis__cnot__body':'CNOT',
                    '@__quantum__qis__mz__body': 'Measure'
                    }


    # need how many qubits in the xacc input
    qalloc_size = 0
    dict_qubits = {}

    print("Output XACC IR is:")
    print_bar()
    # read qir line by line
    for line in qir.splitlines():
        fn_start_idx = line.find('@')
        fn_end_idx = line.find('(')
        if fn_start_idx == -1 or fn_end_idx==-1:  # ignore no function line (e.g., "ret void")
            continue
        function_name = line[fn_start_idx:fn_end_idx]   # e.g., "@__quantum__qis__h__body"

        if function_name in qir_func_dic:   # ignore other function names that does not exists in qir_func_dic
            print(qir_func_dic[function_name], end=" ")
            # get the (inside) part and split by comma, if the first word is %Qubit% add it to a dictionary as q0, q1 and so on.., discard %Result% keyword
            start_of_braket = line.find('(')
            arguments = line[start_of_braket+1:-1].strip()
            arguments = [x.strip() for x in arguments.split(',')]

            for args in arguments:
                if not args.startswith('%Qubit*'):
                    arguments.remove(args)


            for iteration in range(len(arguments)):
                arg = arguments[iteration]
                arg_value = arg[7:]
                if arg_value not in dict_qubits:
                    dict_qubits[arg_value] = 'q' + str(len(dict_qubits))    # represent it like q0, q1 etc..
                if iteration == len(arguments)-1:
                    print(dict_qubits[arg_value])
                else:
                    print(dict_qubits[arg_value], end=',')

    print_bar()

if __name__ == "__main__":
    main()
