# Make sure you have FPT.tgz file in your root directory

# Install X-11 development tool
sudo apt-get install xorg openbox libx11-dev libxext-dev -y

# Install FPT
tar xfz FPT.tgz
ln    -s  /FPT/acom       /usr/local/bin/acom
ln    -s  /FPT/bcom       /usr/local/bin/bcom
ln    -s  /FPT/ycom       /usr/local/bin/ycom
ln    -s  /FPT/zcom       /usr/local/bin/zcom
./FPT/DotosC/compile_dotos
./FPT/DotosGPP/compile_dotos
./FPT/FileProcessors/compile
./FPT/make_FPT
./FPT/make_FPT_symbolic_links
