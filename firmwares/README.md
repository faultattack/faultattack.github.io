Building
--------
Install cmake and arm cross compiler. 
Project uses arm-none-eabi-gcc by default. 


Clone this git repo and within the repository:  

mkdir build 

cd build 

cmake .. 

make 

cmake will try to install the picoSDK automatically, at least if there is no PICO_SDK env defined. 

more instructions can be found at: https://github.com/raspberrypi/pico-sdk


