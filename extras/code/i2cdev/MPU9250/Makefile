#   Makefile for MPU9250 I2CDEV examples
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

ALL = MasterTest PassthruTest

MPUSRC = ../../../../src/

I2CINC = $(CPDB)/src
I2CSRC = $(CPDB)/extras/i2c/i2cdev/src
CPCMN =  $(CPDB)/extras/common

all: $(ALL)

MasterTest: MasterTest.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Master_I2C.o I2CDevI2C.o main.o timing.o errmsg.o
	g++ -std=c++11 -o MasterTest MasterTest.o MPU.o MPU9250.o MPU9250_Master.o MPU9250_Master_I2C.o I2CDevI2C.o main.o timing.o errmsg.o -li2c

MasterTest.o: MasterTest.cpp 
	g++ -std=c++11 -Wall -I$(CPCMN) -I$(MPUSRC) -c MasterTest.cpp

PassthruTest: PassthruTest.o MPU.o MPU9250.o MPU9250_Passthru.o I2CDevI2C.o main.o timing.o errmsg.o
	g++ -std=c++11 -o PassthruTest PassthruTest.o MPU.o MPU9250.o MPU9250_Passthru.o I2CDevI2C.o main.o timing.o errmsg.o -li2c

PassthruTest.o: PassthruTest.cpp 
	g++ -std=c++11 -Wall -I$(CPCMN) -I$(MPUSRC) -c PassthruTest.cpp

MPU.o: $(MPUSRC)/MPU.cpp 
	g++ -std=c++11 -Wall -I$(MPUSRC) -c $(MPUSRC)/MPU.cpp

MPU9250.o: $(MPUSRC)/MPU9250.cpp 
	g++ -std=c++11 -Wall -I$(MPUSRC) -c $(MPUSRC)/MPU9250.cpp

MPU9250_Master.o: $(MPUSRC)/MPU9250_Master.cpp 
	g++ -std=c++11 -Wall -I$(I2CINC) -I$(MPUSRC) -c $(MPUSRC)/MPU9250_Master.cpp

MPU9250_Passthru.o: $(MPUSRC)/MPU9250_Passthru.cpp 
	g++ -std=c++11 -Wall -I$(I2CINC) -I$(MPUSRC) -c $(MPUSRC)/MPU9250_Passthru.cpp

MPU9250_Master_I2C.o: $(MPUSRC)/MPU9250_Master_I2C.cpp 
	g++ -std=c++11 -Wall -I$(I2CINC) -I$(MPUSRC) -c $(MPUSRC)/MPU9250_Master_I2C.cpp

I2CDevI2C.o: $(I2CSRC)/I2CDevI2C.cpp 
	g++ -std=c++11 -Wall -I$(I2CINC) -c $(I2CSRC)/I2CDevI2C.cpp

main.o: $(CPCMN)/main.cpp 
	g++ -std=c++11 -Wall -c $(CPCMN)/main.cpp

timing.o: $(CPCMN)/timing.cpp 
	g++ -std=c++11 -Wall -I$(CPCMN) -c $(CPCMN)/timing.cpp

errmsg.o: $(CPCMN)/errmsg.cpp 
	g++ -std=c++11 -Wall -I$(CPCMN) -c $(CPCMN)/errmsg.cpp

run: MasterTest
	./MasterTest

edit:
	vim MasterTest.cpp

clean:
	rm -f $(ALL) *.o *~
