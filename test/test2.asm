LOAD R0, 5
LOAD R1, 5
CMP R0, R1
JNE not_equal
LOAD R2, 10
JMP end
not_equal:
LOAD R2, 20
end:
