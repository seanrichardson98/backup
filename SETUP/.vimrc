"TABS"
set tabstop=4
set expandtab
set shiftwidth=4  
set autoindent   
set smartindent 
set cindent    

"STYLE"
set number
colo minimalist

"MACROS"
    "command-to-command (command)"

    "command-to-key (map)"

    "key-to-key (nmap)"

"OTHER"

"LaTeX - ulti-snips <------- DO THIS"

"function Glatex_stuff()
"    nmap \a i\begin{align*}<CR><CR>\end{align*}<Up>
"    nmap \e i\begin{equation*}<CR><CR>\end{equation*}<Up>
"    nmap \m i\marginpar{}<Left>
"    nmap \i i\begin{itemize}<CR><CR>\end{itemize}<Up>
"    nmap \d i\begin{definition}<CR><CR>\end{definition}<Up>
"    nmap \t i\begin{theorem}<CR><CR>\end{theorem}<Up>
"    nmap \p i\begin{proof}<CR><CR>\end{proof}<Up>
"    nmap \l i\begin{lemma}<CR><CR>\end{lemma}<Up>
"    nmap \q i\begin{prop}<CR><CR>\end{prop}<Up>
"    set textwidth=75
"    setlocal spell spelllang=en_us
"endfunction
"
"au BufNewFile,BufRead *.tex
"    \ call Glatex_stuff()
"
"au BufNewFile,BufRead *.latex
"    \ call Glatex_stuff()
