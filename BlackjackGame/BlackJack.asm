section .data
	;string constants
	welcome_msg db "--- Welcome to Blackjack! ---", 0xA, 0xD, 0
	empty_line db 0xA, 0xD, 0
	main_options_msg db "Please choose options!", 0xA, 0xD, 0
	main_option1 db "1. Play game", 0xA, 0xD, 0
	main_option2 db "2. Exit", 0xA, 0xD, 0
	prompt_msg db "Your choice: ", 0
	invalid_msg db "Invalid option. Please try again.", 0xA, 0xD, 0

	inst_game_msg db "--- What would you like to do? ---", 0xA, 0xD, 0
	inst_game_options db "Please choose options!", 0xA, 0xD, 0
	inst_game_option1 db "1. Start", 0xA, 0xD, 0
	inst_game_option2 db "2. Help", 0xA, 0xD, 0
	inst_game_option3 db "3. Back to main menu", 0xA, 0xD, 0
	help_msg db "Blackjack is a card game where players try to get as close to 21 without exceeding it.", 0xA, 0xD, 0
	help_msg2 db "You will be given two cards at the start, and you can ask for one more any time.", 0xA, 0xD, 0
	help_msg3 db "If the total of your cards exceed 21, you loose.", 0xA, 0xD, 0

	game_init_msg1 db "The dealer has drawn two cards for you", 0xA, 0xD, 0
	game_init_msg2 db "Your card is: ", 0
	game_init_msg3 db "Your second card is: ", 0

	user_total_msg db "Your card total is: ", 0
	dealer_first_card_msg db "Dealer's first card is: ", 0
	dealer_total_msg db "Dealer's card total is: ", 0
	show_card_msg db "You drew: ", 0

	play_game_options db "Choose an option: ", 0xA, 0xD, 0
	play_game_option1 db "1. Draw a card", 0xA, 0xD, 0
	play_game_option2 db "2. Stop here", 0xA, 0xD, 0
	cards db 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10

	player_win_msg db "You won!", 0xA, 0xD, 0
	player_lose_msg db "You lost...", 0xA, 0xD, 0
	draw_msg db "Draw.", 0xA, 0xD, 0

section .bss

	;user input
	main_input resb 1
	gm_input resb 1
	play_input resb 1

	;cards
	user_card resb 1
	user_card2 resb 1
	dealer_card resb 1
	user_total resb 1
	dealer_total resb 1

	num_buffer resb 4

section .text
	global _start

_start:
	; Clear main_input buffer to avoid residual values
	mov byte [main_input], 0

	;print welcome msg
	mov eax, 4	;call syswrite system
	mov ebx, 1	;stdout
	mov ecx, welcome_msg 	;print welcome message
	mov edx, 32	;length of message
	int 0x80	;syscall

	;prit empty line
	mov eax, 4
	mov ebx, 1
	mov ecx, empty_line
	mov edx, 3
	int 0x80

	;print option msg
	mov eax, 4
	mov ebx, 1
	mov ecx, main_options_msg
	mov edx, 25
	int 0x80

	;print option1
	mov eax, 4
	mov ebx, 1
	mov ecx, main_option1
	mov edx, 15
	int 0x80

	;print option2
	mov eax, 4
	mov ebx, 1
	mov ecx, main_option2
	mov edx, 10
	int 0x80

	;print prompt msg
	mov eax, 4
	mov ebx, 1
	mov ecx, prompt_msg
	mov edx, 14
	int 0x80

	; Clear main_input buffer before taking user input
	mov byte [main_input], 0

	;take user input
	mov eax, 3	;call sysread system
	mov ebx, 0	;stdin
	mov ecx, main_input	;address to save input
	mov edx, 1	;length of input. as it is only one integer
	int 0x80	;run system call

	;check input
	cmp byte [main_input], '1'	;check '1' is given as input
	je game_menu

	cmp byte [main_input], '2'	;check '2' is given as input
	je exit_program

	;invalid input
	mov eax, 4
	mov ebx, 1
	mov ecx, invalid_msg
	mov edx, 36
	int 0x80
	jmp _start	; go back to _start


game_menu:
	; Clear gm_input buffer
	mov byte [gm_input], 0

	;display instant menu
	mov eax, 4
	mov ebx, 1
	mov ecx, inst_game_msg
	mov edx, 37
	int 0x80

	;display game menu options
	mov eax, 4
        mov ebx, 1
        mov ecx, inst_game_options
        mov edx, 25
        int 0x80

	;display game menu options 1, play
	mov eax, 4
	mov ebx, 1
	mov ecx, inst_game_option1
	mov edx, 11
	int 0x80

	;display game menu options 2, help
	mov eax, 4
	mov ebx, 1
	mov ecx, inst_game_option2
	mov edx, 10
	int 0x80

	;display game menu options 3, back
        mov eax, 4
        mov ebx, 1
        mov ecx, inst_game_option3
        mov edx, 23
        int 0x80

	;print prompt msg
        mov eax, 4
        mov ebx, 1
        mov ecx, prompt_msg
        mov edx, 14
        int 0x80

	; Clear gm_input buffer before taking user input
	mov byte [gm_input], 0

	;user input
	mov eax, 3	; sys_read
	mov ebx, 0	; stdin
	mov ecx, gm_input	; Address to save input
	mov edx, 1
	int 0x80

	; Check input
	cmp byte [gm_input], '1'
	je init_game_play

	cmp byte [gm_input], '2'
	je help

	cmp byte [gm_input], '3'
	je _start

	;invalid input
        mov eax, 4
        mov ebx, 1
        mov ecx, invalid_msg
        mov edx, 36
        int 0x80
        jmp game_menu

help:
	;print discriptions about the game
	mov eax, 4
        mov ebx, 1
        mov ecx, help_msg
        mov edx, 85
        int 0x80

	mov eax, 4
        mov ebx, 1
        mov ecx, help_msg2
        mov edx, 80
        int 0x80

	mov eax, 4
        mov ebx, 1
        mov ecx, help_msg3
        mov edx, 48
        int 0x80
	jmp game_menu

init_game_play:
	; Initialize player and dealer totals
        mov byte [user_total], 0
        mov byte [dealer_total], 0

	;draw two cards for user and dealer
	call draw_card
	mov [user_card], al
	add [user_total], al

	;explanation msg
	mov eax, 4
        mov ebx, 1
        mov ecx, game_init_msg1
        mov edx, 42
        int 0x80

	;Print card value
	mov eax, 4
        mov ebx, 1
        mov ecx, game_init_msg2
        mov edx, 15
	int 0x80

	mov eax, [user_card]
	call print_value

	;draw second card
	call draw_card
        mov [user_card2], al
        add [user_total], al

	;Print second card value
        mov eax, 4
        mov ebx, 1
        mov ecx, game_init_msg3
        mov edx, 22
        int 0x80

        mov eax, [user_card2]
	call print_value

	;Print user card total
	mov eax, 4
        mov ebx, 1
        mov ecx, user_total_msg
        mov edx, 21
        int 0x80

        mov eax, [user_total]
	call print_value

	;draw dealer's card
        call draw_card
        mov [dealer_card], al
        add [dealer_total], al

	;Print dealer's card value
        mov eax, 4
        mov ebx, 1
        mov ecx, dealer_first_card_msg
        mov edx, 25
        int 0x80

        mov eax, [dealer_card]
	call print_value
	jmp play_game

play_game:
	; Clear play_input buffer
	mov byte [play_input], 0

	;print options for gameplay
	mov eax, 4
        mov ebx, 1
        mov ecx, play_game_options
        mov edx, 21
        int 0x80

	;print option1
        mov eax, 4
        mov ebx, 1
        mov ecx, play_game_option1
        mov edx, 17
        int 0x80

	;print option2
        mov eax, 4
        mov ebx, 1
        mov ecx, play_game_option2
        mov edx, 15
        int 0x80

	;prompt msg
        mov eax, 4
        mov ebx, 1
        mov ecx, prompt_msg
        mov edx, 14
        int 0x80

	; Clear play_input buffer before taking user input
	mov byte [play_input], 0

	;user input
        mov eax, 3      ; sys_read
        mov ebx, 0	; stdin
        mov ecx, play_input	; Address to save input
        mov edx, 1
        int 0x80

        ; Check input
        cmp byte [play_input], '1'
        je draw_player_card

        cmp byte [play_input], '2'
        je dealer_turn

	;invalid input
        mov eax, 4
        mov ebx, 1
        mov ecx, invalid_msg
        mov edx, 36
        int 0x80
        jmp play_game

draw_card:
	rdtsc	;Load the current timestamp into EDX:EAX
	xor edx, edx	;Set EDX to 0
	mov ecx, 13	;Load 13 into ECX to use as the divisor
	div ecx		;Divide EAX by 13, remainder goes into EDX
	mov al, [cards + edx]	;Load the card value at index EDX from the 'cards' array into AL
	ret	;Return to the caller

draw_player_card:
	;Draw a random card and update player's total
	call draw_card
	mov [user_card], al
	add [user_total], al

	;Print card value
	mov eax, 4
	mov ebx, 1
	mov ecx, show_card_msg
	mov edx, 11
	int 0x80

	mov eax,[user_card]
        call print_value

        ;Print user card total
        mov eax, 4
	mov ebx, 1
        mov ecx, user_total_msg
        mov edx, 21
        int 0x80

	mov eax, [user_total]
        call print_value

	;Check if player's total exceeds 21
	cmp [user_total], 21
	ja player_loses
	jmp play_game

dealer_turn:
	cmp byte [dealer_total], 17
	jl draw_dealer_card  ; Dealer draws if total is 16 or less
	jmp check_result

draw_dealer_card:
	call draw_card
	mov [dealer_card], al
	add [dealer_total], al

	; Check if dealer's total exceeds 21
	cmp byte [dealer_total], 21
	ja player_wins
	ret

check_result:
	mov al, [user_total]
	cmp al, [dealer_total]
	je game_draw
	cmp byte [user_total], 21
	je player_wins
	cmp byte [dealer_total], 21
	je player_loses
	mov al, [user_total]
	cmp al, [dealer_total]
	jg player_wins
	jl player_loses

game_draw:
	mov eax, 4
        mov ebx, 1
        mov ecx, dealer_total_msg
        mov edx, 25
        int 0x80

	mov eax, [dealer_total]
	call print_value

	mov eax, 4
        mov ebx, 1
        mov ecx, draw_msg
        mov edx, 8
        int 0x80

	mov eax, 4
        mov ebx, 1
        mov ecx, empty_line
        mov edx, 3
        int 0x80

	jmp game_menu

player_wins:
	mov eax, 4
        mov ebx, 1
        mov ecx, dealer_total_msg
        mov edx, 25
        int 0x80

        mov eax, [dealer_total]
        call print_value

	mov eax, 4
        mov ebx, 1
        mov ecx, player_win_msg
        mov edx, 11
        int 0x80

        mov eax, 4
        mov ebx, 1
        mov ecx, empty_line
        mov edx, 3
        int 0x80

        jmp game_menu

player_loses:
	mov eax, 4
        mov ebx, 1
        mov ecx, dealer_total_msg
        mov edx, 25
        int 0x80

        mov eax, [dealer_total]
        call print_value

	mov eax, 4
        mov ebx, 1
        mov ecx, player_lose_msg
        mov edx, 16
        int 0x80

        mov eax, 4
        mov ebx, 1
        mov ecx, empty_line
        mov edx, 3
        int 0x80

        jmp game_menu

print_value:
	xor ecx, ecx	;Clear ECX (index for num_buffer)
	mov ebx, 10	;Divisor for modulo operation

	; Clear num_buffer before starting conversion
	mov edi, num_buffer
	mov byte [edi], 0
	mov byte [edi+1], 0
	mov byte [edi+2], 0
	mov byte [edi+3], 0


convert_loop:
	xor edx, edx	; Clear EDX before division
	div ebx		; Divide EAX by 10, quotient in EAX, remainder in EDX
	add dl, '0'	; Convert remainder to ASCII
	mov [num_buffer + ecx], dl	; Store ASCII character in buffer
	inc ecx			;Increment buffer index
	test eax, eax		;Check if quotient is 0
	jnz convert_loop	;If not 0, continue the loop
	dec ecx		; Adjust index to last valid character

print_loop:
	mov eax, 4	; sys_write
	mov ebx, 1	; stdout
	lea ecx, [num_buffer + ecx]	; Address of current character
	mov edx, 1	; Length of 1 character
	int 0x80	; Make syscall

	dec ecx		; Move to previous character in buffer
	jns print_loop	; Loop if ECX is not negative

	; Print newline after number
	mov eax, 4
        mov ebx, 1
        mov ecx, empty_line
        mov edx, 3
        int 0x80

	ret

exit_program:
	mov eax, 1
	mov ebx, 0
	int 0x80
