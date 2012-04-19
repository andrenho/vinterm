" local syntax file - set colors on a per-machine basis:
" vim: tw=0 ts=8 sw=8
" Vim color file
" Maintainer:	Andre Wagner <andre.nho@gmail.com>
" Last Change:	2012 Apr 19

set background=dark
hi clear
set t_so=[2m
set t_se=[2n

if exists("syntax_on")
  syntax reset
endif

let g:colors_name = "vinterm"
hi Normal		term=NONE
hi Comment		term=standout
hi Constant		term=underline
hi Special		term=bold
hi Identifier		term=bold
hi Statement 		term=bold
hi PreProc		term=bold
hi Type			term=bold
hi Function		term=bold
hi Repeat		term=bold
hi Operator		term=NONE
hi Ignore		term=underline
hi Error		term=reverse
hi Todo			term=reverse,bold

" Common groups that link to default highlighting.
" You can specify other highlighting easily.
hi link String		Constant
hi link Character	Constant
hi link Number		Constant
hi link Boolean		Constant
hi link Float		Number
hi link Conditional	Repeat
hi link Label		Statement
hi link Keyword		Statement
hi link Exception	Statement
hi link Include		PreProc
hi link Define		PreProc
hi link Macro		PreProc
hi link PreCondit	PreProc
hi link StorageClass	Type
hi link Structure	Type
hi link Typedef		Type
hi link Tag		Special
hi link SpecialChar	Special
hi link Delimiter	Special
hi link SpecialComment 	Special
hi link Debug		Special
