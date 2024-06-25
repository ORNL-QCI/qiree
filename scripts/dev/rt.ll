; Arrays
%Array* @__quantum__rt__array_concatenate(%Array*, %Array*)
%Array* @__quantum__rt__array_copy(%Array*, i1)
%Array* @__quantum__rt__array_create_1d(i32, i64)
i8* @__quantum__rt__array_get_element_ptr_1d(%Array*, i64)
void @__quantum__rt__array_record_output(i64, i8*)
i64 @__quantum__rt__array_get_size_1d(%Array*)
%Array* @__quantum__rt__array_slice_1d(%Array*, %Range, i1)
void @__quantum__rt__array_update_alias_count(%Array*, i32)
void @__quantum__rt__array_update_reference_count(%Array*, i32)

; Big ints
%BigInt* @__quantum__rt__bigint_add(%BigInt*, %BigInt*)
%BigInt* @__quantum__rt__bigint_bitand(%BigInt*, %BigInt*)
%BigInt* @__quantum__rt__bigint_bitnot(%BigInt*)
%BigInt* @__quantum__rt__bigint_bitor(%BigInt*, %BigInt*)
%BigInt* @__quantum__rt__bigint_bitxor(%BigInt*, %BigInt*)
%BigInt* @__quantum__rt__bigint_create_array(i32, i8*)
%BigInt* @__quantum__rt__bigint_create_i64(i64)
%BigInt* @__quantum__rt__bigint_divide(%BigInt*, %BigInt*)
i1 @__quantum__rt__bigint_equal(%BigInt*, %BigInt*)
i8* @__quantum__rt__bigint_get_data(%BigInt*)
i32 @__quantum__rt__bigint_get_length(%BigInt*)
i1 @__quantum__rt__bigint_greater(%BigInt*, %BigInt*)
i1 @__quantum__rt__bigint_greater_eq(%BigInt*, %BigInt*)
%BigInt* @__quantum__rt__bigint_modulus(%BigInt*, %BigInt*)
%BigInt* @__quantum__rt__bigint_multiply(%BigInt*, %BigInt*)
%BigInt* @__quantum__rt__bigint_negate(%BigInt*)
%BigInt* @__quantum__rt__bigint_power(%BigInt*, i32)
%BigInt* @__quantum__rt__bigint_shiftleft(%BigInt*, i64)
%BigInt*@__quantum__rt__bigint_shiftright(%BigInt*, i64)
void @__quantum__rt__bigint_subtract(%BigInt*, %BigInt*)
%String* @__quantum__rt__bigint_to_string(%BigInt*)
void @__quantum__rt__bigint_update_reference_count(%BigInt*, i32)

; Bools
void @__quantum__rt__bool_record_output(i1, i8*)
%String* @__quantum__rt__bool_to_string(i1)

; Callables
%Callable* @__quantum__rt__callable_copy(%Callable*, i1)
%Callable* @__quantum__rt__callable_create([4 x void(%Tuple*, %Tuple*, %Tuple*)*]*, [2 x void (%Tuple*, i32)]*, %Tuple*)
void @__quantum__rt__callable_invoke(%Callable*, %Tuple*, %Tuple*)
void @__quantum__rt__callable_make_adjoint(%Callable*)
void @__quantum__rt__callable_make_controlled(%Callable*)
void @__quantum__rt__callable_update_alias_count(%Callable*, i32)
void @__quantum__rt__callable_update_reference_count(%Callable*, i32)

; Capture??
void @__quantum__rt__capture_update_alias_count(%Callable*, i32)
void @__quantum__rt__capture_update_reference_count(%Callable*, i32)

; Doubles
void @__quantum__rt__double_record_output(double, i8*)
%String* @__quantum__rt__double_to_string(double)

; Fail
void @__quantum__rt__fail(%String*)

; Ints
void @__quantum__rt__int_record_output(i64, i8*)
%String* @__quantum__rt__int_to_string(i64)

; Memory usage
i8* @__quantum__rt__memory_allocate(i64)

; Messages
void @__quantum__rt__message(%String*)
void @__quantum__rt__message_record_output(%String*)

; Pauli
%String* @__quantum__rt__pauli_to_string(i2)

; Qubits
%Qubit* @__quantum__rt__qubit_allocate()
%Array* @__quantum__rt__qubit_allocate_array(i64)
void @__quantum__rt__qubit_release(%Qubit*)
void @__quantum__rt__qubit_release_array(%Array*)
%String* @__quantum__rt__qubit_to_string(%Qubit*)

; Ranges
%String* @__quantum__rt__range_to_string(%Range)

; Results
i1 @__quantum__rt__result_equal(%Result*, %Result*)
%Result* @__quantum__rt__result_get_one()
%Result* @__quantum__rt__result_get_zero()
void @__quantum__rt__result_record_output(%Result*, i8*)
%String* @__quantum__rt__result_to_string(%Result*)
void @__quantum__rt__result_update_reference_count(%Result*, i32)

; Strings
%String* @__quantum__rt__string_concatenate(%String*, %String*)
%String* @__quantum__rt__string_create(i8*)
i1 @__quantum__rt__string_equal(%String*, %String*)
i8* @__quantum__rt__string_get_data(%String*)
i32 @__quantum__rt__string_get_length(%String*)
void @__quantum__rt__string_update_reference_count(%String*, i32)

; Tuples
%Tuple* @__quantum__rt__tuple_copy(%Tuple*, i1)
%Tuple* @__quantum__rt__tuple_create(i64)
void @__quantum__rt__tuple_record_output(i64, i8*)
void @__quantum__rt__tuple_update_alias_count(%Tuple*, i32)
void @__quantum__rt__tuple_update_reference_count(%Tuple*, i32)

