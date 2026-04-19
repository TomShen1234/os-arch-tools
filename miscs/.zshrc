# Slightly customize the shell prompt of the container

# Enable prompt substitution
setopt PROMPT_SUBST

# Basic prompt:
# %n = username
# %m = hostname (short)
# %~ = current directory (with ~ for home)
# %# = % for normal user, # for root
PROMPT='%n@%m:%~ %# '

# Enable colors
autoload -U colors && colors

# Optional: colored prompt
PROMPT='%F{green}%n@%m%f:%F{blue}%~%f %# '

# History settings
HISTSIZE=1000
SAVEHIST=1000
HISTFILE=~/.zsh_history

# Enable useful options
setopt autocd          # cd by typing directory name
setopt hist_ignore_dups
setopt share_history

# Completion system
autoload -Uz compinit
compinit

# Aliases (optional)
alias ll='ls -lah'
alias la='ls -A'
alias l='ls -CF'
alias ls='ls --color=auto'

# Print a welcome message
echo "Welcome to Computer Architecture and Operating Systems class!"
echo "Shared work directory: /workspace"
echo "Resources for hands-on activities and homeworks: /opt/resources"
