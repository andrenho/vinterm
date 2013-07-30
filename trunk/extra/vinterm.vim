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
hi Normal		cterm=NONE           term=NONE          
hi Comment		cterm=standout       term=standout
hi Constant		cterm=underline      term=underline
hi Special		cterm=bold           term=bold
hi Identifier		cterm=bold           term=bold
hi Statement 		cterm=bold           term=bold
hi PreProc		cterm=bold           term=bold
hi Type			cterm=bold           term=bold
hi Function		cterm=bold           term=bold
hi Repeat		cterm=bold           term=bold
hi Operator		cterm=NONE           term=NONE
hi Ignore		cterm=underline      term=underline
hi Visual		cterm=reverse        term=reverse
hi Error		cterm=reverse        term=reverse
hi Todo			cterm=reverse,bold   term=reverse,bold

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
