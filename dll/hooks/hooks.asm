.CODE

EXTERN Origin_PickItemCheck:QWORD
EXTERN pickupItemEnable:QWORD
PickItemCheckHook PROC
    cmp byte ptr [pickupItemEnable],0
    je quit
    mov dword ptr [rcx+240h],0
quit:
    jmp Origin_PickItemCheck
PickItemCheckHook ENDP


EXTERN Hook_GetMarkPoint:PROC
EXTERN Origin_GetMarkPoint:QWORD


Tmp_GetMarkPoint PROC
        push r15
        push r14
        push r13
        push r12
        push r11
        push r10
        push r9
        push r8
        push rdi
        push rsi
        push rbp
        push rbx
        push rdx
        push rcx
        push rax

        ; 2. 保存 XMM 寄存器
        sub rsp, 100h
        movdqu [rsp+000h], xmm0
        movdqu [rsp+010h], xmm1
        movdqu [rsp+020h], xmm2
        movdqu [rsp+030h], xmm3
        movdqu [rsp+040h], xmm4
        movdqu [rsp+050h], xmm5
        movdqu [rsp+060h], xmm6
        movdqu [rsp+070h], xmm7
        movdqu [rsp+080h], xmm8
        movdqu [rsp+090h], xmm9
        movdqu [rsp+0A0h], xmm10
        movdqu [rsp+0B0h], xmm11
        movdqu [rsp+0C0h], xmm12
        movdqu [rsp+0D0h], xmm13
        movdqu [rsp+0E0h], xmm14
        movdqu [rsp+0F0h], xmm15

;保存
        sub rsp,28h
        add rcx,220h
        call Hook_GetMarkPoint
        add rsp,28h

;恢复
        movdqu xmm15, [rsp+0F0h]
       movdqu xmm14, [rsp+0E0h]
       movdqu xmm13, [rsp+0D0h]
       movdqu xmm12, [rsp+0C0h]
       movdqu xmm11, [rsp+0B0h]
       movdqu xmm10, [rsp+0A0h]
       movdqu xmm9, [rsp+090h]
       movdqu xmm8, [rsp+080h]
       movdqu xmm7, [rsp+070h]
       movdqu xmm6, [rsp+060h]
       movdqu xmm5, [rsp+050h]
       movdqu xmm4, [rsp+040h]
       movdqu xmm3, [rsp+030h]
       movdqu xmm2, [rsp+020h]
       movdqu xmm1, [rsp+010h]
       movdqu xmm0, [rsp+000h]
       add rsp, 100h

       ; 5. 恢复整数寄存器
       pop rax
       pop rcx
       pop rdx
       pop rbx
       pop rbp
       pop rsi
       pop rdi
       pop r8
       pop r9
       pop r10
       pop r11
       pop r12
       pop r13
       pop r14
       pop r15

        jmp Origin_GetMarkPoint
Tmp_GetMarkPoint ENDP


END