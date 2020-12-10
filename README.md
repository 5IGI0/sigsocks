# SIGSocks

SIGSocks is a socks5 proxy implemented in **C**.

- [Build from source](#Build)
- [Usage](#Usage)


## Build


1. Clone the repo: ``git clone --recursive https://github.com/5IGI0/sigsocks.git``
2. Go to the repo folder cd sigsocks (or whatever you called it)
3. Execute ``make``
4. Get your executable in ``build`` folder

## Usage

run: ``./sigsocks``

run in verbose mode: ``./sigsocks -v`` 

listen on specific port: ``SOCKS_PORT=<port> ./sigsocks``