#   Makefile for MPU6050 I2CDEV examples
#  
#   Copyright (C) 2018 Simon D. Levy
#
#   This file is part of MPU.
#
#   MPU is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   MPU is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   You should have received a copy of the GNU General Public License
#   along with MPU.  If not, see <http://www.gnu.org/licenses/>.
  
# Change this to whereever you installed CrossPlatformDataBus
CPDB = $(HOME)/CrossPlatformDataBus
#CPDB = $(HOME)/Documents/Arduino/libraries/CrossPlatformDataBus

ALL = Basic

MPUSRC = ../../../../src

I2CINC = $(CPDB)/src
I2CSRC = $(CPDB)/extras/i2c/i2cdev/src
CPCMN =  $(CPDB)/extras/common

all: $(ALL)

Basic: Basic.o MPU.o MPU6xx0.o MPU6050.o I2CDevI2C.o main.o timing.o
	g++ -std=c++11 -o Basic Basic.o MPU.o MPU6xx0.o MPU6050.o I2CDevI2C.o main.o timing.o -li2c

Basic.o: Basic.cpp 
	g++ -std=c++11 -Wall -I$(MPUSRC) -c Basic.cpp

MPU6050.o: $(MPUSRC)/MPU6050.cpp  $(MPUSRC)/MPU6050.h
	g++ -std=c++11 -Wall -I$(I2CINC) -I$(MPUSRC) -c $(MPUSRC)/MPU6050.cpp

MPU6xx0.o: $(MPUSRC)/MPU6xx0.cpp  $(MPUSRC)/MPU6xx0.h
	g++ -std=c++11 -Wall -I$(I2CINC) -I$(MPUSRC) -c $(MPUSRC)/MPU6xx0.cpp

MPU.o: $(MPUSRC)/MPU.cpp  $(MPUSRC)/MPU.h
	g++ -std=c++11 -Wall -I$(MPUSRC) -c $(MPUSRC)/MPU.cpp

I2CDevI2C.o: $(I2CSRC)/I2CDevI2C.cpp 
	g++ -std=c++11 -Wall -I$(I2CINC) -c $(I2CSRC)/I2CDevI2C.cpp

main.o: $(CPCMN)/main.cpp 
	g++ -std=c++11 -Wall -c $(CPCMN)/main.cpp

timing.o: $(CPCMN)/timing.cpp 
	g++ -std=c++11 -Wall -I$(CPCMN) -c $(CPCMN)/timing.cpp

run: Basic
	./Basic

edit:
	vim Basic.cpp

clean:
	rm -f $(ALL) *.o *~
