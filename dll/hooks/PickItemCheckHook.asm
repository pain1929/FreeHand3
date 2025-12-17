.CODE

EXTERN Origin_PickItemCheck:QWORD

PickItemCheckHook PROC
    mov dword ptr [rcx+240],0
    jmp Origin_PickItemCheck
PickItemCheckHook ENDP


END