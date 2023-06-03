;;;;;;;;;;;;;;;;;;;;
; scancode constants
;;;;;;;;;;;;;;;;;;;;

(defq
	+sc_unknown 0

	+sc_a 4
	+sc_b 5
	+sc_c 6
	+sc_d 7
	+sc_e 8
	+sc_f 9
	+sc_g 10
	+sc_h 11
	+sc_i 12
	+sc_j 13
	+sc_k 14
	+sc_l 15
	+sc_m 16
	+sc_n 17
	+sc_o 18
	+sc_p 19
	+sc_q 20
	+sc_r 21
	+sc_s 22
	+sc_t 23
	+sc_u 24
	+sc_v 25
	+sc_w 26
	+sc_x 27
	+sc_y 28
	+sc_z 29

	+sc_1 30
	+sc_2 31
	+sc_3 32
	+sc_4 33
	+sc_5 34
	+sc_6 35
	+sc_7 36
	+sc_8 37
	+sc_9 38
	+sc_0 39

	+sc_return 40
	+sc_escape 41
	+sc_backspace 42
	+sc_tab 43
	+sc_space 44

	+sc_minus 45
	+sc_equals 46
	+sc_leftbracket 47
	+sc_rightbracket 48
	+sc_backslash 49
	+sc_nonushash 50
	+sc_semicolon 51
	+sc_apostrophe 52
	+sc_grave 53
	+sc_comma 54
	+sc_period 55
	+sc_slash 56

	+sc_capslock 57

	+sc_f1 58
	+sc_f2 59
	+sc_f3 60
	+sc_f4 61
	+sc_f5 62
	+sc_f6 63
	+sc_f7 64
	+sc_f8 65
	+sc_f9 66
	+sc_f10 67
	+sc_f11 68
	+sc_f12 69

	+sc_printscreen 70
	+sc_scrolllock 71
	+sc_pause 72
	+sc_insert 73
	+sc_home 74
	+sc_pageup 75
	+sc_delete 76
	+sc_end 77
	+sc_pagedown 78
	+sc_right 79
	+sc_left 80
	+sc_down 81
	+sc_up 82

	+sc_numlockclear 83
	+sc_kp_divide 84
	+sc_kp_multiply 85
	+sc_kp_minus 86
	+sc_kp_plus 87
	+sc_kp_enter 88
	+sc_kp_1 89
	+sc_kp_2 90
	+sc_kp_3 91
	+sc_kp_4 92
	+sc_kp_5 93
	+sc_kp_6 94
	+sc_kp_7 95
	+sc_kp_8 96
	+sc_kp_9 97
	+sc_kp_0 98
	+sc_kp_period 99

	+sc_nonusbackslash 100
	+sc_application 101
	+sc_power 102
	+sc_kp_equals 103
	+sc_f13 104
	+sc_f14 105
	+sc_f15 106
	+sc_f16 107
	+sc_f17 108
	+sc_f18 109
	+sc_f19 110
	+sc_f20 111
	+sc_f21 112
	+sc_f22 113
	+sc_f23 114
	+sc_f24 115
	+sc_execute 116
	+sc_help 117
	+sc_menu 118
	+sc_select 119
	+sc_stop 120
	+sc_again 121
	+sc_undo 122
	+sc_cut 123
	+sc_copy 124
	+sc_paste 125
	+sc_find 126
	+sc_mute 127
	+sc_volumeup 128
	+sc_volumedown 129

	+sc_lockingcapslock 130
	+sc_lockingnumlock 131
	+sc_lockingscrolllock 132

	+sc_kp_comma 133
	+sc_kp_equalsas400 134

	+sc_international1 135
	+sc_international2 136
	+sc_international3 137
	+sc_international4 138
	+sc_international5 139
	+sc_international6 140
	+sc_international7 141
	+sc_international8 142
	+sc_international9 143

	+sc_lang1 144
	+sc_lang2 145
	+sc_lang3 146
	+sc_lang4 147
	+sc_lang5 148
	+sc_lang6 149
	+sc_lang7 150
	+sc_lang8 151
	+sc_lang9 152

	+sc_alterase 153
	+sc_sysreq 154
	+sc_cancel 155
	+sc_clear 156
	+sc_prior 157
	+sc_return2 158
	+sc_separator 159
	+sc_out 160
	+sc_oper 161
	+sc_clearagain 162
	+sc_crsel 163
	+sc_exsel 164

	+sc_kp_00 176
	+sc_kp_000 177
	+sc_thousandsseparator 178
	+sc_decimalseparator 179
	+sc_currencyunit 180
	+sc_currencysubunit 181
	+sc_kp_leftparen 182
	+sc_kp_rightparen 183
	+sc_kp_leftbrace 184
	+sc_kp_rightbrace 185
	+sc_kp_tab 186
	+sc_kp_backspace 187
	+sc_kp_a 188
	+sc_kp_b 189
	+sc_kp_c 190
	+sc_kp_d 191
	+sc_kp_e 192
	+sc_kp_f 193
	+sc_kp_xor 194
	+sc_kp_power 195
	+sc_kp_percent 196
	+sc_kp_less 197
	+sc_kp_greater 198
	+sc_kp_ampersand 199
	+sc_kp_dblampersand 200
	+sc_kp_verticalbar 201
	+sc_kp_dblverticalbar 202
	+sc_kp_colon 203
	+sc_kp_hash 204
	+sc_kp_space 205
	+sc_kp_at 206
	+sc_kp_exclam 207
	+sc_kp_memstore 208
	+sc_kp_memrecall 209
	+sc_kp_memclear 210
	+sc_kp_memadd 211
	+sc_kp_memsubtract 212
	+sc_kp_memmultiply 213
	+sc_kp_memdivide 214
	+sc_kp_plusminus 215
	+sc_kp_clear 216
	+sc_kp_clearentry 217
	+sc_kp_binary 218
	+sc_kp_octal 219
	+sc_kp_decimal 220
	+sc_kp_hexadecimal 221

	+sc_lctrl 224
	+sc_lshift 225
	+sc_lalt 226
	+sc_lgui 227
	+sc_rctrl 228
	+sc_rshift 229
	+sc_ralt 230
	+sc_rgui 231
	+sc_mode 257
	+sc_audionext 258
	+sc_audioprev 259
	+sc_audiostop 260
	+sc_audioplay 261
	+sc_audiomute 262
	+sc_mediaselect 263
	+sc_www 264
	+sc_mail 265
	+sc_calculator 266
	+sc_computer 267
	+sc_ac_search 268
	+sc_ac_home 269
	+sc_ac_back 270
	+sc_ac_forward 271
	+sc_ac_stop 272
	+sc_ac_refresh 273
	+sc_ac_bookmarks 274

	+sc_brightnessdown 275
	+sc_brightnessup 276
	+sc_displayswitch 277
	+sc_kbdillumtoggle 278
	+sc_kbdillumdown 279
	+sc_kbdillumup 280
	+sc_eject 281
	+sc_sleep 282
)