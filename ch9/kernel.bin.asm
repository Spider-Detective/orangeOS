00030400  BC80280300        mov esp,0x32880
00030405  C705A02C03000000  mov dword [dword 0x32ca0],0x0
         -0000
0003040F  0F0105A42C0300    sgdt [dword 0x32ca4]
00030416  E87C000000        call 0x30497
0003041B  0F0115A42C0300    lgdt [dword 0x32ca4]
00030422  0F011D80280300    lidt [dword 0x32880]
00030429  EA300403000800    jmp 0x8:0x30430
00030430  0F0B              ud2
00030432  EA000000004000    jmp 0x40:0x0
00030439  F4                hlt
0003043A  6AFF              push byte -0x1
0003043C  6A00              push byte +0x0
0003043E  EB4E              jmp short 0x3048e
00030440  6AFF              push byte -0x1
00030442  6A01              push byte +0x1
00030444  EB48              jmp short 0x3048e
00030446  6AFF              push byte -0x1
00030448  6A02              push byte +0x2
0003044A  EB42              jmp short 0x3048e
0003044C  6AFF              push byte -0x1
0003044E  6A03              push byte +0x3
00030450  EB3C              jmp short 0x3048e
00030452  6AFF              push byte -0x1
00030454  6A04              push byte +0x4
00030456  EB36              jmp short 0x3048e
00030458  6AFF              push byte -0x1
0003045A  6A05              push byte +0x5
0003045C  EB30              jmp short 0x3048e
0003045E  6AFF              push byte -0x1
00030460  6A06              push byte +0x6
00030462  EB2A              jmp short 0x3048e
00030464  6AFF              push byte -0x1
00030466  6A07              push byte +0x7
00030468  EB24              jmp short 0x3048e
0003046A  6A08              push byte +0x8
0003046C  EB20              jmp short 0x3048e
0003046E  6AFF              push byte -0x1
00030470  6A09              push byte +0x9
00030472  EB1A              jmp short 0x3048e
00030474  6A0A              push byte +0xa
00030476  EB16              jmp short 0x3048e
00030478  6A0B              push byte +0xb
0003047A  EB12              jmp short 0x3048e
0003047C  6A0C              push byte +0xc
0003047E  EB0E              jmp short 0x3048e
00030480  6A0D              push byte +0xd
00030482  EB0A              jmp short 0x3048e
00030484  6A0E              push byte +0xe
00030486  EB06              jmp short 0x3048e
00030488  6AFF              push byte -0x1
0003048A  6A10              push byte +0x10
0003048C  EB00              jmp short 0x3048e
0003048E  E8AE030000        call 0x30841
00030493  83C408            add esp,byte +0x8
00030496  F4                hlt
00030497  55                push ebp
00030498  89E5              mov ebp,esp
0003049A  53                push ebx
0003049B  83EC14            sub esp,byte +0x14
0003049E  E8B7000000        call 0x3055a
000304A3  81C35D1B0000      add ebx,0x1b5d
000304A9  83EC0C            sub esp,byte +0xc
000304AC  8D834CEBFFFF      lea eax,[ebx-0x14b4]
000304B2  50                push eax
000304B3  E8C8050000        call 0x30a80
000304B8  83C410            add esp,byte +0x10
000304BB  C7C0A42C0300      mov eax,0x32ca4
000304C1  0FB700            movzx eax,word [eax]
000304C4  0FB7C0            movzx eax,ax
000304C7  8D5001            lea edx,[eax+0x1]
000304CA  C7C0A42C0300      mov eax,0x32ca4
000304D0  8D4002            lea eax,[eax+0x2]
000304D3  8B00              mov eax,[eax]
000304D5  83EC04            sub esp,byte +0x4
000304D8  52                push edx
000304D9  50                push eax
000304DA  C7C0A0280300      mov eax,0x328a0
000304E0  50                push eax
000304E1  E83A060000        call 0x30b20
000304E6  83C410            add esp,byte +0x10
000304E9  C7C0A42C0300      mov eax,0x32ca4
000304EF  8945F4            mov [ebp-0xc],eax
000304F2  C7C0A42C0300      mov eax,0x32ca4
000304F8  8D4002            lea eax,[eax+0x2]
000304FB  8945F0            mov [ebp-0x10],eax
000304FE  8B45F4            mov eax,[ebp-0xc]
00030501  66C700FF03        mov word [eax],0x3ff
00030506  C7C0A0280300      mov eax,0x328a0
0003050C  89C2              mov edx,eax
0003050E  8B45F0            mov eax,[ebp-0x10]
00030511  8910              mov [eax],edx
00030513  C7C080280300      mov eax,0x32880
00030519  8945EC            mov [ebp-0x14],eax
0003051C  C7C080280300      mov eax,0x32880
00030522  8D4002            lea eax,[eax+0x2]
00030525  8945E8            mov [ebp-0x18],eax
00030528  8B45F4            mov eax,[ebp-0xc]
0003052B  66C700FF03        mov word [eax],0x3ff
00030530  C7C0A0280300      mov eax,0x328a0
00030536  89C2              mov edx,eax
00030538  8B45F0            mov eax,[ebp-0x10]
0003053B  8910              mov [eax],edx
0003053D  E8DF000000        call 0x30621
00030542  83EC0C            sub esp,byte +0xc
00030545  8D8376EBFFFF      lea eax,[ebx-0x148a]
0003054B  50                push eax
0003054C  E82F050000        call 0x30a80
00030551  83C410            add esp,byte +0x10
00030554  90                nop
00030555  8B5DFC            mov ebx,[ebp-0x4]
00030558  C9                leave
00030559  C3                ret
0003055A  8B1C24            mov ebx,[esp]
0003055D  C3                ret
0003055E  55                push ebp
0003055F  89E5              mov ebp,esp
00030561  53                push ebx
00030562  83EC04            sub esp,byte +0x4
00030565  E8F0FFFFFF        call 0x3055a
0003056A  81C3961A0000      add ebx,0x1a96
00030570  83EC08            sub esp,byte +0x8
00030573  6A11              push byte +0x11
00030575  6A20              push byte +0x20
00030577  E885050000        call 0x30b01
0003057C  83C410            add esp,byte +0x10
0003057F  83EC08            sub esp,byte +0x8
00030582  6A11              push byte +0x11
00030584  68A0000000        push dword 0xa0
00030589  E873050000        call 0x30b01
0003058E  83C410            add esp,byte +0x10
00030591  83EC08            sub esp,byte +0x8
00030594  6A20              push byte +0x20
00030596  6A21              push byte +0x21
00030598  E864050000        call 0x30b01
0003059D  83C410            add esp,byte +0x10
000305A0  83EC08            sub esp,byte +0x8
000305A3  6A28              push byte +0x28
000305A5  68A1000000        push dword 0xa1
000305AA  E852050000        call 0x30b01
000305AF  83C410            add esp,byte +0x10
000305B2  83EC08            sub esp,byte +0x8
000305B5  6A04              push byte +0x4
000305B7  6A21              push byte +0x21
000305B9  E843050000        call 0x30b01
000305BE  83C410            add esp,byte +0x10
000305C1  83EC08            sub esp,byte +0x8
000305C4  6A02              push byte +0x2
000305C6  68A1000000        push dword 0xa1
000305CB  E831050000        call 0x30b01
000305D0  83C410            add esp,byte +0x10
000305D3  83EC08            sub esp,byte +0x8
000305D6  6A01              push byte +0x1
000305D8  6A21              push byte +0x21
000305DA  E822050000        call 0x30b01
000305DF  83C410            add esp,byte +0x10
000305E2  83EC08            sub esp,byte +0x8
000305E5  6A01              push byte +0x1
000305E7  68A1000000        push dword 0xa1
000305EC  E810050000        call 0x30b01
000305F1  83C410            add esp,byte +0x10
000305F4  83EC08            sub esp,byte +0x8
000305F7  68FF000000        push dword 0xff
000305FC  6A21              push byte +0x21
000305FE  E8FE040000        call 0x30b01
00030603  83C410            add esp,byte +0x10
00030606  83EC08            sub esp,byte +0x8
00030609  68FF000000        push dword 0xff
0003060E  68A1000000        push dword 0xa1
00030613  E8E9040000        call 0x30b01
00030618  83C410            add esp,byte +0x10
0003061B  90                nop
0003061C  8B5DFC            mov ebx,[ebp-0x4]
0003061F  C9                leave
00030620  C3                ret
00030621  55                push ebp
00030622  89E5              mov ebp,esp
00030624  53                push ebx
00030625  83EC04            sub esp,byte +0x4
00030628  E82DFFFFFF        call 0x3055a
0003062D  81C3D3190000      add ebx,0x19d3
00030633  E826FFFFFF        call 0x3055e
00030638  6A00              push byte +0x0
0003063A  C7C03A040300      mov eax,0x3043a
00030640  50                push eax
00030641  688E000000        push dword 0x8e
00030646  6A00              push byte +0x0
00030648  E871010000        call 0x307be
0003064D  83C410            add esp,byte +0x10
00030650  6A00              push byte +0x0
00030652  C7C040040300      mov eax,0x30440
00030658  50                push eax
00030659  688E000000        push dword 0x8e
0003065E  6A01              push byte +0x1
00030660  E859010000        call 0x307be
00030665  83C410            add esp,byte +0x10
00030668  6A00              push byte +0x0
0003066A  C7C046040300      mov eax,0x30446
00030670  50                push eax
00030671  688E000000        push dword 0x8e
00030676  6A02              push byte +0x2
00030678  E841010000        call 0x307be
0003067D  83C410            add esp,byte +0x10
00030680  6A03              push byte +0x3
00030682  C7C04C040300      mov eax,0x3044c
00030688  50                push eax
00030689  688E000000        push dword 0x8e
0003068E  6A03              push byte +0x3
00030690  E829010000        call 0x307be
00030695  83C410            add esp,byte +0x10
00030698  6A03              push byte +0x3
0003069A  C7C052040300      mov eax,0x30452
000306A0  50                push eax
000306A1  688E000000        push dword 0x8e
000306A6  6A04              push byte +0x4
000306A8  E811010000        call 0x307be
000306AD  83C410            add esp,byte +0x10
000306B0  6A00              push byte +0x0
000306B2  C7C058040300      mov eax,0x30458
000306B8  50                push eax
000306B9  688E000000        push dword 0x8e
000306BE  6A05              push byte +0x5
000306C0  E8F9000000        call 0x307be
000306C5  83C410            add esp,byte +0x10
000306C8  6A00              push byte +0x0
000306CA  C7C05E040300      mov eax,0x3045e
000306D0  50                push eax
000306D1  688E000000        push dword 0x8e
000306D6  6A06              push byte +0x6
000306D8  E8E1000000        call 0x307be
000306DD  83C410            add esp,byte +0x10
000306E0  6A00              push byte +0x0
000306E2  C7C064040300      mov eax,0x30464
000306E8  50                push eax
000306E9  688E000000        push dword 0x8e
000306EE  6A07              push byte +0x7
000306F0  E8C9000000        call 0x307be
000306F5  83C410            add esp,byte +0x10
000306F8  6A00              push byte +0x0
000306FA  C7C06A040300      mov eax,0x3046a
00030700  50                push eax
00030701  688E000000        push dword 0x8e
00030706  6A08              push byte +0x8
00030708  E8B1000000        call 0x307be
0003070D  83C410            add esp,byte +0x10
00030710  6A00              push byte +0x0
00030712  C7C06E040300      mov eax,0x3046e
00030718  50                push eax
00030719  688E000000        push dword 0x8e
0003071E  6A09              push byte +0x9
00030720  E899000000        call 0x307be
00030725  83C410            add esp,byte +0x10
00030728  6A00              push byte +0x0
0003072A  C7C074040300      mov eax,0x30474
00030730  50                push eax
00030731  688E000000        push dword 0x8e
00030736  6A0A              push byte +0xa
00030738  E881000000        call 0x307be
0003073D  83C410            add esp,byte +0x10
00030740  6A00              push byte +0x0
00030742  C7C078040300      mov eax,0x30478
00030748  50                push eax
00030749  688E000000        push dword 0x8e
0003074E  6A0B              push byte +0xb
00030750  E869000000        call 0x307be
00030755  83C410            add esp,byte +0x10
00030758  6A00              push byte +0x0
0003075A  C7C07C040300      mov eax,0x3047c
00030760  50                push eax
00030761  688E000000        push dword 0x8e
00030766  6A0C              push byte +0xc
00030768  E851000000        call 0x307be
0003076D  83C410            add esp,byte +0x10
00030770  6A00              push byte +0x0
00030772  C7C080040300      mov eax,0x30480
00030778  50                push eax
00030779  688E000000        push dword 0x8e
0003077E  6A0D              push byte +0xd
00030780  E839000000        call 0x307be
00030785  83C410            add esp,byte +0x10
00030788  6A00              push byte +0x0
0003078A  C7C084040300      mov eax,0x30484
00030790  50                push eax
00030791  688E000000        push dword 0x8e
00030796  6A0E              push byte +0xe
00030798  E821000000        call 0x307be
0003079D  83C410            add esp,byte +0x10
000307A0  6A00              push byte +0x0
000307A2  C7C088040300      mov eax,0x30488
000307A8  50                push eax
000307A9  688E000000        push dword 0x8e
000307AE  6A10              push byte +0x10
000307B0  E809000000        call 0x307be
000307B5  83C410            add esp,byte +0x10
000307B8  90                nop
000307B9  8B5DFC            mov ebx,[ebp-0x4]
000307BC  C9                leave
000307BD  C3                ret
000307BE  55                push ebp
000307BF  89E5              mov ebp,esp
000307C1  53                push ebx
000307C2  83EC1C            sub esp,byte +0x1c
000307C5  E8C1010000        call 0x3098b
000307CA  0536180000        add eax,0x1836
000307CF  8B5D08            mov ebx,[ebp+0x8]
000307D2  8B4D0C            mov ecx,[ebp+0xc]
000307D5  8B5514            mov edx,[ebp+0x14]
000307D8  885DE8            mov [ebp-0x18],bl
000307DB  884DE4            mov [ebp-0x1c],cl
000307DE  8855E0            mov [ebp-0x20],dl
000307E1  0FB655E8          movzx edx,byte [ebp-0x18]
000307E5  C1E203            shl edx,byte 0x3
000307E8  C7C0C02C0300      mov eax,0x32cc0
000307EE  01D0              add eax,edx
000307F0  8945F8            mov [ebp-0x8],eax
000307F3  8B4510            mov eax,[ebp+0x10]
000307F6  8945F4            mov [ebp-0xc],eax
000307F9  8B45F4            mov eax,[ebp-0xc]
000307FC  89C2              mov edx,eax
000307FE  8B45F8            mov eax,[ebp-0x8]
00030801  668910            mov [eax],dx
00030804  8B45F8            mov eax,[ebp-0x8]
00030807  66C740020800      mov word [eax+0x2],0x8
0003080D  8B45F8            mov eax,[ebp-0x8]
00030810  C6400400          mov byte [eax+0x4],0x0
00030814  0FB645E0          movzx eax,byte [ebp-0x20]
00030818  C1E005            shl eax,byte 0x5
0003081B  89C2              mov edx,eax
0003081D  0FB645E4          movzx eax,byte [ebp-0x1c]
00030821  09D0              or eax,edx
00030823  89C2              mov edx,eax
00030825  8B45F8            mov eax,[ebp-0x8]
00030828  885005            mov [eax+0x5],dl
0003082B  8B45F4            mov eax,[ebp-0xc]
0003082E  C1E810            shr eax,byte 0x10
00030831  89C2              mov edx,eax
00030833  8B45F8            mov eax,[ebp-0x8]
00030836  66895006          mov [eax+0x6],dx
0003083A  90                nop
0003083B  83C41C            add esp,byte +0x1c
0003083E  5B                pop ebx
0003083F  5D                pop ebp
00030840  C3                ret
00030841  55                push ebp
00030842  89E5              mov ebp,esp
00030844  57                push edi
00030845  56                push esi
00030846  53                push ebx
00030847  83EC6C            sub esp,byte +0x6c
0003084A  E80BFDFFFF        call 0x3055a
0003084F  81C3B1170000      add ebx,0x17b1
00030855  C745E074000000    mov dword [ebp-0x20],0x74
0003085C  8D4590            lea eax,[ebp-0x70]
0003085F  8D9320000000      lea edx,[ebx+0x20]
00030865  B914000000        mov ecx,0x14
0003086A  89C7              mov edi,eax
0003086C  89D6              mov esi,edx
0003086E  F3A5              rep movsd
00030870  C7C0A02C0300      mov eax,0x32ca0
00030876  C70000000000      mov dword [eax],0x0
0003087C  C745E400000000    mov dword [ebp-0x1c],0x0
00030883  EB16              jmp short 0x3089b
00030885  83EC0C            sub esp,byte +0xc
00030888  8D8390EBFFFF      lea eax,[ebx-0x1470]
0003088E  50                push eax
0003088F  E8EC010000        call 0x30a80
00030894  83C410            add esp,byte +0x10
00030897  8345E401          add dword [ebp-0x1c],byte +0x1
0003089B  817DE48F010000    cmp dword [ebp-0x1c],0x18f
000308A2  7EE1              jng 0x30885
000308A4  C7C0A02C0300      mov eax,0x32ca0
000308AA  C70000000000      mov dword [eax],0x0
000308B0  83EC08            sub esp,byte +0x8
000308B3  FF75E0            push dword [ebp-0x20]
000308B6  8D8392EBFFFF      lea eax,[ebx-0x146e]
000308BC  50                push eax
000308BD  E8FE010000        call 0x30ac0
000308C2  83C410            add esp,byte +0x10
000308C5  8B4508            mov eax,[ebp+0x8]
000308C8  8B448590          mov eax,[ebp+eax*4-0x70]
000308CC  83EC08            sub esp,byte +0x8
000308CF  FF75E0            push dword [ebp-0x20]
000308D2  50                push eax
000308D3  E8E8010000        call 0x30ac0
000308D8  83C410            add esp,byte +0x10
000308DB  83EC08            sub esp,byte +0x8
000308DE  FF75E0            push dword [ebp-0x20]
000308E1  8D83A2EBFFFF      lea eax,[ebx-0x145e]
000308E7  50                push eax
000308E8  E8D3010000        call 0x30ac0
000308ED  83C410            add esp,byte +0x10
000308F0  83EC08            sub esp,byte +0x8
000308F3  FF75E0            push dword [ebp-0x20]
000308F6  8D83A5EBFFFF      lea eax,[ebx-0x145b]
000308FC  50                push eax
000308FD  E8BE010000        call 0x30ac0
00030902  83C410            add esp,byte +0x10
00030905  83EC0C            sub esp,byte +0xc
00030908  FF7518            push dword [ebp+0x18]
0003090B  E834010000        call 0x30a44
00030910  83C410            add esp,byte +0x10
00030913  83EC08            sub esp,byte +0x8
00030916  FF75E0            push dword [ebp-0x20]
00030919  8D83AEEBFFFF      lea eax,[ebx-0x1452]
0003091F  50                push eax
00030920  E89B010000        call 0x30ac0
00030925  83C410            add esp,byte +0x10
00030928  83EC0C            sub esp,byte +0xc
0003092B  FF7514            push dword [ebp+0x14]
0003092E  E811010000        call 0x30a44
00030933  83C410            add esp,byte +0x10
00030936  83EC08            sub esp,byte +0x8
00030939  FF75E0            push dword [ebp-0x20]
0003093C  8D83B3EBFFFF      lea eax,[ebx-0x144d]
00030942  50                push eax
00030943  E878010000        call 0x30ac0
00030948  83C410            add esp,byte +0x10
0003094B  83EC0C            sub esp,byte +0xc
0003094E  FF7510            push dword [ebp+0x10]
00030951  E8EE000000        call 0x30a44
00030956  83C410            add esp,byte +0x10
00030959  837D0CFF          cmp dword [ebp+0xc],byte -0x1
0003095D  7423              jz 0x30982
0003095F  83EC08            sub esp,byte +0x8
00030962  FF75E0            push dword [ebp-0x20]
00030965  8D83B9EBFFFF      lea eax,[ebx-0x1447]
0003096B  50                push eax
0003096C  E84F010000        call 0x30ac0
00030971  83C410            add esp,byte +0x10
00030974  83EC0C            sub esp,byte +0xc
00030977  FF750C            push dword [ebp+0xc]
0003097A  E8C5000000        call 0x30a44
0003097F  83C410            add esp,byte +0x10
00030982  90                nop
00030983  8D65F4            lea esp,[ebp-0xc]
00030986  5B                pop ebx
00030987  5E                pop esi
00030988  5F                pop edi
00030989  5D                pop ebp
0003098A  C3                ret
0003098B  8B0424            mov eax,[esp]
0003098E  C3                ret
0003098F  55                push ebp
00030990  89E5              mov ebp,esp
00030992  83EC10            sub esp,byte +0x10
00030995  E8F1FFFFFF        call 0x3098b
0003099A  0566160000        add eax,0x1666
0003099F  8B4508            mov eax,[ebp+0x8]
000309A2  8945FC            mov [ebp-0x4],eax
000309A5  C745F000000000    mov dword [ebp-0x10],0x0
000309AC  8B45FC            mov eax,[ebp-0x4]
000309AF  8D5001            lea edx,[eax+0x1]
000309B2  8955FC            mov [ebp-0x4],edx
000309B5  C60030            mov byte [eax],0x30
000309B8  8B45FC            mov eax,[ebp-0x4]
000309BB  8D5001            lea edx,[eax+0x1]
000309BE  8955FC            mov [ebp-0x4],edx
000309C1  C60078            mov byte [eax],0x78
000309C4  837D0C00          cmp dword [ebp+0xc],byte +0x0
000309C8  750E              jnz 0x309d8
000309CA  8B45FC            mov eax,[ebp-0x4]
000309CD  8D5001            lea edx,[eax+0x1]
000309D0  8955FC            mov [ebp-0x4],edx
000309D3  C60030            mov byte [eax],0x30
000309D6  EB61              jmp short 0x30a39
000309D8  C745F41C000000    mov dword [ebp-0xc],0x1c
000309DF  EB52              jmp short 0x30a33
000309E1  8B45F4            mov eax,[ebp-0xc]
000309E4  8B550C            mov edx,[ebp+0xc]
000309E7  89C1              mov ecx,eax
000309E9  D3FA              sar edx,cl
000309EB  89D0              mov eax,edx
000309ED  83E00F            and eax,byte +0xf
000309F0  8845FB            mov [ebp-0x5],al
000309F3  837DF000          cmp dword [ebp-0x10],byte +0x0
000309F7  7506              jnz 0x309ff
000309F9  807DFB00          cmp byte [ebp-0x5],0x0
000309FD  7E30              jng 0x30a2f
000309FF  C745F001000000    mov dword [ebp-0x10],0x1
00030A06  0FB645FB          movzx eax,byte [ebp-0x5]
00030A0A  83C030            add eax,byte +0x30
00030A0D  8845FB            mov [ebp-0x5],al
00030A10  807DFB39          cmp byte [ebp-0x5],0x39
00030A14  7E0A              jng 0x30a20
00030A16  0FB645FB          movzx eax,byte [ebp-0x5]
00030A1A  83C007            add eax,byte +0x7
00030A1D  8845FB            mov [ebp-0x5],al
00030A20  8B45FC            mov eax,[ebp-0x4]
00030A23  8D5001            lea edx,[eax+0x1]
00030A26  8955FC            mov [ebp-0x4],edx
00030A29  0FB655FB          movzx edx,byte [ebp-0x5]
00030A2D  8810              mov [eax],dl
00030A2F  836DF404          sub dword [ebp-0xc],byte +0x4
00030A33  837DF400          cmp dword [ebp-0xc],byte +0x0
00030A37  79A8              jns 0x309e1
00030A39  8B45FC            mov eax,[ebp-0x4]
00030A3C  C60000            mov byte [eax],0x0
00030A3F  8B4508            mov eax,[ebp+0x8]
00030A42  C9                leave
00030A43  C3                ret
00030A44  55                push ebp
00030A45  89E5              mov ebp,esp
00030A47  53                push ebx
00030A48  83EC14            sub esp,byte +0x14
00030A4B  E80AFBFFFF        call 0x3055a
00030A50  81C3B0150000      add ebx,0x15b0
00030A56  FF7508            push dword [ebp+0x8]
00030A59  8D45E8            lea eax,[ebp-0x18]
00030A5C  50                push eax
00030A5D  E82DFFFFFF        call 0x3098f
00030A62  83C408            add esp,byte +0x8
00030A65  83EC0C            sub esp,byte +0xc
00030A68  8D45E8            lea eax,[ebp-0x18]
00030A6B  50                push eax
00030A6C  E80F000000        call 0x30a80
00030A71  83C410            add esp,byte +0x10
00030A74  90                nop
00030A75  8B5DFC            mov ebx,[ebp-0x4]
00030A78  C9                leave
00030A79  C3                ret
00030A7A  6690              xchg ax,ax
00030A7C  6690              xchg ax,ax
00030A7E  6690              xchg ax,ax
00030A80  55                push ebp
00030A81  89E5              mov ebp,esp
00030A83  8B7508            mov esi,[ebp+0x8]
00030A86  8B3DA02C0300      mov edi,[dword 0x32ca0]
00030A8C  B40F              mov ah,0xf
00030A8E  AC                lodsb
00030A8F  84C0              test al,al
00030A91  7425              jz 0x30ab8
00030A93  3C0A              cmp al,0xa
00030A95  7518              jnz 0x30aaf
00030A97  50                push eax
00030A98  53                push ebx
00030A99  89F8              mov eax,edi
00030A9B  B3A0              mov bl,0xa0
00030A9D  F6F3              div bl
00030A9F  25FF000000        and eax,0xff
00030AA4  40                inc eax
00030AA5  B3A0              mov bl,0xa0
00030AA7  F6E3              mul bl
00030AA9  89C7              mov edi,eax
00030AAB  5B                pop ebx
00030AAC  58                pop eax
00030AAD  EBDF              jmp short 0x30a8e
00030AAF  65668907          mov [gs:edi],ax
00030AB3  83C702            add edi,byte +0x2
00030AB6  EBD6              jmp short 0x30a8e
00030AB8  893DA02C0300      mov [dword 0x32ca0],edi
00030ABE  5D                pop ebp
00030ABF  C3                ret
00030AC0  55                push ebp
00030AC1  89E5              mov ebp,esp
00030AC3  8B7508            mov esi,[ebp+0x8]
00030AC6  8B3DA02C0300      mov edi,[dword 0x32ca0]
00030ACC  8A650C            mov ah,[ebp+0xc]
00030ACF  AC                lodsb
00030AD0  84C0              test al,al
00030AD2  7425              jz 0x30af9
00030AD4  3C0A              cmp al,0xa
00030AD6  7518              jnz 0x30af0
00030AD8  50                push eax
00030AD9  53                push ebx
00030ADA  89F8              mov eax,edi
00030ADC  B3A0              mov bl,0xa0
00030ADE  F6F3              div bl
00030AE0  25FF000000        and eax,0xff
00030AE5  40                inc eax
00030AE6  B3A0              mov bl,0xa0
00030AE8  F6E3              mul bl
00030AEA  89C7              mov edi,eax
00030AEC  5B                pop ebx
00030AED  58                pop eax
00030AEE  EBDF              jmp short 0x30acf
00030AF0  65668907          mov [gs:edi],ax
00030AF4  83C702            add edi,byte +0x2
00030AF7  EBD6              jmp short 0x30acf
00030AF9  893DA02C0300      mov [dword 0x32ca0],edi
00030AFF  5D                pop ebp
00030B00  C3                ret
00030B01  8B542404          mov edx,[esp+0x4]
00030B05  8A442408          mov al,[esp+0x8]
00030B09  EE                out dx,al
00030B0A  90                nop
00030B0B  90                nop
00030B0C  C3                ret
00030B0D  8B542404          mov edx,[esp+0x4]
00030B11  31C0              xor eax,eax
00030B13  EC                in al,dx
00030B14  90                nop
00030B15  90                nop
00030B16  C3                ret
00030B17  6690              xchg ax,ax
00030B19  6690              xchg ax,ax
00030B1B  6690              xchg ax,ax
00030B1D  6690              xchg ax,ax
00030B1F  90                nop
00030B20  55                push ebp
00030B21  89E5              mov ebp,esp
00030B23  56                push esi
00030B24  57                push edi
00030B25  51                push ecx
00030B26  8B7D08            mov edi,[ebp+0x8]
00030B29  8B750C            mov esi,[ebp+0xc]
00030B2C  8B4D10            mov ecx,[ebp+0x10]
00030B2F  83F900            cmp ecx,byte +0x0
00030B32  740B              jz 0x30b3f
00030B34  3E8A06            mov al,[ds:esi]
00030B37  46                inc esi
00030B38  268807            mov [es:edi],al
00030B3B  47                inc edi
00030B3C  49                dec ecx
00030B3D  EBF0              jmp short 0x30b2f
00030B3F  8B4508            mov eax,[ebp+0x8]
00030B42  59                pop ecx
00030B43  5F                pop edi
00030B44  5E                pop esi
00030B45  89EC              mov esp,ebp
00030B47  5D                pop ebp
00030B48  C3                ret
00030B49  0000              add [eax],al
00030B4B  000A              add [edx],cl
00030B4D  0A0A              or cl,[edx]
00030B4F  0A0A              or cl,[edx]
00030B51  0A0A              or cl,[edx]
00030B53  0A0A              or cl,[edx]
00030B55  0A0A              or cl,[edx]
00030B57  0A0A              or cl,[edx]
00030B59  0A0A              or cl,[edx]
00030B5B  2D2D2D2D2D        sub eax,0x2d2d2d2d
00030B60  226373            and ah,[ebx+0x73]
00030B63  7461              jz 0x30bc6
00030B65  7274              jc 0x30bdb
00030B67  2220              and ah,[eax]
00030B69  626567            bound esp,[ebp+0x67]
00030B6C  696E732D2D2D2D    imul ebp,[esi+0x73],dword 0x2d2d2d2d
00030B73  2D0A002D2D        sub eax,0x2d2d000a
00030B78  2D2D2D2263        sub eax,0x63222d2d
00030B7D  7374              jnc 0x30bf3
00030B7F  61                popa
00030B80  7274              jc 0x30bf6
00030B82  2220              and ah,[eax]
00030B84  656E              gs outsb
00030B86  64732D            fs jnc 0x30bb6
00030B89  2D2D2D2D0A        sub eax,0xa2d2d2d
00030B8E  0000              add [eax],al
00030B90  2000              and [eax],al
00030B92  45                inc ebp
00030B93  7863              js 0x30bf8
00030B95  657074            gs jo 0x30c0c
00030B98  696F6E21202D2D    imul ebp,[edi+0x6e],dword 0x2d2d2021
00030B9F  3E2000            and [ds:eax],al
00030BA2  0A0A              or cl,[edx]
00030BA4  004546            add [ebp+0x46],al
00030BA7  4C                dec esp
00030BA8  41                inc ecx
00030BA9  47                inc edi
00030BAA  53                push ebx
00030BAB  3A20              cmp ah,[eax]
00030BAD  004353            add [ebx+0x53],al
00030BB0  3A20              cmp ah,[eax]
00030BB2  004549            add [ebp+0x49],al
00030BB5  50                push eax
00030BB6  3A20              cmp ah,[eax]
00030BB8  004572            add [ebp+0x72],al
00030BBB  726F              jc 0x30c2c
00030BBD  7220              jc 0x30bdf
00030BBF  636F64            arpl [edi+0x64],bp
00030BC2  653A20            cmp ah,[gs:eax]
00030BC5  0023              add [ebx],ah
00030BC7  44                inc esp
00030BC8  45                inc ebp
00030BC9  20446976          and [ecx+ebp*2+0x76],al
00030BCD  696465204572726F  imul esp,[ebp+0x20],dword 0x6f727245
00030BD5  7200              jc 0x30bd7
00030BD7  23444220          and eax,[edx+eax*2+0x20]
00030BDB  52                push edx
00030BDC  45                inc ebp
00030BDD  53                push ebx
00030BDE  45                inc ebp
00030BDF  52                push edx
00030BE0  56                push esi
00030BE1  45                inc ebp
00030BE2  44                inc esp
00030BE3  002D2D20204E      add [dword 0x4e20202d],ch
00030BE9  4D                dec ebp
00030BEA  49                dec ecx
00030BEB  20496E            and [ecx+0x6e],cl
00030BEE  7465              jz 0x30c55
00030BF0  7272              jc 0x30c64
00030BF2  7570              jnz 0x30c64
00030BF4  7400              jz 0x30bf6
00030BF6  234250            and eax,[edx+0x50]
00030BF9  204272            and [edx+0x72],al
00030BFC  6561              gs popa
00030BFE  6B706F69          imul esi,[eax+0x6f],byte +0x69
00030C02  6E                outsb
00030C03  7400              jz 0x30c05
00030C05  234F46            and ecx,[edi+0x46]
00030C08  204F76            and [edi+0x76],cl
00030C0B  657266            gs jc 0x30c74
00030C0E  6C                insb
00030C0F  6F                outsd
00030C10  7700              ja 0x30c12
00030C12  234252            and eax,[edx+0x52]
00030C15  20424F            and [edx+0x4f],al
00030C18  55                push ebp
00030C19  4E                dec esi
00030C1A  44                inc esp
00030C1B  205261            and [edx+0x61],dl
00030C1E  6E                outsb
00030C1F  6765204578        and [gs:di+0x78],al
00030C24  636565            arpl [ebp+0x65],sp
00030C27  6465640000        add [fs:eax],al
00030C2C  235544            and edx,[ebp+0x44]
00030C2F  20496E            and [ecx+0x6e],cl
00030C32  7661              jna 0x30c95
00030C34  6C                insb
00030C35  6964204F70636F64  imul esp,[eax+0x4f],dword 0x646f6370
00030C3D  652028            and [gs:eax],ch
00030C40  55                push ebp
00030C41  6E                outsb
00030C42  646566696E656420  imul bp,[gs:esi+0x65],word 0x2064
00030C4A  4F                dec edi
00030C4B  7063              jo 0x30cb0
00030C4D  6F                outsd
00030C4E  64652900          sub [gs:eax],eax
00030C52  0000              add [eax],al
00030C54  234E4D            and ecx,[esi+0x4d]
00030C57  20446576          and [ebp+0x76],al
00030C5B  696365204E6F74    imul esp,[ebx+0x65],dword 0x746f4e20
00030C62  204176            and [ecx+0x76],al
00030C65  61                popa
00030C66  696C61626C652028  imul ebp,[ecx+0x62],dword 0x2820656c
00030C6E  4E                dec esi
00030C6F  6F                outsd
00030C70  204D61            and [ebp+0x61],cl
00030C73  7468              jz 0x30cdd
00030C75  20436F            and [ebx+0x6f],al
00030C78  7072              jo 0x30cec
00030C7A  6F                outsd
00030C7B  636573            arpl [ebp+0x73],sp
00030C7E  736F              jnc 0x30cef
00030C80  7229              jc 0x30cab
00030C82  0023              add [ebx],ah
00030C84  44                inc esp
00030C85  46                inc esi
00030C86  20446F75          and [edi+ebp*2+0x75],al
00030C8A  626C6520          bound ebp,[ebp+0x20]
00030C8E  46                inc esi
00030C8F  61                popa
00030C90  756C              jnz 0x30cfe
00030C92  7400              jz 0x30c94
00030C94  2020              and [eax],ah
00030C96  2020              and [eax],ah
00030C98  43                inc ebx
00030C99  6F                outsd
00030C9A  7072              jo 0x30d0e
00030C9C  6F                outsd
00030C9D  636573            arpl [ebp+0x73],sp
00030CA0  736F              jnc 0x30d11
00030CA2  7220              jc 0x30cc4
00030CA4  53                push ebx
00030CA5  65676D            gs a16 insd
00030CA8  656E              gs outsb
00030CAA  7420              jz 0x30ccc
00030CAC  4F                dec edi
00030CAD  7665              jna 0x30d14
00030CAF  7272              jc 0x30d23
00030CB1  756E              jnz 0x30d21
00030CB3  2028              and [eax],ch
00030CB5  7265              jc 0x30d1c
00030CB7  7365              jnc 0x30d1e
00030CB9  7276              jc 0x30d31
00030CBB  65642900          sub [fs:eax],eax
00030CBF  23545320          and edx,[ebx+edx*2+0x20]
00030CC3  49                dec ecx
00030CC4  6E                outsb
00030CC5  7661              jna 0x30d28
00030CC7  6C                insb
00030CC8  6964205453530023  imul esp,[eax+0x54],dword 0x23005353
00030CD0  4E                dec esi
00030CD1  50                push eax
00030CD2  205365            and [ebx+0x65],dl
00030CD5  676D              a16 insd
00030CD7  656E              gs outsb
00030CD9  7420              jz 0x30cfb
00030CDB  4E                dec esi
00030CDC  6F                outsd
00030CDD  7420              jz 0x30cff
00030CDF  50                push eax
00030CE0  7265              jc 0x30d47
00030CE2  7365              jnc 0x30d49
00030CE4  6E                outsb
00030CE5  7400              jz 0x30ce7
00030CE7  235353            and edx,[ebx+0x53]
00030CEA  205374            and [ebx+0x74],dl
00030CED  61                popa
00030CEE  636B2D            arpl [ebx+0x2d],bp
00030CF1  53                push ebx
00030CF2  65676D            gs a16 insd
00030CF5  656E              gs outsb
00030CF7  7420              jz 0x30d19
00030CF9  46                inc esi
00030CFA  61                popa
00030CFB  756C              jnz 0x30d69
00030CFD  7400              jz 0x30cff
00030CFF  234750            and eax,[edi+0x50]
00030D02  204765            and [edi+0x65],al
00030D05  6E                outsb
00030D06  657261            gs jc 0x30d6a
00030D09  6C                insb
00030D0A  205072            and [eax+0x72],dl
00030D0D  6F                outsd
00030D0E  7465              jz 0x30d75
00030D10  6374696F          arpl [ecx+ebp*2+0x6f],si
00030D14  6E                outsb
00030D15  0023              add [ebx],ah
00030D17  50                push eax
00030D18  46                inc esi
00030D19  205061            and [eax+0x61],dl
00030D1C  6765204661        and [gs:bp+0x61],al
00030D21  756C              jnz 0x30d8f
00030D23  7400              jz 0x30d25
00030D25  0000              add [eax],al
00030D27  002D2D202028      add [dword 0x2820202d],ch
00030D2D  49                dec ecx
00030D2E  6E                outsb
00030D2F  7465              jz 0x30d96
00030D31  6C                insb
00030D32  207265            and [edx+0x65],dh
00030D35  7365              jnc 0x30d9c
00030D37  7276              jc 0x30daf
00030D39  65642E20446F20    and [cs:edi+ebp*2+0x20],al
00030D40  6E                outsb
00030D41  6F                outsd
00030D42  7420              jz 0x30d64
00030D44  7573              jnz 0x30db9
00030D46  652E2900          sub [cs:eax],eax
00030D4A  0000              add [eax],al
00030D4C  234D46            and ecx,[ebp+0x46]
00030D4F  207838            and [eax+0x38],bh
00030D52  37                aaa
00030D53  204650            and [esi+0x50],al
00030D56  55                push ebp
00030D57  20466C            and [esi+0x6c],al
00030D5A  6F                outsd
00030D5B  61                popa
00030D5C  7469              jz 0x30dc7
00030D5E  6E                outsb
00030D5F  672D506F696E      sub eax,0x6e696f50
00030D65  7420              jz 0x30d87
00030D67  45                inc ebp
00030D68  7272              jc 0x30ddc
00030D6A  6F                outsd
00030D6B  7220              jc 0x30d8d
00030D6D  284D61            sub [ebp+0x61],cl
00030D70  7468              jz 0x30dda
00030D72  204661            and [esi+0x61],al
00030D75  756C              jnz 0x30de3
00030D77  7429              jz 0x30da2
00030D79  0023              add [ebx],ah
00030D7B  41                inc ecx
00030D7C  43                inc ebx
00030D7D  20416C            and [ecx+0x6c],al
00030D80  69676E6D656E74    imul esp,[edi+0x6e],dword 0x746e656d
00030D87  204368            and [ebx+0x68],al
00030D8A  65636B00          arpl [gs:ebx+0x0],bp
00030D8E  234D43            and ecx,[ebp+0x43]
00030D91  204D61            and [ebp+0x61],cl
00030D94  636869            arpl [eax+0x69],bp
00030D97  6E                outsb
00030D98  65204368          and [gs:ebx+0x68],al
00030D9C  65636B00          arpl [gs:ebx+0x0],bp
00030DA0  235846            and ebx,[eax+0x46]
00030DA3  205349            and [ebx+0x49],dl
00030DA6  4D                dec ebp
00030DA7  44                inc esp
00030DA8  20466C            and [esi+0x6c],al
00030DAB  6F                outsd
00030DAC  61                popa
00030DAD  7469              jz 0x30e18
00030DAF  6E                outsb
00030DB0  672D506F696E      sub eax,0x6e696f50
00030DB6  7420              jz 0x30dd8
00030DB8  45                inc ebp
00030DB9  7863              js 0x30e1e
00030DBB  657074            gs jo 0x30e32
00030DBE  696F6E00000014    imul ebp,[edi+0x6e],dword 0x14000000
00030DC5  0000              add [eax],al
00030DC7  0000              add [eax],al
00030DC9  0000              add [eax],al
00030DCB  0001              add [ecx],al
00030DCD  7A52              jpe 0x30e21
00030DCF  0001              add [ecx],al
00030DD1  7C08              jl 0x30ddb
00030DD3  011B              add [ebx],ebx
00030DD5  0C04              or al,0x4
00030DD7  0488              add al,0x88
00030DD9  0100              add [eax],eax
00030DDB  0020              add [eax],ah
00030DDD  0000              add [eax],al
00030DDF  001C00            add [eax+eax],bl
00030DE2  0000              add [eax],al
00030DE4  B3F6              mov bl,0xf6
00030DE6  FF                db 0xff
00030DE7  FFC3              inc ebx
00030DE9  0000              add [eax],al
00030DEB  0000              add [eax],al
00030DED  41                inc ecx
00030DEE  0E                push cs
00030DEF  088502420D05      or [ebp+0x50d4202],al
00030DF5  44                inc esp
00030DF6  830302            add dword [ebx],byte +0x2
00030DF9  BBC5C30C04        mov ebx,0x40cc3c5
00030DFE  0400              add al,0x0
00030E00  1000              adc [eax],al
00030E02  0000              add [eax],al
00030E04  40                inc eax
00030E05  0000              add [eax],al
00030E07  0052F7            add [edx-0x9],dl
00030E0A  FF                db 0xff
00030E0B  FF0400            inc dword [eax+eax]
00030E0E  0000              add [eax],al
00030E10  0000              add [eax],al
00030E12  0000              add [eax],al
00030E14  2000              and [eax],al
00030E16  0000              add [eax],al
00030E18  54                push esp
00030E19  0000              add [eax],al
00030E1B  0042F7            add [edx-0x9],al
00030E1E  FF                db 0xff
00030E1F  FFC3              inc ebx
00030E21  0000              add [eax],al
00030E23  0000              add [eax],al
00030E25  41                inc ecx
00030E26  0E                push cs
00030E27  088502420D05      or [ebp+0x50d4202],al
00030E2D  44                inc esp
00030E2E  830302            add dword [ebx],byte +0x2
00030E31  BBC5C30C04        mov ebx,0x40cc3c5
00030E36  0400              add al,0x0
00030E38  2000              and [eax],al
00030E3A  0000              add [eax],al
00030E3C  7800              js 0x30e3e
00030E3E  0000              add [eax],al
00030E40  E1F7              loope 0x30e39
00030E42  FF                db 0xff
00030E43  FF9D01000000      call far [ebp+0x1]
00030E49  41                inc ecx
00030E4A  0E                push cs
00030E4B  088502420D05      or [ebp+0x50d4202],al
00030E51  44                inc esp
00030E52  830303            add dword [ebx],byte +0x3
00030E55  95                xchg eax,ebp
00030E56  01C5              add ebp,eax
00030E58  C3                ret
00030E59  0C04              or al,0x4
00030E5B  0420              add al,0x20
00030E5D  0000              add [eax],al
00030E5F  009C0000005AF9    add [eax+eax-0x6a60000],bl
00030E66  FF                db 0xff
00030E67  FF8300000000      inc dword [ebx+0x0]
00030E6D  41                inc ecx
00030E6E  0E                push cs
00030E6F  088502420D05      or [ebp+0x50d4202],al
00030E75  44                inc esp
00030E76  830302            add dword [ebx],byte +0x2
00030E79  7AC3              jpe 0x30e3e
00030E7B  41                inc ecx
00030E7C  C50C04            lds ecx,[esp+eax]
00030E7F  042C              add al,0x2c
00030E81  0000              add [eax],al
00030E83  00C0              add al,al
00030E85  0000              add [eax],al
00030E87  00B9F9FFFF4A      add [ecx+0x4afffff9],bh
00030E8D  0100              add [eax],eax
00030E8F  0000              add [eax],al
00030E91  41                inc ecx
00030E92  0E                push cs
00030E93  088502420D05      or [ebp+0x50d4202],al
00030E99  46                inc esi
00030E9A  8703              xchg eax,[ebx]
00030E9C  860483            xchg al,[ebx+eax*4]
00030E9F  05033D01C3        add eax,0xc3013d03
00030EA4  41                inc ecx
00030EA5  C641C741          mov byte [ecx-0x39],0x41
00030EA9  C50C04            lds ecx,[esp+eax]
00030EAC  0400              add al,0x0
00030EAE  0000              add [eax],al
00030EB0  1000              adc [eax],al
00030EB2  0000              add [eax],al
00030EB4  F00000            lock add [eax],al
00030EB7  00D3              add bl,dl
00030EB9  FA                cli
00030EBA  FF                db 0xff
00030EBB  FF0400            inc dword [eax+eax]
00030EBE  0000              add [eax],al
00030EC0  0000              add [eax],al
00030EC2  0000              add [eax],al
00030EC4  1C00              sbb al,0x0
00030EC6  0000              add [eax],al
00030EC8  0401              add al,0x1
00030ECA  0000              add [eax],al
00030ECC  C3                ret
00030ECD  FA                cli
00030ECE  FF                db 0xff
00030ECF  FFB500000000      push dword [ebp+0x0]
00030ED5  41                inc ecx
00030ED6  0E                push cs
00030ED7  088502420D05      or [ebp+0x50d4202],al
00030EDD  02B1C50C0404      add dh,[ecx+0x4040cc5]
00030EE3  0020              add [eax],ah
00030EE5  0000              add [eax],al
00030EE7  002401            add [ecx+eax],ah
00030EEA  0000              add [eax],al
00030EEC  58                pop eax
00030EED  FB                sti
00030EEE  FF                db 0xff
00030EEF  FF36              push dword [esi]
00030EF1  0000              add [eax],al
00030EF3  0000              add [eax],al
00030EF5  41                inc ecx
00030EF6  0E                push cs
00030EF7  088502420D05      or [ebp+0x50d4202],al
00030EFD  44                inc esp
00030EFE  83036E            add dword [ebx],byte +0x6e
00030F01  C5                db 0xc5
00030F02  C3                ret
00030F03  0C04              or al,0x4
00030F05  0400              add al,0x0
00030F07  0000              add [eax],al
00030F09  0000              add [eax],al
00030F0B  0000              add [eax],al
00030F0D  0000              add [eax],al
00030F0F  0000              add [eax],al
00030F11  0000              add [eax],al
00030F13  0000              add [eax],al
00030F15  0000              add [eax],al
00030F17  0000              add [eax],al
00030F19  0000              add [eax],al
00030F1B  0000              add [eax],al
00030F1D  0000              add [eax],al
00030F1F  0000              add [eax],al
00030F21  0000              add [eax],al
00030F23  0000              add [eax],al
00030F25  0000              add [eax],al
00030F27  0000              add [eax],al
00030F29  0000              add [eax],al
00030F2B  0000              add [eax],al
00030F2D  0000              add [eax],al
00030F2F  0000              add [eax],al
00030F31  0000              add [eax],al
00030F33  0000              add [eax],al
00030F35  0000              add [eax],al
00030F37  0000              add [eax],al
00030F39  0000              add [eax],al
00030F3B  0000              add [eax],al
00030F3D  0000              add [eax],al
00030F3F  0000              add [eax],al
00030F41  0000              add [eax],al
00030F43  0000              add [eax],al
00030F45  0000              add [eax],al
00030F47  0000              add [eax],al
00030F49  0000              add [eax],al
00030F4B  0000              add [eax],al
00030F4D  0000              add [eax],al
00030F4F  0000              add [eax],al
00030F51  0000              add [eax],al
00030F53  0000              add [eax],al
00030F55  0000              add [eax],al
00030F57  0000              add [eax],al
00030F59  0000              add [eax],al
00030F5B  0000              add [eax],al
00030F5D  0000              add [eax],al
00030F5F  0000              add [eax],al
00030F61  0000              add [eax],al
00030F63  0000              add [eax],al
00030F65  0000              add [eax],al
00030F67  0000              add [eax],al
00030F69  0000              add [eax],al
00030F6B  0000              add [eax],al
00030F6D  0000              add [eax],al
00030F6F  0000              add [eax],al
00030F71  0000              add [eax],al
00030F73  0000              add [eax],al
00030F75  0000              add [eax],al
00030F77  0000              add [eax],al
00030F79  0000              add [eax],al
00030F7B  0000              add [eax],al
00030F7D  0000              add [eax],al
00030F7F  0000              add [eax],al
00030F81  0000              add [eax],al
00030F83  0000              add [eax],al
00030F85  0000              add [eax],al
00030F87  0000              add [eax],al
00030F89  0000              add [eax],al
00030F8B  0000              add [eax],al
00030F8D  0000              add [eax],al
00030F8F  0000              add [eax],al
00030F91  0000              add [eax],al
00030F93  0000              add [eax],al
00030F95  0000              add [eax],al
00030F97  0000              add [eax],al
00030F99  0000              add [eax],al
00030F9B  0000              add [eax],al
00030F9D  0000              add [eax],al
00030F9F  0000              add [eax],al
00030FA1  0000              add [eax],al
00030FA3  0000              add [eax],al
00030FA5  0000              add [eax],al
00030FA7  0000              add [eax],al
00030FA9  0000              add [eax],al
00030FAB  0000              add [eax],al
00030FAD  0000              add [eax],al
00030FAF  0000              add [eax],al
00030FB1  0000              add [eax],al
00030FB3  0000              add [eax],al
00030FB5  0000              add [eax],al
00030FB7  0000              add [eax],al
00030FB9  0000              add [eax],al
00030FBB  0000              add [eax],al
00030FBD  0000              add [eax],al
00030FBF  0000              add [eax],al
00030FC1  0000              add [eax],al
00030FC3  0000              add [eax],al
00030FC5  0000              add [eax],al
00030FC7  0000              add [eax],al
00030FC9  0000              add [eax],al
00030FCB  0000              add [eax],al
00030FCD  0000              add [eax],al
00030FCF  0000              add [eax],al
00030FD1  0000              add [eax],al
00030FD3  0000              add [eax],al
00030FD5  0000              add [eax],al
00030FD7  0000              add [eax],al
00030FD9  0000              add [eax],al
00030FDB  0000              add [eax],al
00030FDD  0000              add [eax],al
00030FDF  0000              add [eax],al
00030FE1  0000              add [eax],al
00030FE3  0000              add [eax],al
00030FE5  0000              add [eax],al
00030FE7  0000              add [eax],al
00030FE9  0000              add [eax],al
00030FEB  0000              add [eax],al
00030FED  0000              add [eax],al
00030FEF  0000              add [eax],al
00030FF1  0000              add [eax],al
00030FF3  0000              add [eax],al
00030FF5  0000              add [eax],al
00030FF7  0000              add [eax],al
00030FF9  0000              add [eax],al
00030FFB  0000              add [eax],al
00030FFD  0000              add [eax],al
00030FFF  0000              add [eax],al
00031001  0000              add [eax],al
00031003  0000              add [eax],al
00031005  0000              add [eax],al
00031007  0000              add [eax],al
00031009  0000              add [eax],al
0003100B  0000              add [eax],al
0003100D  0000              add [eax],al
0003100F  0000              add [eax],al
00031011  0000              add [eax],al
00031013  0000              add [eax],al
00031015  0000              add [eax],al
00031017  0000              add [eax],al
00031019  0000              add [eax],al
0003101B  0000              add [eax],al
0003101D  0000              add [eax],al
0003101F  00C6              add dh,al
00031021  0B03              or eax,[ebx]
00031023  00D7              add bh,dl
00031025  0B03              or eax,[ebx]
00031027  00E4              add ah,ah
00031029  0B03              or eax,[ebx]
0003102B  00F6              add dh,dh
0003102D  0B03              or eax,[ebx]
0003102F  00050C030012      add [dword 0x1200030c],al
00031035  0C03              or al,0x3
00031037  002C0C            add [esp+ecx],ch
0003103A  0300              add eax,[eax]
0003103C  54                push esp
0003103D  0C03              or al,0x3
0003103F  00830C030094      add [ebx-0x6bfffcf4],al
00031045  0C03              or al,0x3
00031047  00BF0C0300CF      add [edi-0x30fffcf4],bh
0003104D  0C03              or al,0x3
0003104F  00E7              add bh,ah
00031051  0C03              or al,0x3
00031053  00FF              add bh,bh
00031055  0C03              or al,0x3
00031057  0016              add [esi],dl
00031059  0D0300280D        or eax,0xd280003
0003105E  0300              add eax,[eax]
00031060  4C                dec esp
00031061  0D03007A0D        or eax,0xd7a0003
00031066  0300              add eax,[eax]
00031068  8E0D0300A00D      mov cs,[dword 0xda00003]
0003106E  0300              add eax,[eax]
00031070  47                inc edi
00031071  43                inc ebx
00031072  43                inc ebx
00031073  3A20              cmp ah,[eax]
00031075  285562            sub [ebp+0x62],dl
00031078  756E              jnz 0x310e8
0003107A  7475              jz 0x310f1
0003107C  2037              and [edi],dh
0003107E  2E342E            cs xor al,0x2e
00031081  302D31756275      xor [dword 0x75627531],ch
00031087  6E                outsb
00031088  7475              jz 0x310ff
0003108A  317E31            xor [esi+0x31],edi
0003108D  382E              cmp [esi],ch
0003108F  30342E            xor [esi+ebp],dh
00031092  3129              xor [ecx],ebp
00031094  2037              and [edi],dh
00031096  2E342E            cs xor al,0x2e
00031099  3000              xor [eax],al
0003109B  002E              add [esi],ch
0003109D  7368              jnc 0x31107
0003109F  7374              jnc 0x31115
000310A1  7274              jc 0x31117
000310A3  61                popa
000310A4  6200              bound eax,[eax]
000310A6  2E7465            cs jz 0x3110e
000310A9  7874              js 0x3111f
000310AB  002E              add [esi],ch
000310AD  726F              jc 0x3111e
000310AF  6461              fs popa
000310B1  7461              jz 0x31114
000310B3  002E              add [esi],ch
000310B5  65685F667261      gs push dword 0x6172665f
000310BB  6D                insd
000310BC  65002E            add [gs:esi],ch
000310BF  676F              a16 outsd
000310C1  742E              jz 0x310f1
000310C3  706C              jo 0x31131
000310C5  7400              jz 0x310c7
000310C7  2E6461            fs popa
000310CA  7461              jz 0x3112d
000310CC  002E              add [esi],ch
000310CE  627373            bound esi,[ebx+0x73]
000310D1  002E              add [esi],ch
000310D3  636F6D            arpl [edi+0x6d],bp
000310D6  6D                insd
000310D7  656E              gs outsb
000310D9  7400              jz 0x310db
000310DB  0000              add [eax],al
000310DD  0000              add [eax],al
000310DF  0000              add [eax],al
000310E1  0000              add [eax],al
000310E3  0000              add [eax],al
000310E5  0000              add [eax],al
000310E7  0000              add [eax],al
000310E9  0000              add [eax],al
000310EB  0000              add [eax],al
000310ED  0000              add [eax],al
000310EF  0000              add [eax],al
000310F1  0000              add [eax],al
000310F3  0000              add [eax],al
000310F5  0000              add [eax],al
000310F7  0000              add [eax],al
000310F9  0000              add [eax],al
000310FB  0000              add [eax],al
000310FD  0000              add [eax],al
000310FF  0000              add [eax],al
00031101  0000              add [eax],al
00031103  000B              add [ebx],cl
00031105  0000              add [eax],al
00031107  0001              add [ecx],al
00031109  0000              add [eax],al
0003110B  0006              add [esi],al
0003110D  0000              add [eax],al
0003110F  0000              add [eax],al
00031111  0403              add al,0x3
00031113  0000              add [eax],al
00031115  0400              add al,0x0
00031117  004907            add [ecx+0x7],cl
0003111A  0000              add [eax],al
0003111C  0000              add [eax],al
0003111E  0000              add [eax],al
00031120  0000              add [eax],al
00031122  0000              add [eax],al
00031124  1000              adc [eax],al
00031126  0000              add [eax],al
00031128  0000              add [eax],al
0003112A  0000              add [eax],al
0003112C  1100              adc [eax],eax
0003112E  0000              add [eax],al
00031130  0100              add [eax],eax
00031132  0000              add [eax],al
00031134  0200              add al,[eax]
00031136  0000              add [eax],al
00031138  4C                dec esp
00031139  0B03              or eax,[ebx]
0003113B  004C0B00          add [ebx+ecx+0x0],cl
0003113F  007602            add [esi+0x2],dh
00031142  0000              add [eax],al
00031144  0000              add [eax],al
00031146  0000              add [eax],al
00031148  0000              add [eax],al
0003114A  0000              add [eax],al
0003114C  0400              add al,0x0
0003114E  0000              add [eax],al
00031150  0000              add [eax],al
00031152  0000              add [eax],al
00031154  1900              sbb [eax],eax
00031156  0000              add [eax],al
00031158  0100              add [eax],eax
0003115A  0000              add [eax],al
0003115C  0200              add al,[eax]
0003115E  0000              add [eax],al
00031160  C40D0300C40D      les ecx,[dword 0xdc40003]
00031166  0000              add [eax],al
00031168  44                inc esp
00031169  0100              add [eax],eax
0003116B  0000              add [eax],al
0003116D  0000              add [eax],al
0003116F  0000              add [eax],al
00031171  0000              add [eax],al
00031173  000400            add [eax+eax],al
00031176  0000              add [eax],al
00031178  0000              add [eax],al
0003117A  0000              add [eax],al
0003117C  2300              and eax,[eax]
0003117E  0000              add [eax],al
00031180  0100              add [eax],eax
00031182  0000              add [eax],al
00031184  0300              add eax,[eax]
00031186  0000              add [eax],al
00031188  0020              add [eax],ah
0003118A  0300              add eax,[eax]
0003118C  0010              add [eax],dl
0003118E  0000              add [eax],al
00031190  0C00              or al,0x0
00031192  0000              add [eax],al
00031194  0000              add [eax],al
00031196  0000              add [eax],al
00031198  0000              add [eax],al
0003119A  0000              add [eax],al
0003119C  0400              add al,0x0
0003119E  0000              add [eax],al
000311A0  0400              add al,0x0
000311A2  0000              add [eax],al
000311A4  2C00              sub al,0x0
000311A6  0000              add [eax],al
000311A8  0100              add [eax],eax
000311AA  0000              add [eax],al
000311AC  0300              add eax,[eax]
000311AE  0000              add [eax],al
000311B0  2020              and [eax],ah
000311B2  0300              add eax,[eax]
000311B4  2010              and [eax],dl
000311B6  0000              add [eax],al
000311B8  50                push eax
000311B9  0000              add [eax],al
000311BB  0000              add [eax],al
000311BD  0000              add [eax],al
000311BF  0000              add [eax],al
000311C1  0000              add [eax],al
000311C3  0020              add [eax],ah
000311C5  0000              add [eax],al
000311C7  0000              add [eax],al
000311C9  0000              add [eax],al
000311CB  0032              add [edx],dh
000311CD  0000              add [eax],al
000311CF  0008              add [eax],cl
000311D1  0000              add [eax],al
000311D3  0003              add [ebx],al
000311D5  0000              add [eax],al
000311D7  008020030070      add [eax+0x70000320],al
000311DD  1000              adc [eax],al
000311DF  004014            add [eax+0x14],al
000311E2  0000              add [eax],al
000311E4  0000              add [eax],al
000311E6  0000              add [eax],al
000311E8  0000              add [eax],al
000311EA  0000              add [eax],al
000311EC  2000              and [eax],al
000311EE  0000              add [eax],al
000311F0  0000              add [eax],al
000311F2  0000              add [eax],al
000311F4  37                aaa
000311F5  0000              add [eax],al
000311F7  0001              add [ecx],al
000311F9  0000              add [eax],al
000311FB  0030              add [eax],dh
000311FD  0000              add [eax],al
000311FF  0000              add [eax],al
00031201  0000              add [eax],al
00031203  007010            add [eax+0x10],dh
00031206  0000              add [eax],al
00031208  2B00              sub eax,[eax]
0003120A  0000              add [eax],al
0003120C  0000              add [eax],al
0003120E  0000              add [eax],al
00031210  0000              add [eax],al
00031212  0000              add [eax],al
00031214  0100              add [eax],eax
00031216  0000              add [eax],al
00031218  0100              add [eax],eax
0003121A  0000              add [eax],al
0003121C  0100              add [eax],eax
0003121E  0000              add [eax],al
00031220  0300              add eax,[eax]
00031222  0000              add [eax],al
00031224  0000              add [eax],al
00031226  0000              add [eax],al
00031228  0000              add [eax],al
0003122A  0000              add [eax],al
0003122C  9B1000            wait adc [eax],al
0003122F  004000            add [eax+0x0],al
00031232  0000              add [eax],al
00031234  0000              add [eax],al
00031236  0000              add [eax],al
00031238  0000              add [eax],al
0003123A  0000              add [eax],al
0003123C  0100              add [eax],eax
0003123E  0000              add [eax],al
00031240  0000              add [eax],al
00031242  0000              add [eax],al
